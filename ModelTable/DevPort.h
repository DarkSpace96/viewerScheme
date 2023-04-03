#ifndef DEVPORT_H
#define DEVPORT_H

//class QSerialPort;
#include <QSerialPort>

#include "DevOnOff.h"

class DevPort : public DevOnOff {
    Q_OBJECT
public:
    DevPort(QString name = 0, QObject *parent = nullptr);
    ~DevPort();
    QVector<QSerialPort *> getPort();
    void command(uchar nCmd, uchar byte2 = 0, uchar byte3 = 0){}

private slots:
    void sportError(QSerialPort::SerialPortError);

private:
    QVector<QSerialPort *>  port;
};

#endif // DEVPORT_H
