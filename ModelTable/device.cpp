#include <QDebug>
#include <QList>

#include "viewerScheme/ModelTable/device.h"

#include "model.h"

void Device::setName(const QString &str) {
    name = str;
}

QString Device::getName() {
    return name;
}

QVariant Device::getData(int column, int row) {
    if (column < 0 || column > 1 || row < 0 || row > size -1)
        return QVariant();

    if (column == 0) {
        return QVariant(namePropertys[row]);
    } else {
        double value = values[row];
        if (value == NON_VALUE) return QVariant("---");
        return value;
    }
}

const int &Device::getCountRows() {
    return size;
}

void Device::clearData() {
    for (int i = 0; i < size; ++i) {
        values[i] = NON_VALUE;
    }

    for (int i = 0; i < size; ++i) {
        colorValues[i] = QColor(Qt::white);
    }
}

QVariant Device::getColor(const int &column, const int &row) {
    if (column < 0 || column > 1 || row < 0 || row > size -1)
        return QVariant();

    if (column == 0) {
        return QVariant();
    } else {
        return QVariant(colorValues[row]);
    }
}


devCI::devCI() {
    size = 5;
    values = new double[size];
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
}

void devCI::setValue(const propertyDevice &property, const double &value) {
int valuesI = value * 100;
    if (property == DISK_SPACE)
        return;
    if (property == VOLTAGE)
        values[property] = valuesI / 10000.;
    else
        values[property] = valuesI / 100.;

    if (property == HUMIDITY) {
       colorValues[0] = getColorHumid(value);
    }
    if (property == TEMPERATURE) {
       colorValues[1] = getColorTemp(value);
    }
    if (property == VOLTAGE) {
       colorValues[2] = getColorVolt(values[property]);
    }
}

void devCI::setColorItem(const propertyDevice &property, const QColor &color) {
    if (property == DISK_SPACE)
        return;

    colorValues[property] = color;
}

Color devCI::getStateColor() {
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

QColor devCI::getColorTemp(const double &temp) {
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

QColor devCI::getColorHumid(const double &humid) {
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

QColor devCI::getColorVolt(const double &volt) {
    if (volt == NON_VALUE)
        return QColor(Qt::white);

    if (volt >= 11 && volt <= 13)
        return QColor(Qt::green);
    else
        return QColor(Qt::white);
}

devHT_102_2::devHT_102_2() {
    size = 3;
    values = new double[size];

    namePropertys = new QString[size];
    namePropertys[0] = "Влажность, %";
    namePropertys[1] = "Температура, ℃";
    namePropertys[2] = "Время, Ч";

    colorValues = new QColor[size];
    colorValues[0] = Qt::white;
    colorValues[1] = Qt::white;
    colorValues[2] = Qt::white;
}

void devHT_102_2::setValue(const propertyDevice &property, const double &value) {
    if (property == VOLTAGE || property == PING || property == DISK_SPACE)
        return;

    int prop = property;
    if (prop == TIME)
        prop = 2;

    int valuesI = value * 100;
    values[prop] = valuesI / 100.;

    if (property == HUMIDITY) {
       colorValues[0] = getColorHumid(value);
    }

    if (property == TEMPERATURE) {
       colorValues[1] = getColorTemp(value);
    }
}

Color devHT_102_2::getStateColor() {
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

QColor devHT_102_2::getColorTemp(const double &temp) {
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

QColor devHT_102_2::getColorHumid(const double &humid) {
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

devLight::devLight() {
    size = 3;
    values = new double[size];

    namePropertys = new QString[size];
    namePropertys[0] = "Влажность, %";
    namePropertys[1] = "Температура, ℃";
    namePropertys[2] = "Время, Ч";

    colorValues = new QColor[size];
    colorValues[0] = Qt::white;
    colorValues[1] = Qt::white;
    colorValues[2] = Qt::white;
}

void devLight::setValue(const propertyDevice &property, const double &value) {
    if (property == VOLTAGE || property == PING || property == DISK_SPACE)
        return;

    int prop = property;
    if (prop == TIME)
        prop = 2;

    int valuesI = value * 100;
    values[prop] = valuesI / 100.;

    if (property == HUMIDITY) {
       colorValues[0] = getColorHumid(value);
    }

    if (property == TEMPERATURE) {
       colorValues[1] = getColorTemp(value);
    }
}

void devLight::setColorItem(const propertyDevice &property, const QColor &color) {
    if (property == VOLTAGE || property == PING || property == DISK_SPACE)
        return;

    int prop = property;
    if (prop == TIME)
        prop = 2;

    colorValues[prop] = color;
}

void devHT_102_2::setColorItem(const propertyDevice &property, const QColor &color) {
    if (property == VOLTAGE || property == PING || property == DISK_SPACE)
        return;

    int prop = property;
    if (prop == TIME)
        prop = 2;

    colorValues[prop] = color;
}


QColor devLight::getColorTemp(const double &temp) {
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

QColor devLight::getColorHumid(const double &humid) {
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

Color devLight::getStateColor() {
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

devSwitch::devSwitch() {
    size = 1;
    values = new double[size];

    namePropertys = new QString[size];
    namePropertys[0] = "Пинг, мс";

    colorValues = new QColor[size];
    colorValues[0] = Qt::white;

}

void devSwitch::setValue(const propertyDevice &property, const double &value) {
    if (property != PING)
        return;

    int valuesI = value * 100;
    values[0] = valuesI / 100.;
}

void devSwitch::setColorItem(const propertyDevice &property, const QColor &color) {
    if (property != PING)
        return;

    colorValues[0] = color;
}

Color devSwitch::getStateColor() {
Color colorState;
QColor colorPing = colorValues[0];
    if (colorPing == Qt::red ) {
        colorState.color = QColor(Qt::red);
        colorState.raiting = GREEN;//RED;
        return colorState;
    }
    colorState.color = QColor(Qt::green);
    colorState.raiting = GREEN;
    return colorState;
}

QColor devSwitch::getColorTemp(const double &temp) {
    Q_UNUSED(temp)
    return QColor();
}

devClient::devClient() {
    size = 2;
    values = new double[size];

    namePropertys = new QString[size];
    namePropertys[0] = "Пинг, мс";
    namePropertys[1] = "Температура, ℃";

    colorValues = new QColor[size];
    colorValues[0] = Qt::white;
    colorValues[1] = Qt::white;
}

void devClient::setValue(const propertyDevice &property, const double &value) {
    if (property != TEMPERATURE && property != PING)
        return;

    if (property == PING) {
        int valuesI = value * 100;
        values[0] = valuesI / 100.;
    }

    if (property == TEMPERATURE) {
        int valuesI = value * 100;
        values[1] = valuesI / 100.;

        colorValues[1] = getColorTemp(value);
    }
}

QColor devClient::getColorTemp(const double &temp) {
    if (temp == NON_VALUE)
        return QColor(Qt::white);

    if (temp >= 91) {
        return QColor(Qt::red);
    } else if (temp >= 64 && temp < 91) {
        return QColor(Qt::yellow);
    } else if (temp < 64) {
        return QColor(Qt::green);
    }
    return QColor();
}

void devClient::setColorItem(const propertyDevice &property, const QColor &color) {
    if (property != PING || property != TEMPERATURE)
        return;
    if (property == PING)
        colorValues[0] = color;
    else
        colorValues[1] = color;
}

Color devClient::getStateColor() {
QColor colorPing = colorValues[0];
QColor colorTemp = colorValues[1];
Color colorState;
    if (colorPing == Qt::gray) {
        colorState.color = QColor(Qt::gray);
        colorState.raiting = GREEN;
        return colorState;
    }
    if (colorPing == QColor(Qt::green) && (colorTemp == QColor(Qt::red) || colorTemp == QColor(Qt::yellow)))
        colorState.color = colorTemp;
    else
        colorState.color = QColor(Qt::green);
    colorState.raiting = GREEN;
    return colorState;
}

devDisk::devDisk() {
    size = 1;
    values = new double[size];

    namePropertys = new QString[size];
    namePropertys[0] = "Дисковое пронстранство, %";

    colorValues = new QColor[size];
    colorValues[0] = Qt::white;
}

void devDisk::setValue(const propertyDevice &property, const double &value) {
    if (property != DISK_SPACE)
        return;

    int valuesI = value * 100;
    values[0] = valuesI / 100.;
}

void devDisk::setColorItem(const propertyDevice &property, const QColor &color) {
    if (property != DISK_SPACE)
        return;

    colorValues[0] = color;
}

Color devDisk::getStateColor() {
QColor colorSpace = colorValues[0];
Color colorState;
    if (colorSpace == Qt::red) {
        colorState.color = QColor(Qt::red);
        colorState.raiting = GREEN;;
        return colorState;
    }
    colorState.color = QColor(Qt::green);
    colorState.raiting = GREEN;
    return colorState;
}

QColor devDisk::getColorTemp(const double &temp) {
    Q_UNUSED(temp)
    return QColor();
}

devCPU::devCPU() {
    size = 2;
    values = new double[size];

    namePropertys = new QString[size];
    namePropertys[0] = "Время работы комплекса, Ч";
    namePropertys[1] = "Температура, ℃";

    colorValues = new QColor[size];
    colorValues[0] = Qt::white;
    colorValues[1] = Qt::white;
}

void devCPU::setValue(const propertyDevice &property, const double &value) {
    if (property != TEMPERATURE && property != TIME)
        return;

    int prop = property;
    if (prop == TIME)
        prop = 0;

    int valuesI = value * 100;
    values[prop] = valuesI / 100.;

    if (property == TEMPERATURE) {
       colorValues[1] = getColorTemp(value);
    }
}

QColor devCPU::getColorTemp(const double &temp) {
    if (temp == NON_VALUE)
        return QColor(Qt::white);

    if (temp >= 91) {
        return QColor(Qt::red);
    } else if (temp >= 64 && temp < 91) {
        return QColor(Qt::yellow);
    } else if (temp < 64) {
        return QColor(Qt::green);
    }
    return QColor();
}

void devCPU::setColorItem(const propertyDevice &property, const QColor &color) {
    if (property != DISK_SPACE)
        return;

    int prop = property;

    colorValues[prop] = color;
}

Color devCPU::getStateColor(){
QColor colorTemp = colorValues[1];
Color colorState;
    if (colorTemp == Qt::red) {
        colorState.color = QColor(Qt::red);
        colorState.raiting = GREEN;
        return colorState;
    } else if (colorTemp == Qt::yellow ) {
        colorState.color = QColor(Qt::yellow);
        colorState.raiting = GREEN;
        return colorState;
    }
    colorState.color = QColor(Qt::green);
    colorState.raiting = GREEN;
    return colorState;
}

QColor devCPU::getColorHumid(const double &humid) {
    Q_UNUSED(humid)
    return QColor();
}

devSelect::devSelect() {
    size = 0;
    colorValues = new QColor[size];
    colorValues[0] = Qt::green;
}

void devSelect::setColorItem(const propertyDevice &property, const QColor &color) {
    if (property != LOG)
        return;
    colorValues[0] = color;
}

Color devSelect::getStateColor() {
Color colorState;
    colorState.color = colorValues[0];
    colorState.raiting = GREEN;
    return colorState;
}
