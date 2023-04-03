#ifndef DEVNT19_H
#define DEVNT19_H

#include "DevOnOff.h"

class DevNT19 : public DevOnOff {
    Q_OBJECT
public:
    explicit DevNT19(QString name = 0, QObject *parent = nullptr);
    ~DevNT19();
    void analizCmd(QByteArray *);
    void setOnOff(int, bool);
    void command(uchar nCmd, uchar byte2 = 0, uchar byte3 = 0);

    bool isOn(int);

signals:
    void upDate();

private:
    bool on[3],
         voltage[6];
    char *createCommand(uchar nCmd, uchar nBloc);
};

#endif // DEVNT19_H
