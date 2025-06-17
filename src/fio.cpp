#include "fio.h"
#include <fstream>
#include "tlog.h"

FIO *FIO::m_instance = nullptr;
bool FIO::is_input_page_show=false;
int readFileAndToInt(std::string path)
{
    std::ifstream file(path);
    std::string line;
    int res = -1;
    if (file.is_open())
    {
        getline(file, line);

        try
        {
            res = std::stoi(line);
        }
        catch (const std::invalid_argument &e)
        {
            return -1;
        }
        catch (const std::out_of_range &e)
        {
            return -1;
        }
    }
    file.close();
    return res;
}
std::string readLine(std::string path)
{
    std::ifstream file(path);
    std::string line = "";
    if (file.is_open())
    {
        getline(file, line);
    }
    file.close();
    return line;
}
void writeLine(std::string path, std::string content)
{
    std::ofstream file(path);
    std::string line = "";
    if (file.is_open())
    {
        file << content;
    }
    file.close();
}
void FIO::set_input_status(bool status){
    is_input_page_show=status;
};
bool FIO::get_input_status(){
    return is_input_page_show;
};
bool FIO::is_ondemand()
{
    std::string cur_governor = readLine("/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor");
    return cur_governor == "ondemand";
}
void FIO::log(QString s){
    tlog_info(s.toStdString().c_str());
    return;
}
QString FIO::hex2str(QString hex){
    QByteArray a= QByteArray::fromHex(hex.toLatin1());
    return QString(a);
}
void FIO::set_ondemand()
{
    if (this->is_ondemand())
    {
        writeLine("/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor", "performance");
        writeLine("/sys/class/devfreq/fde40000.npu/governor", "userspace");
    }
    else
    {
        writeLine("/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor", "ondemand");
        writeLine("/sys/class/devfreq/fde40000.npu/governor", "simple_ondemand");
    }
}
int FIO::bat_cur()
{

    return readFileAndToInt("/sys/class/power_supply/battery/current_now");
}
int FIO::bat_vol()
{
    return readFileAndToInt("/sys/class/power_supply/battery/voltage_now");
}
int FIO::cpu_freq()
{
    return readFileAndToInt("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq");
}
FIO *FIO::getInstance()
{
    if (m_instance == nullptr)
    {
        // qDebug() << "Creating new FIO";
        m_instance = new FIO();
    }
    return m_instance;
}