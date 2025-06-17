#include "llmchat.h"
#include "json.hpp"
#include "tlog.h"

#include "QtNetwork/qtnetworkglobal.h"
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QThread>
#include <QJsonDocument>
#include <QMutex>
#include <QNetworkConfigurationManager>

using json = nlohmann::json;
LLMChat *LLMChat::m_instance = nullptr;
QString LLMChat::apiChannel = "deepseek";
LLMChat::LLMChat(QObject *parent)
    : QObject{parent}
{
    //  初始化一个新的QNetworkAccessManager对象
    this->setQNAM(new QNetworkAccessManager(this));
    QNetworkConfigurationManager QNAMConfig;
    // 设置QNetworkAccessManager的默认网络配置
    this->QNAM()->setConfiguration(QNAMConfig.defaultConfiguration());

    connect(QNAM(),
            &QNetworkAccessManager::finished,
            this,
            &LLMChat::streamFinished);
};

LLMChat *LLMChat::getInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new LLMChat();
    }
    return m_instance;
}
QNetworkRequest LLMChat::prepareRequest()
{
    QNetworkRequest request;
    
    request.setUrl((QUrl("https://api.deepseek.com/chat/completions")));
    request.setRawHeader("Authorization", "Bearer 你的TOKEN");
    
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=UTF-8");
    request.setTransferTimeout(20000);

    return request;
}
void LLMChat::set_apiChannel(QString status)
{   
    apiChannel = status;
}
QString LLMChat::get_apiChannel_status()
{
    return apiChannel;
}
void LLMChat::createChat(const QString &text)
{
    try
    {
        json j;
        // 提取所有content字段并合并
        QNetworkRequest request = prepareRequest();

        auto jr = json::parse(text.toStdString());
        if (jr.is_array())
        {
            // 构造message列表
            j["messages"] = jr;
        }
        else
        {
            emit msgErr("[QML->CPP]JSON传参错误");
        }
        j["temperature"] = 1;
        j["top_p"] = 1;

        j["model"] = "deepseek-reasoner";
        
        j["stream"] = true;
        j["max_tokens"] = 16384;

        m_reply = this->QNAM()->post(request, j.dump().data());
        connect(m_reply, SIGNAL(readyRead()), this, SLOT(streamReceived()));
    }
    catch (json::parse_error &e)
    {
        emit msgErr("[QML->CPP]JSON解析错误");
    }
}

void LLMChat::streamReceived()
{
    try
    {
        QString res_str = m_reply->readAll();
        //  使用istringstream来逐行读取字符串
        std::istringstream iss(res_str.toStdString());
        std::string line;
        std::vector<std::string> lines_with_data;
        // 提取所有content字段并合并
        std::string delta_content = "";
        std::string delta_thinking = "";
        // 逐行读取
        while (std::getline(iss, line))
        {
            // 检查行是否以"data:"开头
            if (!line.empty() && line.substr(0, 5) == "data:" && line != "data: [DONE]")
            {
                QString trimed_str = QString::fromStdString(line).section("data:", 1, 1).trimmed();
                // if(!trimed_str.contains("[DONE]")){
                //     qDebug()<<trimed_str;
                auto jr = json::parse(trimed_str.toStdString());

                // 提取所有content字段并合并
                for (const auto &choice : jr["choices"])
                {
                    if (choice.contains("delta"))
                    {
                        if (choice["delta"].contains("content") && !choice["delta"]["content"].is_null())
                        {
                            delta_content += choice["delta"]["content"].get<std::string>();
                        }

                        if (choice["delta"].contains("reasoning_content") && !choice["delta"]["reasoning_content"].is_null()&&!choice["delta"]["reasoning_content"].get<std::string>().empty())
                        {
                            delta_thinking += choice["delta"]["reasoning_content"].get<std::string>();
                        }
                    }
                    else
                    {
                        continue;
                    }
                }
            }
        }
        emit contentReceived(QString::fromStdString(delta_content));
        emit thinkingReceived(QString::fromStdString(delta_thinking));
    }
    catch (json::parse_error &e)
    {
        emit msgErr(QString("[JSON错误]" + QString::fromStdString(e.what())));
    }
}
void LLMChat::streamFinished(QNetworkReply *reply)
{
    if (reply->error())
    {
        emit msgErr("[网络错误]" + reply->errorString());
    }

    emit msgFinished();
}

QNetworkAccessManager *LLMChat::QNAM() const
{
    return m_QNAM;
}

void LLMChat::setQNAM(QNetworkAccessManager *value)
{
    m_QNAM = value;
}
