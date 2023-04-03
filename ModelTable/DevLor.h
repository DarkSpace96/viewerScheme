#ifndef DEVLOR_H
#define DEVLOR_H

#include <QImage>
#include <QFile>
#include <QByteArray>
#include <QDir>

//#include <sys/times.h>

#include "DevOnOff.h"
#include "../resource/camNum.h"
#include "../resource/struct.h"

class DevLor : public DevOnOff {
    Q_OBJECT
public:
    explicit DevLor(QString name = 0, QObject *parent = nullptr);
    ~DevLor();

    void setOnOff(bool);
    void setConnection(bool);
    void setNumber(int);
    void setIpAddress(QString);
    void setDataMesseg(DataMesseg *messeg);
    void incNfl();
    void paintOn();
    void command(uchar nCmd, uchar byte2 = 0, uchar byte3 = 0){}

    void setData(const propertyDate property, const double value);
    void setColorItem(const propertyDate &property, const QColor &color, const int n);
    Color getStateColor(int n = 0);

    int getNumber();
    int getNfl();
    int getIfd();

    bool isRecording(); // получение состояния записи
    bool isPaint();

    QString getIpAddress();// получение IP-адреса
    QString getFullDir(); // получение пути к папке где сохраняются кадры
    QString getFileName();

    QImage *getImage();
    unsigned char *getPbmp();

    bool writeMark(QString *qstb, int *nr);

    void setMainDir(QString *qs);\

    DataMesseg *messeg;
signals:
//    void paint(int);
    void startTcpCam(int, bool);

public slots:
    void setRecording(bool b, int num);

private:
    int number,
        fps,
        nfl,
        ifd_qfm;
    bool connect,
         recording,
         paint = false;
    char buf0[4];
    unsigned char *pbmp;
    QString ipAddress,// IP-адресс;
            *mainDir, fullDir;
    QImage *image;
    QByteArray *pqba;
    QFile qfmarks;
};

#endif // DEVLOR_H
