#ifndef DEVSENSOR_H
#define DEVSENSOR_H

#include "Devices.h"

class DevSensorPrivate;

class DevSensor : public Devices {
    Q_OBJECT
public:
    explicit DevSensor(QString name = 0, QObject *parent = nullptr);
    ~DevSensor();
    void command(uchar nCmd, uchar byte2 = 0, uchar byte3 = 0){}

protected:

//    virtual void setDate(const propertyDate, const double) = 0;
//    virtual void command(uchar, uchar, uchar) = 0;
//    virtual double getDate(const propertyDate) = 0;
    virtual QColor getColorTemp(const double &temp) = 0;
    virtual QColor getColorHumid(const double &humid) = 0;
    QColor colorForTemp = QColor(Qt::white);
    DevSensor(DevSensorPrivate &dd, QString name = 0, QObject *parent = nullptr);

private:
};

#endif // DEVSENSOR_H
