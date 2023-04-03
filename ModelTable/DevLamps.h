#ifndef DEVLAMPS_H
#define DEVLAMPS_H

#include <QByteArray>

#include "DevSensor.h"
#include "../resource/cmd.h"

class DevLamps : public DevSensor {
    Q_OBJECT
public:
    explicit DevLamps(QString name = 0, QObject *parent = nullptr);
    ~DevLamps();
    void setOnOff(bool, bool b = false);
    void setMatrixOnOff(int, bool);
    void setMatrix(int, int, bool);
    void setMatrix(int, int);
    void setMatrixValue(int, int, Qt::KeyboardModifiers);
    void getMatrixValue(int *, int *);
    void setGroup(int);
    void setNumber(int, int);
    void setNpch(uchar);
    void setColorItem(const propertyDate &property, const QColor &color, const int n = 0);
    void setData(const propertyDate, const double);
    void analizCmd(const QByteArray *);
    void command(uchar nCmd, uchar byte2 = 0, uchar byte3 = 0);

    int getGroup();
    int getNumber();
    int getNumberLamp();
    int getMatrixValue(int, bool b = false);

    bool isMatrixOn(int);
    bool isOn();
    bool polMatrix();

    double getData(const propertyDate);

    uchar getNpch();

signals:
    void upOnOff();
    void upDateValue();
    void onNT211(bool, int);

public slots:
    void devOnOff(int, bool);

private:
    int number,
        numberLamp,
        group,
        matrixValue[2];
    uchar npch;
    bool matrixOn[2];

    void newMatrix(int *, int *);

protected:
    QColor getColorTemp(const double &temp);
    QColor getColorHumid(const double &humid);
    Color getStateColor(int n = 0);
};

#endif // DEVLAMPS_H
