#ifndef DevNT16_H
#define DevNT16_H

#include "DevOnOff.h"

class DevNT16 : public DevOnOff {
    Q_OBJECT
public:
    explicit DevNT16(QString name = 0, QObject *parent = nullptr);
    ~DevNT16();
    void analizCmd(QByteArray *);

    bool isOn(int);

public slots:
    void command(uchar nCmd, uchar byte2 = 0, uchar byte3 = 0);

signals:
    void upDate();

private:
    bool on[16];
};

#endif // DevNT16_H
