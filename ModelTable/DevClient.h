#ifndef DEVCLIENT_H
#define DEVCLIENT_H

#include <QImage>
#include <QByteArray>

#include "DevOnOff.h"

#include "../resource/camNum.h"

class DevClient : public DevOnOff {
    Q_OBJECT
public:
    DevClient(QString name = 0, int nCln = 0, QObject *parent = nullptr);
    ~DevClient();

    void setMod(int);
    void setModScreen(QVector<Devices *>, int, int);
    void setModScreen(int *);
    void analizCmd(QByteArray *);
    void setOnOff(bool);
    void getModScreen(int *, int *, int *, int *);
    void setArrDate(int *arr);
    void command(uchar nCmd, uchar byte2 = 0, uchar byte3 = 0){}
    void setImage(QImage *image);

    int getMod();
    int getModScreen(int);

public slots:
    void updateArr();
    void updateMenu();

signals:
    void commandNT16(uchar nCmd, uchar byte2 = 0, uchar byte3 = 0);
    void upDate();
    void upDateMod();

private:
    int mod,
        nClient,
        *modScreen,
        *arrDate;
    bool menu[8];
    QImage *image;
};

#endif // DEVCLIENT_H
