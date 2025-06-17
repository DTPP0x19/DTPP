#ifndef FIO_H
#define FIO_H

#include <QObject>

class FIO : public QObject
{
    Q_OBJECT
public:

    static FIO *getInstance();
    Q_INVOKABLE QString hex2str(QString hex_str);
    Q_INVOKABLE void log(QString s);
    Q_INVOKABLE int bat_cur();
    Q_INVOKABLE int bat_vol();
    Q_INVOKABLE int cpu_freq();
    Q_INVOKABLE bool is_ondemand();
    Q_INVOKABLE void set_ondemand();
    Q_INVOKABLE void set_input_status(bool status);
    Q_INVOKABLE bool get_input_status();
private:
    static FIO *m_instance;
    static bool is_input_page_show;
};

#endif // FIO_H
