#ifndef DEVICES_H
#define DEVICES_H

#include <QObject>
#include <QByteArray>
#include <QVariant>
#include <QDebug>
#include <memory>

//#include "../communication/SendCmds.h"
#include "../resource/struct.h"

const int NON_VALUE = -1000000;

class DevicesPrivate;
class Logger;

class Devices : public QObject{

    Q_OBJECT
public:
    explicit Devices(QString name = 0, QObject *parent = nullptr);
    virtual ~Devices();

    void setName(QString);
    void setNameMod(QString);
    void setInterfaceCanal(uchar nInterface = 0, uchar nChannel = 0);
    void setOnOff(bool);
//    void setSenCmds(SendCmds *);
    void setStateDev(const STATE_DEV &state, const int n = 0);
    void setSizeState(int n = 1);
    void setShow(bool);
    void setSignal(bool);
    void clearData();

    uchar getChannel();
    uchar getInterface();

    bool isOn();
    bool isShow();
    bool isSignal();

    char *createCommand(uchar, uchar byte2 = 0, uchar byte3 = 0);

    const int &getCountRows();

    virtual void setData(const propertyDate, const double) = 0;

    virtual void setColorItem(const propertyDate &property, const QColor &color, const int n = 0) = 0;
    virtual double getData(const propertyDate) = 0;
    virtual Color getStateColor(int n = 0) = 0;
    virtual QVariant getColor(const int &column, const int &row);

    QString getName();
    QString getNameMod();

    QVariant getDataMod(int column, int row);

    STATE_DEV getStateDev(const int n = 0);

//    virtual void setSizeDate(const int) = 0;


Q_SIGNALS:
    void appendCmd(const char *cmd, int size, priorityRequest status);
    void upDate();
    void upData();
    void upState();
    void errorMessage(QString);

protected:
    int size, sizeState;
    uchar channel,
          interface;
    bool on = false,
         bShow = false,
         signal = false;
    char arr[8];
    double *data;
    QString name,
            nameMod,
            error,
            *namePropertys;
    STATE_DEV *stateDev;
    QColor *colorValues, *colorValuesText;
//    SendCmds *sendCmd;

    void createAnErrorMessage(QString, uchar);

protected:
    virtual void analizCmd(const QByteArray *) = 0;
    virtual void command(uchar nCmd, uchar byte2 = 0, uchar byte3 = 0) = 0;

    Devices(DevicesPrivate &dd, QString name = 0, QObject *parent = nullptr);

private:
    std::unique_ptr<Logger> logger;
};


#endif // DEVICES_H
