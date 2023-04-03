#ifndef DEVZENIT_H
#define DEVZENIT_H

#include <QDebug>
#include <QObject>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QMessageBox>
#ifdef Q_OS_UNIX
#include <QtSql>
#include <grid.h>
#endif

#include "DevSensor.h"

#ifdef Q_OS_UNIX
#include <Tmk.h>
#include <TmkThread.h>
#endif

//#include <functions.h>

#include "resource/struct.h"


#define NMT 1		//subaddress to MT
#define SHOW_TARGET_MI6 0x5800

struct MatrixData {
    float x = 0; // в какую точку смортрит наблюдатель по оси x. Значения в градусах
    float y = 0;// в какую точку смортрит наблюдатель по оси y. Значения в градусах.
    float rotationZ = 0; // поворот фигигуры вдоль оси z. Передавать градусы.
    float scale = 3; // масштаб. Увеличение, уменьшение размера. Делаем из этого эффект безразмерности фигуры
    float zoom = 1; // Приближение, отдаление зума камеры. (было 1)
    float frustum = 1;//Усечение пирамиды
    float k3 = 1.5;//Коэфицент увеличение поворота
protected:
    void setData(const MatrixData& m) {
        (*this) = m;
    }
};

struct GridData : MatrixData {
    float height = 4001; // отстояние в сантиметрах. Отдаление, приближение сетки.
    float pitchingX = 0; // качка курсовая. Пример, ударил волной борта корабля. Крениться в право, либо в лево..
    float pitchingY = 0; // качка киливая. Подъем и спуск коробля
    float bmm = 1000;//размер ячейки в мм
    float alpha = 46;//Угол обзора камеры. Наподобие закрытия и открытия глаза. В градусах.
};



class DevZenit : public DevSensor {
    Q_OBJECT
public:
    explicit DevZenit(DataMesseg *messeg, GridData *data, QString name = "", QObject *parent = nullptr);
//    GridData getGridData();
    ~DevZenit();
//    void setDataMesseg(DataMesseg *messeg);
    void setMarks(int nCam, unsigned short int un);

    int *getArrDate();

    QString getDataBaseSql();

    //QSqlError writeTargetInBase(QString camName, QString name);

signals:
    void masseg(QString);
    void upFrame();
    void updateGrid();
    void updateArr();
    void createCommit();
    void setFileName(QString qs, bool lor);
    void dateFromOnOff(bool);

public slots:
    void tmkCreated(int);
    void tmkNotCreated(int);
    void miRecv(int, int);
    void clearbTA13U();
    void checkZar();
    void analizCmd(const QByteArray *) {}

private:
    int *arrDate,
        cntz,
        nzone_w,
        nzone_h,
        mpg = 0, mpm = 0, mps = 0, mpsf, mpmf,
        mlg = 0, mlm = 0, mls = 0, mlsf, mlmf;
    unsigned short int *bufc,
                       *bufd;
    char cp,
         cl;
    bool bTA1_3U[2],
         showingFromZarya,
         bzar,
         bdate1 = false,	//установите значение 1 после первого получения даты и времени
         bdate = false;	//установите значение 1 после настройки QDate и QTime;
    struct dttm dt6, dt6_1;

//    TmkThread *ptmkth;// поток для "Зенита"
//    Tmk *ptmk;// объект для работы с tmk

    QTimer tmr,
           tmr_z;
    DataMesseg *messeg;
    QString dataBaseSql;//str_Lat, str_Lon;
    GridData *gridData;

    void fillBufd0();
    void getDttm(dttm *pdttm, unsigned short *buf);
    void setDateTime(int hh, int mm, int ss, int dd, int MM, int yyyy, bool bZenit);
    void fillAngle(QString *qStr, int ng, int nm, int nmm, char cp);
    void setDistance(int nh, int n20);
    void setDate(const propertyDate, const double) {};
    void command(uchar nCmd, uchar byte2 = 0, uchar byte3 = 0){}
    double getDate(const propertyDate) {return double();};

    void getTargetMi6(QDateTime qdt);
//    QSqlError writeQStcmnInBase(QString commit);
//    QSqlError initDb();
};

#endif // DEVZENIT_H
