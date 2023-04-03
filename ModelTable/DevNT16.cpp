#include "DevNT16.h"

DevNT16::DevNT16(QString name, QObject *parent) : DevOnOff(name, parent) {
    for (int i = 0; i < 3; ++i) {
        on[i] = true;
    }
    setInterfaceCanal(0xE3);
}

DevNT16::~DevNT16() { }

void DevNT16::analizCmd(QByteArray *cmd) {
QString error = tr("НТ-16: ");
uchar Ncmd = cmd->at(0),
      byte_2 = cmd->at(1),
      byte_5 = cmd->at(4);
    if (byte_2 >= 0xF0) {
        createAnErrorMessage(error, byte_2);
        return;
    }
    switch (Ncmd) {
    case SetNT16:
        uchar nBit = 0x01;
        bool newOn[16];

        for (int i = 0; i < 16; ++i) {
            newOn[i] = byte_5 & nBit;
            byte_5 = byte_5 >> 1;
        }

        for (int i = 0; i < 16; ++i) {
            if (newOn[i] != isOn(i))
                on[i] = newOn[i];
        }
        break;
    }
}

void DevNT16::command(uchar nCmd, uchar byte2, uchar byte3) {
char *cmd;
priorityRequest status;
    cmd = createCommand(nCmd, byte2, byte3);
    switch (nCmd) {
    case SetNT16:
        status = priorityRequest::control;
        break;
    }
    emit appendCmd(cmd, 6, status);
}

bool DevNT16::isOn(int num) {
    return on[num];
}
