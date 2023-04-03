#include "DevNT223_2.h"

DevNT223_2::DevNT223_2(QString name, QObject *parent) : DevOnOff(name, parent) {}

DevNT223_2::~DevNT223_2(){
    delete date;
}

void DevNT223_2::clearDate() {
    for (int i = 0; i < size; ++i) {
        date[i] = OFF;
    }
}

void DevNT223_2::command(uchar nCmd, uchar byte2, uchar byte3) {
char *cmd;
priorityRequest status;
Q_UNUSED(byte3);
    cmd = createCommand(nCmd, byte2);
    switch (nCmd) {
    case DevOn:
    case DevOff:
        status = priorityRequest::control;
        break;
    default:
        status = priorityRequest::query;
        break;
    }
    emit appendCmd(cmd, 6, status);
}


char *DevNT223_2::createCommand(int nCmd, int nDev) {
int i, sum = 0;
char *cmd = arr;
    cmd[0] = nCmd;
    cmd[1] = 0;
    cmd[2] = 0;
    cmd[3] = getInterface();
    cmd[4] = getDevChannel(nDev);
    for (i = 0; i < 5; i++)
        sum += int(cmd[i]);
    cmd[5] = (unsigned char)((255 - sum) + 1);
    return cmd;
}

void DevNT223_2::analizCmd(const QByteArray *cmd) {
QString error = getName() + " ошибка на каналах: ";
uchar Ncmd = cmd->at(0),
      byte_2 = cmd->at(1);
int i;
bool berr = false;

    if (byte_2 >= 0xF0) {
        createAnErrorMessage(getName(), byte_2);
        return;
    }
    switch (Ncmd) {
    case DevOn:
        if (getInterface() == cmd->at(3))
            for (i = 0; i < size; ++i) // запрос параметров КУ
                if (getDevChannel(i) == cmd->at(4)) {
                    emit devOnOff(getNDev(i), true);
                    return;
                }
        break;
    case DevOff:
//        if (getInterface() == cmd->at(3))
//            for (i = 0; i < size; ++i) // запрос параметров КУ
//                if (getDevChannel(i) == cmd->at(4)) {
//                    emit devOnOff(getNDev(i), false);
//                    return;
//                }
        break;
    case GetDev:
        uchar nBit = 0x01;
        for (i = 0; i < 6; i++) {
            if ((byte_2 & nBit))
                setDate(i, RUN);
            else {
                setDate(i, ERROR);
                berr = true;
                emit devOnOff(getNDev(i), false);
                error += (QString::number(i+1) + " ");
            }
            byte_2 = byte_2 >> 1;
        }
        if (berr) {
            errorMessage(error);
        }
        break;
    }
}

void DevNT223_2::setSizeDate(int nSize) {
    size = nSize;
    date = new STATE_DEV[size];
    channel = new uchar[size];
    channelDev = new int[size];
}

void DevNT223_2::setDate(const int num, const STATE_DEV nDate) {
    date[num] = nDate;
}

void DevNT223_2::setChannel(int num, int nDev, uchar nChannel) {
    channelDev[num] = nDev;
    channel[num] = nChannel;
}

int DevNT223_2::getNDev(int num) {
    return channelDev[num];
}

uchar DevNT223_2::getDevChannel(int num) {
    return channel[num];
}

STATE_DEV DevNT223_2::getDate(const int num) {
    return date[num];
}
