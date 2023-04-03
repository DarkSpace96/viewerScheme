#include "DevCam.h"
#include <cmath>

DevCam::DevCam(QString name, QObject *parent) : DevSensor(name, parent) {
    size = 5;
    data = new double[size];
    namePropertys = new QString[size];

    namePropertys[0] = "Влажность, %";
    namePropertys[1] = "Температура, ℃";
    namePropertys[2] = "Напряжение, В";
    namePropertys[3] = "Время, Ч";
    namePropertys[4] = "Пинг, мс";

    colorValues = new QColor[size];
    colorValuesText = new QColor[size];
    for (int i = 0; i < size; i ++) {
        colorValues[i] = Qt::white;
        colorValuesText[i] = Qt::black;
    }

    setSizeState();

    nzoom[0] = NZOOM00;
    nzoom[1] = NZOOM01;
    nzoom[2] = NZOOM02;
    nzoom[3] = NZOOM03;
    nzoom[4] = NZOOM04;
    nzoom[5] = NZOOM05;
    nzoom[6] = NZOOM06;
    nzoom[7] = NZOOM07;
    nzoom[8] = NZOOM08;
    nzoom[9] = NZOOM09;
}

DevCam::~DevCam(){}

void DevCam::command(uchar nCmd, uchar byte2, uchar byte3) {
char *cmd;
priorityRequest status;
    cmd = createCommand(nCmd, byte2, byte3);
    switch (nCmd) {
    case Diaphragm:
    //case newFocus:
    case Scale:
    case Place:
    case Course:
    case SetPlace:
    case SetCourse:
        status = priorityRequest::control;
        break;
    default:
        status = priorityRequest::query;
        break;
    }
    emit appendCmd(cmd, 6, status);
    switch (nCmd) {
    case 0:
        if (byte2 == 0x00 && byte3 == 0x00) {
            cmd = createCommand(GetFocus, byte2, byte3);
            emit appendCmd(cmd, 6, priorityRequest::query);
        }
        break;
    case Scale:
        if (byte2 == 0x00 && byte3 == 0x00) {
            cmd = createCommand(GetScale, byte2, byte3);
            emit appendCmd(cmd, 6, priorityRequest::query);
        }
        break;
    case Course:
        if (byte2 == 0x00 && byte3 == 0x00) {
            cmd = createCommand(GetCourse, byte2, byte3);
            emit appendCmd(cmd, 6, priorityRequest::query);
        }
        break;
    case Place:
        if (byte2 == 0x00 && byte3 == 0x00) {
            cmd = createCommand(GetPlace, byte2, byte3);
            emit appendCmd(cmd, 6, priorityRequest::query);
        }
        break;
    default:
        break;
    }
}

void DevCam::setColorItem(const propertyDate &property, const QColor &color, const int n) {
    if (property == DISK_SPACE)
        return;

    colorValues[property] = color;
}

Color DevCam::getStateColor(int n) {
Q_UNUSED(n);
QColor colorHumid = colorValues[0];
QColor colorTemp = colorValues[1];
QColor colorPing = colorValues[4];
Color colorState;
    if (colorHumid == Qt::red || colorTemp == Qt::red || colorPing == Qt::red) {
        colorState.color = QColor(Qt::red);
        colorState.raiting = GREEN;
        return colorState;
    } else if (colorHumid == Qt::yellow || colorTemp == Qt::yellow || colorPing == Qt::yellow) {
        colorState.color = QColor(Qt::yellow);
        colorState.raiting = GREEN;
        return colorState;
    }
    colorState.color = QColor(Qt::green);
    colorState.raiting = GREEN;
    return colorState;
}

QColor DevCam::getColorVolt(const double &volt) {
    if (volt == NON_VALUE)
        return QColor(Qt::white);

    if (volt >= 11 && volt <= 13)
        return QColor(Qt::green);
    else
        return QColor(Qt::white);
}

void DevCam::setOnOff(bool nOn){
    on = nOn;
    if (!on)
        bShow = false;
}


void DevCam::setBOnOff(bool *nOnOff) {
    bOnOff = nOnOff;
}

void DevCam::setValue(propertyValue property, int nValue) {
    if (valueMinMax[property][0] <= nValue &&
        valueMinMax[property][1] >= nValue) {
        switch (property) {
        case SCALE: {
            int i, ns, n = 0, min, max;
            getValueMinMax(SCALE, min, max);
            if (nValue > 600)
                value[property] = 0;
            else
                value[property] = nValue;

            if (value[property] > max) {
                ns = 100;
                goto end;
            }
            if (value[property] == min) {
                ns = 10;
                goto end;
            }
            ns = 100;
            for (i = 0; n == 0 && i < 9; i++)
                if (value[property] <= nzoom[i + 1]) {
                    ns = ((i + 1) * 10 + (10 * (value[property] - nzoom[i])) / (nzoom[i + 1] - nzoom[i]));
                    n = 1;
                }
end:
            scale = ns / 10.;
            break;
        }
        case FOCUS:
            if (nValue > 500)
                value[property] = 0;
            else
                value[property] = nValue;
            break;
        default:
            value[property] = nValue;
            break;
        }
    }
    qDebug() << "DevCam::setValue" << property << nValue << value[property];
    if (isControl()) {
        qDebug() << "DevCam::setValue2" << property << nValue;
        emit upDate();
    }
}

void DevCam::setAnglStart(propertyValue property, int value) {
    anglStart[property] = value;
}

void DevCam::setValueMinMax(propertyValue property, int min, int max) {
    valueMinMax[property][0] = min;
    valueMinMax[property][1] = max;
}

void DevCam::getValueMinMax(propertyValue property, int &min, int &max) {
    min = valueMinMax[property][0];
    max = valueMinMax[property][1];
}


void DevCam::setControl(bool nControl) {
    qDebug() << "DevCam::setControl" << getNumber() << nControl;
    bControl = nControl;
}

void DevCam::setNumber(int nNumber) {
    number = nNumber;
}

void DevCam::setViewingAngle(int nViewing) {
    viewingAngle = nViewing;
}

void DevCam::setData(const propertyDate property, const double nData) {
int nDataI = nData * 100;
    if (property == DISK_SPACE)
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
    if (property == VOLTAGE) {
        colorValues[2] = getColorVolt(data[property]);
    }
    emit upData();
}

void DevCam::analizCmd(const QByteArray *cmd) {
QString error = tr("Камера ") + getName() + ": ";
uchar Ncmd = cmd->at(0),
      byte_2 = cmd->at(1),
      byte_3 = cmd->at(2);
int zn;

    if (byte_2 >= 0xF0) {
        createAnErrorMessage(error, byte_2);
        return;
    }

    switch (Ncmd) {//проверка полученного ответа
    case GetScale:
        if (getNumber() >= BLe)
            break;
        setValue(SCALE, byte_2 * 256 + byte_3);
        qDebug() << "GetScale" << getValue(SCALE) << byte_2 << byte_3;
        break;
    case GetFocus:
        setValue(FOCUS, byte_2 * 256 + byte_3);
        qDebug() << "GetFocus" << getValue(FOCUS) << byte_2 << byte_3;
        break;
    case GetCourse:
        zn = 1;
        if (byte_2 >= 0x80) {
            byte_2 -= 0x80;
            zn = -1;
        }
        setValue(COURSE, zn * (byte_2 * 256 + byte_3));
        break;
    case GetPlace:
        zn = 1;
        if (byte_2 >= 0x80) {
            byte_2 -= 0x80;
            zn = -1;
        }
        setValue(PLACE, zn * (byte_2 * 256 + byte_3));
        break;
    case SetCourse:
        zn = 1;
        if (byte_2 >= 0x80) {
            byte_2 -= 0x80;
            zn = -1;
        }
        setValue(COURSE, zn * (byte_2 * 256 + byte_3));
        break;
    case SetPlace:
        zn = 1;
        if (byte_2 >= 0x80) {
            byte_2 -= 0x80;
            zn = -1;
        }
        setValue(PLACE, zn * (byte_2 * 256 + byte_3));
        break;
    case getCam:
        setData(TIME, ((uchar)cmd->at(1) << 8) + (uchar)cmd->at(2));
//        setDate(TEMPERATURE, ((((uchar)cmd->at(3) << 8) + (uchar)cmd->at(4)) / 65535.0) * 175 - 45);
        setData(TEMPERATURE, (((uchar)cmd->at(3) << 8) + (uchar)cmd->at(5)) * 0.04 - 40.1);
        setData(VOLTAGE, ((uchar)cmd->at(8) << 8) + (uchar)cmd->at(9));
//        setDate(HUMIDITY, ((((uchar)cmd->at(14) << 8) + (uchar)cmd->at(15)) / 65535.0) * 100);
        setData(HUMIDITY, 0.5872 * (uchar)cmd->at(14) - 0.00040845 * pow((uchar)cmd->at(14), 2) - 2.0468);

        if (!(getData(TEMPERATURE) <= 50 && getData(TEMPERATURE) >= -15)) {
            if (!bOnOff) {
                error += ": отключена из-за критической температуры";
                emit errorMessage(error);
                emit errorCamOff(getNumber());
//                camMenu->camOnPBClic(camPushButton[nCam]);
            }
        }
        if (getNumber() < BLe) {
            setValue(SCALE, ((uchar)cmd->at(5) << 8) + (uchar)cmd->at(6));
            qDebug() << "GetScale" << getValue(SCALE) << (uchar)cmd->at(5) << (uchar)cmd->at(6);
        }
        byte_2 = cmd->at(10);
        byte_3 = cmd->at(11);
        zn = 1;
        if (byte_2 >= 0x80) {
            byte_2 -= 0x80;
            zn = -1;
        }
        setValue(FOCUS, (byte_2 << 8) + byte_3);
        qDebug() << "GetScale" << getValue(FOCUS) << (uchar)cmd->at(10) << (uchar)cmd->at(11);

        byte_2 = cmd->at(16);
        byte_3 = cmd->at(17);
        zn = 1;
        if (byte_2 >= 0x80) {
            byte_2 -= 0x80;
            zn = -1;
        }
        setValue(COURSE, zn * (byte_2 * 256 + byte_3));

        zn = 1;
        byte_2 = cmd->at(18);
        byte_3 = cmd->at(19);
        if (byte_2 >= 0x80) {
            byte_2 -= 0x80;
            zn = -1;
        }
        setValue(PLACE, zn * (byte_2 * 256 + byte_3));
        break;
    }
    emit upDateValue(getNumber());
}

void DevCam::devOnOff(int num, bool bOn) {
    if (num != getNumber())
        return;
    if (bOn) {
        setStateDev(RUN);
        command(getCam, 0, 0);
    } else
        setStateDev(OFF);
}

QColor DevCam::getColorTemp(const double &temp) {
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

QColor DevCam::getColorHumid(const double &humid) {
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

int DevCam::getNumber() {
    return number;
}

int DevCam::getViewingAngle() {
    return viewingAngle;
}

int DevCam::getValue(propertyValue property) {
    return value[property];
}

int DevCam::getAnglStart(propertyValue property) {
    return anglStart[property];
}

bool DevCam::isControl() {
    return bControl;
}

double DevCam::getData(const propertyDate property) {
    return data[property];
}

float DevCam::getScale() {
    return scale;
}
