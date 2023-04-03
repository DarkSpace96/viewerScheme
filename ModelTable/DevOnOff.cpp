#include "DevOnOff.h"

DevOnOff::DevOnOff(QString name, QObject *parent) : Devices(name, parent) {}

DevOnOff::~DevOnOff(){
}

void DevOnOff::command(uchar nCmd, uchar byte2, uchar) {
char *cmd;
priorityRequest status;
    cmd = createCommandOnOf(nCmd, byte2);
    switch (nCmd) {
    case DevOn:
    case DevOff:
        status = priorityRequest::control;
        break;
    default:
        status = priorityRequest::query;
        break;
    }
    qDebug() << "DevOnOff::command" << nCmd << objectName() << byte2;
    emit appendCmd(cmd, 6, status);
}

void DevOnOff::setColorItem(const propertyDate &property, const QColor &color, const int n) {
    if (property != LOG)
        return;
    colorValues[n] = color;
}

char *DevOnOff::createCommandOnOf(uchar nCmd, int nDev) {
int i, sum = 0;
char *cmd = arr;
    cmd[0] = nCmd;
    cmd[1] = 0;
    cmd[2] = 0;
    cmd[3] = getInterface();
    for (int i = 0; i < 8; ++i) {
        if (getNDev(i) == nDev) {
            cmd[4] = getDevChannel(i);
            break;
        }
    }
//    cmd[4] = getDevChannel(nDev);
    for (i = 0; i < 5; i++)
        sum += int(cmd[i]);
    cmd[5] = (unsigned char)((255 - sum) + 1);
    return cmd;
}

Color DevOnOff::getStateColor(int n) {
Color colorState;
    colorState.color = colorValues[n];
    colorState.raiting = GREEN;
    return colorState;
}

void DevOnOff::analizCmd(const QByteArray *cmd) {
QString error = getName() + " ошибка на каналах: ";
uchar Ncmd = cmd->at(0),
      byte_2 = cmd->at(1);
int i, num = 1;
bool berr = false;

    if (byte_2 >= 0xF0) {
        createAnErrorMessage(getName(), byte_2);
        return;
    }
    switch (Ncmd) {
    case DevOn:
        if (getInterface() == cmd->at(3))
            for (i = 0; i < sizeState; ++i) // запрос параметров КУ
                if (getDevChannel(i) == cmd->at(4)) {
                    emit devOnOff(getNDev(i), true);
                    return;
                }
        break;
    case DevOff:
        if (getInterface() == cmd->at(3))
            for (i = 0; i < sizeState; ++i) // запрос параметров КУ
                if (getDevChannel(i) == cmd->at(4)) {
                    emit devOnOff(getNDev(i), false);
                    return;
                }
        break;
    case GetDev:
        uchar nBit = 0x01;
        for (i = 0; i < 6; i++) {
            if ((byte_2 & nBit))
                setStateDev(RUN, i);
            else {
                setStateDev(ERROR, i);
                berr = true;
                emit devOnOff(getNDev(i), false);
                error += (QString::number(i+1) + " ");
            }
            byte_2 = byte_2 >> 1;
        }
//            if (i == 0) {
//                if ((byte_2 & num)) {
//                    setDate(i, RUN);
//                } else {
//                    setDate(i, ERROR);
//                    berr = true;
//                    emit devOnOff(getNDev(i), false);
//                    error += (QString::number(i+1) + " ");
//                }
//            } else {
//                if (byte_2 & (num*=2)) {
//                    setDate(i, RUN);
//                } else {
//                    setDate(i, ERROR);
//                    berr = true;
//                    emit devOnOff(getNDev(i), false);
//                    error += (QString::number(i+1) + " ");
//                }
//            }
//        }
        if (berr) {
            errorMessage(error);
        }
        break;
    }
}

void DevOnOff::setSizeDate() {
    size = sizeState;
    channel = new uchar[size];
    channelDev = new int[size];
    colorValues = new QColor[size];
    for (int i = 0; i < size; ++i) {
        colorValues[i] = Qt::green;
    }
}

void DevOnOff::setChannel(int num, int nDev) {
uchar nChannel = num * 0x11;
    channelDev[num] = nDev;
    channel[num] = nChannel;
}

int DevOnOff::getNDev(int num) {
    return channelDev[num];
}

uchar DevOnOff::getDevChannel(int num) {
    return channel[num];
}

void DevOnOff::onLamps(bool b, int nDev){
uchar nCmd = b ? DevOn : DevOff;
    command(nCmd, nDev);
}
