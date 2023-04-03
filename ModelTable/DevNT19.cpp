#include "DevNT19.h"

DevNT19::DevNT19(QString name, QObject *parent) : DevOnOff(name, parent) {
    for (int i = 0; i < 3; ++i) {
        on[i] = true;
    }
    setInterfaceCanal(0xE3);
}

DevNT19::~DevNT19() { }

void DevNT19::analizCmd(QByteArray *cmd) {
QString error = tr("НТ-19: ");
uchar Ncmd = cmd->at(0),
      byte_2 = cmd->at(1),
      byte_3 = cmd->at(2);
    if (byte_2 >= 0xF0) {
        createAnErrorMessage(error, byte_2);
        return;
    }
    switch (Ncmd) {
    case GetDev:
        uchar nBit = 0x01;
        bool newOn[3];

        newOn[0] = byte_2 & nBit;
        byte_2 = byte_2 >> 1;
        voltage[0] = byte_2 & nBit;
        byte_2 = byte_2 >> 1;
        voltage[1] = byte_2 & nBit;
        byte_2 = byte_2 >> 1;

        newOn[1] = byte_2 & nBit;
        byte_2 = byte_2 >> 1;
        voltage[2] = byte_2 & nBit;
        byte_2 = byte_2 >> 1;
        voltage[3] = byte_2 & nBit;

        newOn[2] = byte_3 & nBit;
        byte_3 = byte_3 >> 1;
        voltage[4] = byte_3 & nBit;
        byte_3 = byte_3 >> 1;
        voltage[5] = byte_3 & nBit;

        for (int i = 0; i < 3; ++i) {
            if (newOn[i] != isOn(i))
                on[i] = newOn[i];
        }
        break;
    }
}

void DevNT19::setOnOff(int num, bool b) {
//char *cmd;
//priorityRequest status;
uchar nCmd;
    if (isOn(num) == b)
//    if (on[num] == b)
        return;
    on[num] = b;
    nCmd = on[num] ? DevOn : DevOff;
    command(nCmd, num);
//    cmd = createCommand(nCmd, num);
//    switch (nCmd) {
//    case DevOn:
//    case DevOff:
//        status = priorityRequest::control;
//        break;
//    }
//    emit appendCmd(cmd, 6, status);
}

void DevNT19::command(uchar nCmd, uchar byte2, uchar) {
char *cmd;
priorityRequest status;
    cmd = createCommand(nCmd, byte2);
    switch (nCmd) {
    case DevOn:
    case DevOff:
        status = priorityRequest::control;
        break;
    }
    emit appendCmd(cmd, 6, status);
}

bool DevNT19::isOn(int num) {
    return on[num];
}

char *DevNT19::createCommand(uchar nCmd, uchar nBloc) {
int i, sum = 0;
char *cmd = arr;
    cmd[0] = nCmd;
    cmd[1] = cmd[2] = 0;
    cmd[3] = interface;
    cmd[4] = nBloc;
    for (i = 0; i < 5; i++)
        sum += int(cmd[i]);
    cmd[5] = (unsigned char)((255 - sum) + 1);
    return cmd;
}
