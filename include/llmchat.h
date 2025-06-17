#ifndef LLMCHAT_H
#define LLMCHAT_H
#include <QNetworkAccessManager>
#include <QNetworkReply>

class LLMChat : public QObject
{
    Q_OBJECT
public:
    static LLMChat *getInstance();
    Q_INVOKABLE void set_apiChannel(QString status);
    Q_INVOKABLE QString get_apiChannel_status();
signals:
    void thinkingReceived(const QString &text);
    void contentReceived(const QString &text);
    void msgFinished();
    void msgErr(const QString &text);

public slots:
    void createChat(const QString &text);
private slots:
    void streamFinished(QNetworkReply *reply);
    void streamReceived();

private:
    static QString apiChannel;
    // 静态成员变量，用于存储LLMChat的单例实例
    static LLMChat *m_instance;
    // 构造函数，用于创建LLMChat对象，并指定其父对象
    explicit LLMChat(QObject *parent = nullptr);
    // 静态方法，用于获取LLMChat的单例实例
    static LLMChat *llmchat();
    // 静态方法，用于设置LLMChat实例，并传入一个Manager对象
    static void setLLMChat(const LLMChat *llmchat);
    
    // 获取当前使用的QNetworkAccessManager对象
    QNetworkAccessManager *QNAM() const;
    // 设置当前使用的QNetworkAccessManager对象
    void setQNAM(QNetworkAccessManager *value);
    //处理响应json
    void processJson(const QByteArray &list);
    
    // 用于存储重试次数的成员变量
    qint16 m_retries;
    // 用于存储网络响应的成员变量
    QNetworkReply *m_reply;
    // 用于存储网络访问管理器的成员变量
    QNetworkAccessManager *m_QNAM;
    // 根据给定的URL准备一个QNetworkRequest对象
    QNetworkRequest prepareRequest();
};

#endif // LLMCHAT_H
