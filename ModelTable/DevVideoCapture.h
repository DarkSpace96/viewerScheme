#ifndef DEVVIDEOCAPTURE_H
#define DEVVIDEOCAPTURE_H

#include "DevCam.h"
#include "DevOnOff.h"

#include <QImage>
#include <QVector>
#include <QDir>
#include <QDebug>
#include <QElapsedTimer>
#include <QTimer>

//#include <functions.h>
//#include <AddingSoundThread.h>

#include <resource/struct.h>
#include <resource/camNum.h>

//#include <mpegd4.h>
//#include <mpg4types.h>
//#include <mpg4defs.h>
//#include <mmsystem.h>
#include <map>


class DevVideoCapture : public DevOnOff {
    Q_OBJECT
public:
    DevVideoCapture(QString name = 0, QObject *parent = nullptr);
    ~DevVideoCapture();
    void setMod(int);
    void setModScreen(QVector<Devices *>, int, int);
    void setModScreen(int *);
    void analizCmd(QByteArray *);
    void setOnOff(bool);
    void setDataMesseg(DataMesseg *messeg);
    void setDevCam(QVector<DevCam*> dev);
    void getModScreen(int *, int *, int *, int *);
    void setDirRec(QString *st);
    void startRec(int screen);
    void startRecCam(int screen);
    void stopRec(int screen);
    void stopRecCam(int screen);
    void resetRec();
    void command(uchar nCmd, uchar byte2 = 0, uchar byte3 = 0){}

    int getMod();
    int getModScreen(int);

    bool isRec(int scr);

    QString getFileName(int nScr);

    QVector<QImage *> getImage();

public slots:
    void addingSound(QString sound);

signals:
    void commandNT16(uchar nCmd, uchar byte2 = 0, uchar byte3 = 0);
    void upDate();
    void upDateMod();
    void upDateScreen();
    void upRec();
    void setRecordingLor(bool b, int num);

private slots:
    void endThread(int);

private:
    int mod,
        nStnd,
        nCard[4],
        nRec[4],
        *modScreen;
    bool rec[4], bInit[4], bPrev[4],
         bAviThr[3];
    unsigned long ulFOURCC;
    unsigned long ulPitch[4];
    QString *diskPath;
    QByteArray *pqba;
    QVector<QImage *> image;
    QVector<struct qim_par> parm;

//    QVector<listAviFile> newAviFile, aviFile;

//    QVector<AddingSoundThread *> addingSoundThread;

    void startVideo();

    QVector<DevCam*> devCam;

    DataMesseg *messeg;

    //Субтитры
    struct SubFiles {
        int id;//номер экрана
        QFile* strFile;
        QFile* messageFile;
        QTimer* timer;
        QTextStream* strStream;
        QTextStream* messageStream;
        int countFrame = 0;
        QElapsedTimer timeRecord;//Сколько времени записывается
        QTime lastRecord;//Время последней записи субтитра
    };
    std::map<int, SubFiles> subFiles;//int номер экрана
    template<typename ... Args>
    std::string string_format( const std::string& format, Args ... args );
    void initSub();
    void startRecordSub(QString fullFilename, int screen);
    void stopRecordSub(int screen);

};



#endif // DEVVIDEOCAPTURE_H
