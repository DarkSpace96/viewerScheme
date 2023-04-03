#ifndef DEVDISK_H
#define DEVDISK_H

#include <QFile>
#include <QDir>

#include "Devices.h"

#ifdef Q_OS_UNIX
#include <sys/stat.h>
#include <sys/statvfs.h>
#endif

class DevDisk : public Devices {
    Q_OBJECT
public:
    explicit DevDisk(QString name, QString path, QObject *parent = nullptr);
    void command(uchar nCmd, uchar byte2 = 0, uchar byte3 = 0){}
    void setNextDisk(DevDisk *disk);
    void calculateDiskSpace() ;
    void setNumber(int n) {nmber = n;} // Назначение № дика
    void getValue(int &maxValue, int &value, int &hours, int &mins);
    void setData(const propertyDate, const double);
    void clearData();
    int getNumber() {return nmber;} // Возвращение № дика
    bool checkDisk();
    bool exists();
    bool isFillDisk();
    QString getPath();
    QString getStringError();
    DevDisk* getNextDisk();

    void setColorItem(const propertyDate &property, const QColor &color, int n = 0);
    Color getStateColor(int n = 0);

    double getData(const propertyDate);

    bool bExists;

    void analizCmd(const QByteArray *cmd){Q_UNUSED(cmd)}

signals:

public slots:

private:
    int maxValue, //размер диска
        value,// заполнение диска
        hours,// заполнение в часах
        mins, // заполнение в минутах
        nmber;// № диска
    int size;
    QString error;// строка с ошибкой
    QString path;// путь к диску
    DevDisk* nextDisk;// следующий диск

    void getFree(const char *flpth, int *npr, int *nab, int *nbb);
    int getFreeR(const char *flpth, int *npr, int *nab, int *nbb);

};

#endif // DEVDISK_H
