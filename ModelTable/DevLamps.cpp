#include "DevLamps.h"

DevLamps::DevLamps(QString name, QObject *parent) : DevSensor(name, parent) {
    size = 4;
    data = new double[size];

    namePropertys = new QString[size];
    namePropertys[0] = "Влажность, %";
    namePropertys[1] = "Температура, ℃";
    namePropertys[2] = "Напряжение, В";
    namePropertys[3] = "Время, Ч";

    colorValues = new QColor[size];
    colorValues[0] = Qt::white;
    colorValues[1] = Qt::white;
    colorValues[2] = Qt::white;
    colorValues[3] = Qt::white;

    on = matrixOn[0] = matrixOn[1] = false;
    matrixValue[0] = matrixValue[1] = 25;
    setSizeState();
    setStateDev(OFF);
}

DevLamps::~DevLamps(){}

void DevLamps::command(uchar nCmd, uchar byte2, uchar byte3) {
char *cmd;
priorityRequest status;
    cmd = createCommand(nCmd + (getNpch() << 4), byte2, byte3);
    switch (nCmd) {
    case SetPSP:
        status = priorityRequest::control;
        break;
    case GetPSP:
        status = priorityRequest::query;
        break;
    default:
        status = priorityRequest::query;
        break;
    }
    emit appendCmd(cmd, 6, status);
}

void DevLamps::setOnOff(bool nOn, bool b) {
//int matr1, matr2;
    on = matrixOn[0] = matrixOn[1] = nOn;
//    if (on) {
//        setStateDev(RUN);
//        newMatrix(&matr1, &matr2);
//        if (polMatrix())
//            command(SetPSP, matr2, matr1);
//        else
//            command(SetPSP, matr1, matr2);
//    } else {
//        setStateDev(OFF);
//        command(SetPSP, 0, 0);
//    }
    if (b)
        emit upOnOff();
}


void DevLamps::setMatrixOnOff(int nMatrix, bool nOn){
//QByteArray cmd;
int matr1, matr2;

    matrixOn[nMatrix] = nOn;
    if (matrixOn[0] || matrixOn[1]) {
        on = true;
//        setStateDev(RUN);
    } else {
        on = false;
//        setStateDev(OFF);
    }
    emit upOnOff();

    newMatrix(&matr1, &matr2);

    if (on) {
        if (polMatrix())
            command(SetPSP, matr2, matr1);
        else
            command(SetPSP, matr1, matr2);
    }
}


void DevLamps::setMatrix(int value1, int value2, bool nOn) {
int mt, value[2];

        value[0] = value1;
        value[1] = value2;
//    if (polMatrix()) {
//        value[0] = value2;
//        value[1] = value1;
//    } else {
//        value[0] = value1;
//        value[1] = value2;
//    }

    for (mt = 0; mt < 2; mt++) {
        if (value[mt] != 0) {
            matrixValue[mt] = (value[mt] - 70) / 2 + 25;
            if (nOn) {
                matrixOn[mt] = true;
            }
        } else {
            if (nOn) {
                matrixOn[mt] = false;
            }
        }
    }
    if (nOn) {
        if (on != matrixOn[0] || matrixOn[1]) {
            on = matrixOn[0] || matrixOn[1];
            emit upOnOff();
        }
    }
    emit upDateValue();
    if (on)
        setStateDev(RUN);
    else
        setStateDev(OFF);
}

void DevLamps::setMatrix(int nMatrix, int nValue) {
int matr1, matr2;
    matrixValue[nMatrix] = nValue;
    emit upDateValue();

    newMatrix(&matr1, &matr2);
    if ((nValue % 5 > 0) && (nValue % 2 > 0))
        return;

    if (on) {
        if (polMatrix())
            command(SetPSP, matr2, matr1);
        else
            command(SetPSP, matr1, matr2);
    }
}

void DevLamps::setMatrixValue(int nMatrix, int nValue, Qt::KeyboardModifiers mod){
int matr1, matr2;

    if (mod != Qt::CTRL) {
        if (matrixOn[0])
            matrixValue[0] = nValue;
        if (matrixOn[1])
            matrixValue[1] = nValue;
    } else {
        if (matrixOn[nMatrix])
            matrixValue[nMatrix] = nValue;
    }
    emit upDateValue();

    newMatrix(&matr1, &matr2);

    if ((nValue % 5 > 0) && (nValue % 2 > 0))
        return;

    if (on) {
        if (polMatrix())
            command(SetPSP, matr2, matr1);
        else
            command(SetPSP, matr1, matr2);
    }
}

void DevLamps::newMatrix(int *matr1, int *matr2) {
    if (matrixOn[0])
        *matr1 = (matrixValue[0] - 25) * 2 + 70;
    else
        *matr1 = 0;
    if (matrixOn[1])
        *matr2 = (matrixValue[1] - 25) * 2 + 70;
    else
        *matr2 = 0;
}

QColor DevLamps::getColorTemp(const double &temp) {
    if (temp < -15 || temp >= 70) {
        return QColor(Qt::red);
    } else {
        if ((temp >= -15 && temp < -5) || (temp >=50 && temp < 70)) {
            return QColor(Qt::yellow);
        } else if (temp >= -5 && temp < 50) {
            return QColor(Qt::green);
        }
    }
    return QColor();
}

QColor DevLamps::getColorHumid(const double &humid) {
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

Color DevLamps::getStateColor(int n) {
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

void DevLamps::getMatrixValue(int *matr1, int *matr2){
    *matr1 = matrixValue[0];
    *matr2 = matrixValue[1];
}

void DevLamps::setGroup(int nGroup){
    group = nGroup;
}

void DevLamps::setNumber(int nNumber, int nNumberLamp){
    number = nNumber;
    numberLamp = nNumberLamp;
}

void DevLamps::setNpch(uchar Npch) {
    npch = Npch;
}

void DevLamps::setColorItem(const propertyDate &property, const QColor &color, const int) {
    if (property == PING || property == DISK_SPACE)
        return;

    int prop = property;
    if (prop == TIME)
        prop = 3;

    colorValues[prop] = color;
}

void DevLamps::analizCmd(const QByteArray *cmd){
uchar npch = (uchar)cmd->at(0) >> 4, Ncmd = (uchar)cmd->at(0) & 0x0F,
      nInterface = -1, nChannel = -1;
int value1 = -1, value2 = -1;
QString error;
    if (cmd->size() == 6) {
        nInterface = cmd->at(3);
        nChannel = cmd->at(4);
    } else if (cmd->size() == 12) {
        nInterface = cmd->at(9);
        nChannel = cmd->at(10);
    }

    if (getInterface() != nInterface || getChannel() != nChannel || getNpch() != npch) {
        return;
    }
    error = getName() + ": ";
    if ((uchar)cmd->at(1) >= 0xF0) {
        createAnErrorMessage(error, (uchar)cmd->at(1));
        return;
    }

    switch (Ncmd) {
    case SetPSP:
        value1 = (uchar)cmd->at(1);
        value2 = (uchar)cmd->at(2);
//        setMatrix(value1, value2, true);
        break;
    case GetPSP:
        setData(TEMPERATURE, ((((uchar)cmd->at(1) << 8) + (uchar)cmd->at(2)) / 65535.0) * 175 - 45);
        setData(HUMIDITY, ((((uchar)cmd->at(3) << 8) + (uchar)cmd->at(4)) / 65535.0) * 100);
        setData(TIME, ((uchar)cmd->at(5) << 8) + (uchar)cmd->at(6));
        setData(VOLTAGE, ((uchar)cmd->at(9) << 8) + (uchar)cmd->at(10));
        if (polMatrix()) {
            setMatrix((uchar)cmd->at(8), (uchar)cmd->at(7), true);
        } else {
            setMatrix((uchar)cmd->at(7), (uchar)cmd->at(8), true);
        }
//        setMatrix((uchar)cmd->at(7), (uchar)cmd->at(8), true);

        break;
    }
    emit upDate();
}

void DevLamps::setData(const propertyDate property, const double nData){
int nDataI = nData * 100;
    if (property == PING || property == DISK_SPACE)
        return;

    if (property == VOLTAGE)
        data[property] = nDataI / 100.;
    else
        data[property] = nDataI / 100.;

    if (property == HUMIDITY) {
        colorValues[0] = getColorHumid(nData);
    }
    if (property == TEMPERATURE) {
        colorValues[1] = getColorTemp(nData);
    }
    emit upData();
}

int DevLamps::getGroup(){
    return group;
}

int DevLamps::getNumber(){
    return number;
}

int DevLamps::getNumberLamp(){
    return numberLamp;
}

uchar DevLamps::getNpch() {
    return npch;
}

int DevLamps::getMatrixValue(int matrix, bool b){
    if (!b)
        return matrixValue[matrix];
    else
        return (matrixValue[matrix] - 25) * 2 + 70;
}

bool DevLamps::isMatrixOn(int matrix){
    return matrixOn[matrix];
}

bool DevLamps::isOn() {
    return matrixOn[0] || matrixOn[1];
}

bool DevLamps::polMatrix() {
bool b = false;
    if ((group != 3 || group != 5) && number < 2)
        b = true;
    return b;
}

double DevLamps::getData(const propertyDate property){
    if (property == PING || property == DISK_SPACE)
        return NON_VALUE;

    int prop = property;
    if (prop == TIME)
        prop = 3;
    return data[prop];
}

void DevLamps::devOnOff(int num, bool bOn) {
int matr1, matr2;
    if (num != getNumberLamp())
        return;
    if (bOn) {
        setStateDev(RUN);
        newMatrix(&matr1, &matr2);
        if (polMatrix())
            command(SetPSP, matr2, matr1);
        else
            command(SetPSP, matr1, matr2);
        command(GetPSP, 0, 0);
    } else {
        setStateDev(OFF);
//        command(SetPSP, 0, 0);
    }

//    setOnOff(bOn);
//    if (bOn)
//        command(GetPSP, 0, 0);
}
