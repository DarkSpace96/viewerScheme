#ifndef DEVNT223_2_H
#define DEVNT223_2_H

#include <QObject>

#include "DevOnOff.h"
#include "../resource/cmd.h"

class DevNT223_2 : public DevOnOff {
    Q_OBJECT
public:
    explicit DevNT223_2(QString name = 0, QObject *parent = nullptr);
    ~DevNT223_2();
    void clearDate();
    void setSizeDate(int);
    void setChannel(int, int, uchar);
    void analizCmd(const QByteArray *);
    void command(uchar nCmd, uchar byte2 = 0, uchar byte3 = 0);

    int getNDev(int);

    uchar getDevChannel(int);

    int *channelDev;
    uchar *channel;

Q_SIGNALS:
    void devOnOff(int, bool);

private:
    char *createCommand(int, int);
    void setDate(const int, const STATE_DEV);
    STATE_DEV getDate(const int);


};

#endif // DEVNT223_2_H
