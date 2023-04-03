#include "DevClient.h"

DevClient::DevClient(QString name, int nCln, QObject *parent) : DevOnOff(name, parent) {
    modScreen = new int[4];
    nClient = nCln;
    for (int i = 0; i < 4; ++i)
        modScreen[i] = -1;
    setInterfaceCanal(0xE4, 0);
}

DevClient::~DevClient(){
//    delete date;
}

void DevClient::setMod(int nMod) {
    mod = nMod;
    updateMenu();
    emit upDateMod();
}

void DevClient::setModScreen(QVector<Devices *> dev, int numScr, int newCam) {
int select = -1, nCam, i;
//char *cmd;
//priorityRequest status = priorityRequest::control;
    nCam = modScreen[numScr];
    if (newCam >= CAM1) {
        for (i = 0; i < NCAM; ++i) {
            if (modScreen[i] == newCam) {
                select = i;
                break;
            }
        }
    }
    modScreen[numScr] = newCam;
    if (modScreen[numScr] >= Lor1)
        emit commandNT16(SetNT16, 0, numScr + 1 + (nClient * 4));
//        cmd = createCommand(SetNT16, 0, numScr + 1 + (nClient * 4));
    else
        emit commandNT16(SetNT16, modScreen[numScr] + 1, numScr + 1 + (nClient * 4));
//        cmd = createCommand(SetNT16, modScreen[numScr] + 1, numScr + 1 + (nClient * 4));
//    emit appendCmd(cmd, 6, status);
    if (newCam >= CAM1) {
        dev[newCam]->setShow(true);
    } else {
        if (nCam >= CAM1)
        dev[nCam]->setShow(false);
    }
    if (select >= 0) {
        if (nCam >= CAM1) {
            dev[nCam]->setShow(true);
            modScreen[select] = nCam;
        } else {
            modScreen[select] = -1;
        }
        emit commandNT16(SetNT16, modScreen[select] + 1, select + 1 + (nClient * 4));
//        cmd = createCommand(SetNT16, modScreen[select] + 1, select + 1 + (nClient * 4));
//        emit appendCmd(cmd, 6, status);
    }
    updateMenu();
}

void DevClient::setModScreen(int *nModScreen) {
//char *cmd;
//priorityRequest status = priorityRequest::control;
    *modScreen = *nModScreen;
    for (int i = 0; i < 4; ++i) {
        emit commandNT16(SetNT16, modScreen[i] + 1, i + 1 + (nClient * 4));
//        cmd = createCommand(SetNT16, modScreen[i] + 1, i + 1 + (nClient * 4));
//        emit appendCmd(cmd, 6, status);
    }
    updateMenu();
}

void DevClient::analizCmd(QByteArray *cmd) {
QString error = tr("НТ-16: ");
uchar Ncmd = cmd->at(0),
      byte_2 = cmd->at(1);
    if (byte_2 >= 0xF0) {
        createAnErrorMessage(error, byte_2);
        return;
    }
}

void DevClient::setOnOff(bool nOn) {
    on = nOn;
}

void DevClient::getModScreen(int *scr1, int *scr2, int *scr3, int *scr4) {
    *scr1 = modScreen[0];
    *scr2 = modScreen[1];
    *scr3 = modScreen[2];
    *scr4 = modScreen[3];
}

void DevClient::setArrDate(int *arr) {
    arrDate = arr;
}

void DevClient::setImage(QImage *image) {
    this->image = image;
}

int DevClient::getMod() {
    return mod;
}

int DevClient::getModScreen(int numScr) {
    return modScreen[numScr];
}

void DevClient::updateArr() {
int i, sum = 0;
char *cmd;
    cmd = new char[23];
    cmd[0] = 0xE4;
    cmd[1] = 0x02;
    for (i = 0; i < 20; ++i) {
        cmd[2 + i] = arrDate[i];
    }
    for (i = 0; i < 23; i++)
        sum += int(cmd[i]);
    cmd[22] = (unsigned char)((255 - sum) + 1);
}

void DevClient::updateMenu() {
int i, sum = 0;
char *cmd;
    cmd = new char[7];
    cmd[0] = 0x04 + 0x11 * (nClient - 1);
    cmd[1] = 0x01;
    cmd[2] = (mod = 0) ? 0x00 : 0x04;
    cmd[3] = (modScreen[1] << 4) + modScreen[0];
    cmd[4] = (modScreen[4] << 4) + modScreen[3];
    for (i = 0; i < 8; ++i) {
        cmd[5] += menu[i] << i;
    }
    for (i = 0; i < 7; i++)
        sum += int(cmd[i]);
    cmd[6] = (unsigned char)((255 - sum) + 1);
}


