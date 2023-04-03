#ifndef STRUCRUR
#define STRUCRUR
#include <QFont>
#include <QVector>
#include <QMatrix>
#include <QImage>
//#include <QHostAddress>
#include <QDateTime>

enum priorityRequest{
    keyboard,
    control,
    query
};

enum RAITING_COLOR{
    GRAY = 0,
    GREEN = 1,
    YELLOW = 2,
    RED = 3
};

struct Color {
    RAITING_COLOR raiting;
    QColor color;
};

enum STATE_DEV {
    OFF = 0,
    RUN = 1,
    NOSIGNAL = 2,
    ERROR = 3,
};

enum propertyDate {
    HUMIDITY = 0,
    TEMPERATURE = 1,
    VOLTAGE = 2,
    TIME = 3,
    PING = 4,
    DISK_SPACE = 5,
    LOG = 6
};

enum propertyValue {
    PLACE = 0,
    COURSE = 1,
    FOCUS = 2,
    SCALE = 3,
};

struct DataMesseg {
    int nhp;//Отстояние
    int npp;//Отклонение по углу места
    int ncp;//Отклонение по курсововму углу
    QString time;
    QString date;
    QString distance;
    QString latitibe;//Широта
    QString longitube;//Долгота
    QString messeg;
};

typedef struct qim_par{
//contains QImage parameters for callback function
  int	nw, ///< Ширина картинки от камеры
        nh, ///< Высота картинки от камеры
        nb,	///< Количество байтов на пиксель
        ngot,
        npr;
  QImage *pqi;
}qim_par,*pqim_par;

typedef struct dttm{
    int myear,	//год-2000
        month,	//1 - 12
        mday,	// 1 - 31
        mhour,	//0 - 23
        min,	//0 - 59
        msec;	//
}dttm;

typedef struct cannel{
    uchar nInterface;
    uchar nChannel;
}cannel;

typedef union anglez{
    int iang;
    unsigned int ang;
    unsigned short int an2[2];
}anglez;

union val{
    short int nval;
    unsigned char buf[2];
};

typedef struct valueOfGrid{
    int numberOfCam;
    int ncurs;
    int nplace;
    int zoom;
}valueOfGrid;

typedef struct dirList {
    QString dir;
    QDateTime create;
}dirList;

typedef struct stCamPort{// порты к КУ
    int nport, //порт
        nInterface, // интерфейс
        nChannel; // канал
}stCamPort;

typedef struct stNt211Port{// порты к NT211
    int nInterface, // интерфейс
        nChannel; // канал
}stNt211Port;

typedef struct stPspPort{// порты к PSP
    char nInterface, // интерфейс
    nChannel[4], // канал
    npch[4];
}stPspPort;

typedef struct modcnt{
//structure to transmit parameters to the mode dialog (moddial)
    int mod[3],	//pointer to the array with parameters
        iw,	//width
        ih,	//heigth
        iswn,	//mfn->maxWidth
        ishn,	//mfn->lineSpacing
        ic;	//palette number
    QFont *mfn;
}modcnt,*pmodcnt;

struct prmUdp{
//structure to explanation udp-camera parameter
    char name[16];	//name
    unsigned int addr,	//register address from udp_cam.h
                 reg;	//register value, ncur=reg&0xfff
    int min,max,ncur,	//for digital parameter - min, max and current values
        nman,nauto,noo,nro,	//Manualmode,AutoMode,OnOff,ReadOut
        npar,				//parameter's number for setting
        oldSet,
        avto;
};

typedef struct stPsp{// данные по группе ПСП
    QVector<QVector<int>> vaule;//значение матриц светильников в группе
    bool activate,// вкл/выкл группу ПСП
         actSl[8];// вкл/выкл матрицы светильника
    int vauleGr,// Значение группы ПСП
        nInterface[4], nChannel[4]; // Блок интерфейса и каналл
}stPsp;

typedef struct PwPar{
    bool bNT203[2], bMIG3612, bTA1_3U[2];
    int ntemp;
}PwPar,*pPwp;

typedef struct pr_nt211{
    bool error[6],
         on[6];
    int cam[6];
}pr_nt211;

typedef struct InpOut{
//for target and source Rect
    int ntX,ntY,ntW,ntH,nsX,nsY,nsW,nsH;
    QMatrix qm;
}InpOut,*pInpOut;

typedef struct sym_par{
//structure to transmit parameters to the window with parameters
    int iswn,ishn,	//font width/height
        iwc,ihc,	//cell width/height
        iw,ih,	//window width/height
        nfsz,
        ic;	//palette number
    char fnm[32];
    QFont *mfn;
}sym_par,*psymp;

typedef struct Zenit{
    int nhp,
        ncp,
        npp;
    int size = 500;
}Zenit;

typedef union tmvl{
  long tv2_sec[2];
  char c2sec[16];
}tmvl;

typedef struct indcd{
//struct for the code recognizing information
  int min,	//minimal code value
    max,	//maximal code value
    nmin;	//1st symbol's number with code min in sorted symbols array
bool	is;		//1 if there are elements with this length otherwise 0
}indcd, *pindcd;

typedef struct jpg_len{
//struct for jpg length, array must be filled by 0xff
  char c4[4];
  int l;
}jpg_len;

typedef union uval{
  unsigned short int uval;
  unsigned char ubuf[2];
}uval;

typedef union uns{
  int n;
  unsigned char uc[4];
}uns;

typedef union uns8{
  int64_t n;
  unsigned char uc[8];
}uns8;

typedef union usz {
    int n;
    char ar[4];
} usz;

typedef union usi2si {
    short int n;
    unsigned short int u;
} usi2si;

#endif // STRUCRUR

