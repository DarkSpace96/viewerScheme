#ifndef DEVONOFF_H
#define DEVONOFF_H

#include "Devices.h"

#include "../resource/cmd.h"

#include <QDebug>

class DevOnOffPrivate;

class DevOnOff : public Devices {
    Q_OBJECT
public:
    explicit DevOnOff(QString name = 0, QObject *parent = nullptr);
    ~DevOnOff();
    void setSizeDate();
    void setChannel(int, int);
    void analizCmd(const QByteArray *);
    void command(uchar nCmd, uchar byte2 = 0, uchar byte3 = 0);

    void setData(const propertyDate property, const double value) {Q_UNUSED(property)Q_UNUSED(value)}
    void setColorItem(const propertyDate &property, const QColor &color, const int n = 0);

    int getNDev(int);
    double getData(const propertyDate){return NON_VALUE;}

    uchar getDevChannel(int);

    int *channelDev;
    uchar *channel;

public slots:
    void onLamps(bool, int);

private:
    char *createCommandOnOf(uchar nCmd, int nDev);

Q_SIGNALS:
    void devOnOff(int, bool);

protected:
//    virtual void command(int, int, int);
    Color getStateColor(int n = 0);

    DevOnOff(DevOnOffPrivate &dd, QString name = 0, QObject *parent = nullptr);
};

#endif // DEVONOFF_H
