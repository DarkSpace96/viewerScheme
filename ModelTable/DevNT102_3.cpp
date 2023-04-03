#include "DevNT102_3.h"

DevNT102_3::DevNT102_3(QString name, QObject *parent) : DevSensor(name, parent) {
    size = 3;
    data = new double[size];

    namePropertys = new QString[size];
    namePropertys[0] = "Влажность, %";
    namePropertys[1] = "Температура, ℃";
    namePropertys[2] = "Время, Ч";

    colorValues = new QColor[size];
    colorValues[0] = Qt::white;
    colorValues[1] = Qt::white;
    colorValues[2] = Qt::white;

    setSizeState();
}

DevNT102_3::~DevNT102_3(){}

void DevNT102_3::command(uchar nCmd, uchar byte2, uchar byte3) {
char *cmd;
    cmd = createCommand(nCmd + 0x90, byte2, byte3);
    emit appendCmd(cmd, 6, priorityRequest::query);
}

void DevNT102_3::analizCmd(const QByteArray *cmd){
uchar nChannel = -1, nCmd = (uchar)cmd->at(0) & 0x0F;
QString error;
    if (cmd->size() == 6) {
        nChannel = cmd->at(4);
    } else if (cmd->size() == 12) {
        nChannel = cmd->at(10);
    }
    if (getChannel() != nChannel)
        return;

    error = getName() +  ": ";
    if ((uchar)cmd->at(1) > 0xF0) {
        createAnErrorMessage(error, (uchar)cmd->at(1));
        return;
    }

    switch (nCmd) {
    case GetNT102:
        setData(TEMPERATURE, ((uchar)cmd->at(1) << 8) + (uchar)cmd->at(2));
        setData(HUMIDITY, ((uchar)cmd->at(3) << 8) + (uchar)cmd->at(4));
        setData(TIME, ((uchar)cmd->at(5) << 8) + (uchar)cmd->at(6));
        break;
    default:
        break;
    }
}

void DevNT102_3::setData(const propertyDate property, const double nDate){
    if (property == VOLTAGE || property == PING || property == DISK_SPACE)
        return;

    int prop = property;
    if (prop == TIME)
        prop = 2;

    int dateI = nDate * 100;
    data[prop] = dateI / 100.;

    if (property == HUMIDITY) {
       colorValues[0] = getColorHumid(nDate);
    }

    if (property == TEMPERATURE) {
       colorValues[1] = getColorTemp(nDate);
    }
    emit upData();
}

double DevNT102_3::getData(const propertyDate property){
    if (property == VOLTAGE || property == PING || property == DISK_SPACE)
        return NON_VALUE;

    int prop = property;
    if (prop == TIME)
        prop = 2;

    return data[prop];
}

void DevNT102_3::devOnOff(int num, bool bOn) {
    if (num != getNumber())
        return;
    if (bOn) {
        setStateDev(RUN);
        command(GetNT102, 0, 0);
    } else
        setStateDev(OFF);
}

QColor DevNT102_3::getColorTemp(const double &temp) {
    if (temp == NON_VALUE)
        return QColor(Qt::white);

    if (temp < -15 || temp >= 50) {
        return QColor(Qt::red);
    } else {
        if ((temp >= -15 && temp < -5) || (temp >=45 && temp < 50)) {
            return QColor(Qt::yellow);
        } else if (temp >= -5 && temp < 45) {
            return QColor(Qt::green);
        }
    }
    return QColor();
}

QColor DevNT102_3::getColorHumid(const double &humid) {
    if (humid == NON_VALUE)
        return QColor(Qt::white);

    if (humid >= 99) {
        return QColor(Qt::red);
    } else if (humid >= 70 && humid < 99) {
        return QColor(Qt::yellow);
    } else if (humid < 70) {
        return QColor(Qt::green);
    }
    return QColor();
}

void DevNT102_3::setNumber(int nNumber) {
    number = nNumber;
}

int DevNT102_3::getNumber() {
    return number;
}

void DevNT102_3::setColorItem(const propertyDate &property, const QColor &color, int) {
    if (property == VOLTAGE || property == PING || property == DISK_SPACE)
        return;

    int prop = property;
    if (prop == TIME)
        prop = 2;

    colorValues[prop] = color;
}

Color DevNT102_3::getStateColor(int) {
QColor colorHumid = colorValues[0];
QColor colorTemp = colorValues[1];
Color colorState;
    if (colorHumid == Qt::red || colorTemp == Qt::red) {
        colorState.color = QColor(Qt::red);
        colorState.raiting = GREEN;
        return colorState;
    } else if (colorHumid == Qt::yellow || colorTemp == Qt::yellow ) {
        colorState.color = QColor(Qt::yellow);
        colorState.raiting = GREEN;
        return colorState;
    }
    colorState.color = QColor(Qt::green);
    colorState.raiting = GREEN;
    return colorState;
}
