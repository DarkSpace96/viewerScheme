#include "DevPort.h"

DevPort::DevPort(QString name, QObject *parent) : DevOnOff(name, parent) {
char buft[32];
    size = 0;
    colorValues = new QColor[size];
    colorValues[0] = Qt::green;
    setSizeState();
    for (int i = 0; i < 4; i++) {
        sprintf(buft, "/dev/ttyAP%d", i);
        port.append(new QSerialPort(QString(buft)));
//        portInf.append(new QSerialPortInfo(*port[i]));
        sprintf(buft, "port_%02d", i);
        port[i]->setObjectName(QString(buft));
        port[i]->setBaudRate(QSerialPort::Baud57600);
        port[i]->setParity(QSerialPort::NoParity);
        port[i]->setStopBits(QSerialPort::OneStop);
        port[i]->setFlowControl(QSerialPort::NoFlowControl);
        port[i]->setDataBits(QSerialPort::Data8);
        fprintf(stdout,"SerialPort: %d %s\n",i,port[i]->portName().toLatin1().data());
        fflush(stdout);
//        QObject::connect(port[i], &QSerialPort::errorOccurred, this, &DevPort::sportError);
//        if (!port[i]->open(QIODevice::ReadWrite)) {
//            fprintf(stdout,"SerialPort %d %s was not opened.\n",i,
//                    port[i]->portName().toLatin1().data());
//            fflush(stdout);
//            setStateDev(RUN);
//            on = true;
//        }
    }
}

DevPort::~DevPort(){
//    delete date;
}

QVector<QSerialPort *> DevPort::getPort() {
    return port;
}

void DevPort::sportError(QSerialPort::SerialPortError err) {
    fprintf(stdout,"sportError: %d\n",(int)err);
    fflush(stdout);
}
