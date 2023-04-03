#include "Devices.h"
#include "logger.h"

/**
 * @brief Devices::Devices - инициализация объекта Devices
 * @param parent - (QWidget *)
 */
Devices::Devices(QString name, QObject *parent) :
    QObject(parent), logger(new Logger(name))
{
    setObjectName(name);
//    logger = new Logger(name);
}

Devices::~Devices(){
}

/**
 * @brief Devices::setName - установка имени
 * @param nName - имя (QString)
 */
void Devices::setName(QString nName) {
    name = nName;
}

void Devices::setNameMod(QString nName) {
    nameMod = nName;
}

/**
 * @brief Devices::setInterfaceCanal - истановка интерфейса и каналла для общения с устройством
 * @param nInterface
 * @param nChannel
 */
void Devices::setInterfaceCanal(uchar nInterface, uchar nChannel){
    interface = nInterface;
    channel = nChannel;
}

/**
 * @brief Devices::setOnOff - вкл/выкл устройства
 * @param nOn - вкл/выкл: true/false (bool)
 */
void Devices::setOnOff(bool nOn) {
    on = nOn;
    if (on)
        setStateDev(RUN);
    else
        setStateDev(OFF);
}

/**
 * @brief Devices::setStateDev - установка статуса устройства
 * @param state - статус (STATE_DEV)
 */
void Devices::setStateDev(const STATE_DEV &state, const int n) {
    stateDev[n] = state;
    logger->accept(QString("Devices::setStateDev: param: state=%1").arg(state));
    emit upState();
}

void Devices::setSizeState(int n) {
    sizeState = n;
    stateDev = new STATE_DEV[sizeState];
}

//void Devices::setSenCmds(SendCmds *nSendCmd) {
//    sendCmd = nSendCmd;
//}

void Devices::createAnErrorMessage(QString startMessage, uchar nError) {
    error = startMessage;
    switch (nError) { // проверка на ошибки
    case 0xF1:
        error += tr("Ошибка в принятой команде(неверные данные)");
        break;
    case 0xF2:
        error += tr("Ошибка при передаче по ИРПС");
        setStateDev(ERROR);
        break;
    case 0xF3:
        error += tr("Ошибка при передаче от НТ-208-1 к НТ-203");

//        error += tr("Ошибка при передаче от НТ-102-2 к НТ-203");

        setStateDev(ERROR);
        break;
    case 0xF4:
        error += tr("Нет ответа");

//        error += tr("Нет ответа от НТ-102-2");
        setStateDev(NOSIGNAL);
        break;
    case 0xF6:
        error += tr("Нет ответа от НТ-102-2!!!!");
        break;
    default:
        break;
    }
    emit errorMessage(error);
}

QVariant Devices::getColor(const int &column, const int &row) {
    if (column < 0 || column > 1 || row < 0 || row > size -1)
        return QVariant();

    if (column == 0) {
        return QVariant();
    } else {
        return QVariant(colorValues[row]);
    }
}

void Devices::setShow(bool nShow) {
    bShow = nShow;
}

void Devices::setSignal(bool nSignal) {
    signal = nSignal;
}

void Devices::clearData() {
    for (int i = 0; i < size; ++i) {
        data[i] = NON_VALUE;
    }
}

char *Devices::createCommand(uchar nCmd, uchar byte2, uchar byte3) {
int i, sum = 0;
char *cmd = arr;
    cmd[0] = nCmd;
    cmd[1] = byte2;
    cmd[2] = byte3;
    cmd[3] = interface;
    cmd[4] = channel;
    for (i = 0; i < 5; i++)
        sum += int(cmd[i]);
    cmd[5] = (unsigned char)((255 - sum) + 1);
    return cmd;
}

const int &Devices::getCountRows() {
    return size;
}


bool Devices::isShow() {
    return bShow;
}

bool Devices::isSignal() {
    return signal;
}

uchar Devices::getChannel() {
    return channel;
}

uchar Devices::getInterface(){
    return interface;
}

bool Devices::isOn(){
    return on;
}

QString Devices::getName(){
    return name;
}

QString Devices::getNameMod() {
    return nameMod;
}

QVariant Devices::getDataMod(int column, int row) {
    if (column < 0 || column > 1 || row < 0 || row > size -1)
        return QVariant();

    if (column == 0) {
        qDebug() << "getDataMod" << namePropertys[row];
        return QVariant(namePropertys[row]);
    } else {
        double value = data[row];
        if (value == NON_VALUE) return QVariant("---");
        return value;
    }
}

STATE_DEV Devices::getStateDev(const int n) {
    return stateDev[n];
}

