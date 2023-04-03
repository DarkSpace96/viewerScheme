#ifndef DEVCPU_H
#define DEVCPU_H

#include <QDebug>
#include <QObject>
#include <QTimer>
#include <QFile>

#include "DevSensor.h"

class DevCPU : public DevSensor {
    Q_OBJECT
public:
    explicit DevCPU(QString name = 0, QObject *parent = nullptr);
    ~DevCPU();
    void setDate(const propertyDate property, const double nDate);
    void command(uchar nCmd, uchar byte2 = 0, uchar byte3 = 0){}
    double getDate(const propertyDate property);

private slots:
    void timeout();

private:
    QTimer *timer;

    int tempMax();
};

#endif // DEVCPU_H
