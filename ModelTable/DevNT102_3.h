#ifndef DEVNT102_3_H
#define DEVNT102_3_H

#include <QDebug>
#include <QObject>

#include "DevSensor.h"
#include "../resource/cmd.h"

class DevNT102_3 : public DevSensor {
    Q_OBJECT
public:
    explicit DevNT102_3(QString name = 0, QObject *parent = nullptr);
    ~DevNT102_3();

    void analizCmd(const QByteArray *);
    void setData(const propertyDate, const double);
    void command(uchar nCmd, uchar byte2 = 0, uchar byte3 = 0);
    double getData(const propertyDate);

    void setNumber(int nNumber);
    int getNumber();

    void setColorItem(const propertyDate &property, const QColor &color, int n = 0);
    Color getStateColor(int n = 0);

public slots:
    void devOnOff(int, bool);

private:
    int number;

    QColor getColorTemp(const double &temp);
    QColor getColorHumid(const double &humid);
};

#endif // DEVNT102_3_H
