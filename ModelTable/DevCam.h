#ifndef DEVCAM_H
#define DEVCAM_H

#include <QDebug>
#include <QObject>

#include "DevSensor.h"
#include "resource/camNum.h"
#include "resource/cmd.h"

class DevCam : public DevSensor {
    Q_OBJECT
public:
    explicit DevCam(QString name = 0, QObject *parent = nullptr);
    ~DevCam();

    void setOnOff(bool);
    void setBOnOff(bool *);
    void setControl(bool);
    void setValue(propertyValue, int);
    void setAnglStart(propertyValue, int);
    void setValueMinMax(propertyValue, int, int);
    void getValueMinMax(propertyValue, int &, int &);
    void setNumber(int);
    void setViewingAngle(int);
    void setData(const propertyDate, const double);
    void analizCmd(const QByteArray *);
    void command(uchar nCmd, uchar byte2 = 0, uchar byte3 = 0);
    void setColorItem(const propertyDate &property, const QColor &color, const int n);
    Color getStateColor(int n = 0);
    QColor getColorVolt(const double &volt);

    int getNumber();
    int getViewingAngle();
    int getValue(propertyValue);
    int getAnglStart(propertyValue);

    bool isControl();

    double getData(const propertyDate);

    float getScale();

public slots:
    void devOnOff(int, bool);

signals:
    void upDateValue(int);
    void errorCamOff(int);

private:
    int number,
        anglStart[2],
        value[4],
        valueMinMax[4][2],
        nzoom[10],
        viewingAngle;
    bool bControl = false,
         *bOnOff;
    float scale;

    QColor getColorTemp(const double &temp);
    QColor getColorHumid(const double &humid);
};

#endif // DEVCAM_H
