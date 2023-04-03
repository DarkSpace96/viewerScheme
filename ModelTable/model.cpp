#include "model.h"

#include <QBrush>
#include <QDebug>

Model::Model(const QString &name, const QString &nameModel, Devices *dev_) : dev(dev_) {
    dev->setNameMod(name);
    this->nameModel = "Параметры " + nameModel;
    state[0] = state[1] = false;
    connect(dev, &Devices::upState, this, &Model::upStateDev);
    connect(dev, &Devices::upData, this, &Model::upData);
}

int Model::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return dev->getCountRows();
}

int Model::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 2;
}

QVariant Model::data(const QModelIndex &index, int role) const {
int column = index.column();
int row = index.row();

    if (role == Qt::ForegroundRole) {
        if (column == 1)
            return QBrush(Qt::black);
    } else if (role == Qt::DisplayRole) {
        return dev->getDataMod(column, row);
    } else if (role == Qt::BackgroundRole) {
        return dev->getColor(column, row);
    }

    return QVariant();
}

QString Model::getName() {
    return dev->getNameMod();
}

QString Model::getNameModel() {
    return nameModel;
}

bool Model::getCountRows() {
int size = dev->getCountRows();
    if (size > 0)
        return false;
    else
        return true;

}

void Model::upData() {
    QModelIndex index = this->index(0, 0);
    QModelIndex index_ = this->index(dev->getCountRows(), 1);
    emit dataChanged(index, index_);
}

void Model::setColorItem(const propertyDate &property, const QColor &color) {
    dev->setColorItem(property, color);
    QModelIndex index = this->index(0, 0);
    QModelIndex index_ = this->index(dev->getCountRows(), 1);
    emit dataChanged(index, index_);
}

void Model::setHeader(const QString &first, const QString &second) {
    header[0] = first;
    header[1] = second;
}

void Model::clearData() {
    dev->clearData();
    QModelIndex index = this->index(0, 0);
    QModelIndex index_ = this->index(dev->getCountRows(), 1);
    emit dataChanged(index, index_);
}

void Model::upStateDev() {
    stateDev = dev->getStateDev(num);
    QModelIndex index = this->index(0, 0);
    QModelIndex index_ = this->index(dev->getCountRows(), 1);
    emit dataChanged(index, index_);
}

void Model::setStateCI(int n, bool b) {
    state[n] = b;
    QModelIndex index = this->index(0, 0);
    QModelIndex index_ = this->index(dev->getCountRows(), 1);
    emit dataChanged(index, index_);
}

Color Model::getStateColor(int type) {
Color color;
//    newStateDev();
    if (type) {
        if (stateDev == OFF) {
            color.raiting = GRAY;
            color.color = Qt::gray;
            return color;
        } else if (stateDev == NOSIGNAL) {
            color.raiting = YELLOW;
            color.color = Qt::yellow;
            return color;
        } else if (stateDev == ERROR) {
            color.raiting = RED;
            color.color = Qt::red;
            return color;
        }
    } else {
        if (stateDev == OFF || stateDev == NOSIGNAL || stateDev == ERROR) {
            color.raiting = GRAY;
            color.color = Qt::gray;
            return color;
        }
    }
    if (stateDev == RUN) {
        return dev->getStateColor();
    }
    return Color();
}

void Model::getStateColorCI(Color *color) {
//    newStateDev();
    if (stateDev == OFF) {
        color[0].raiting = GRAY;
        color[0].color = Qt::gray;

        if (state[1]) {
            color[2].raiting = GREEN;
            color[2].color = Qt::green;
        } else {
            color[2].raiting = GRAY;
            color[2].color = Qt::gray;
        }

    } else if (stateDev == NOSIGNAL) {
        color[0].raiting = YELLOW;
        color[0].color = Qt::yellow;
        if (state[1]) {
            color[2].raiting = GREEN;
            color[2].color = Qt::green;
        } else {
            color[2].raiting = RED;
            color[2].color = Qt::red;
        }
    } else if (stateDev == ERROR) {
        color[0].raiting = RED;
        color[0].color = Qt::red;
        if (state[1]) {
            color[2].raiting = GREEN;
            color[2].color = Qt::green;
        } else {
            color[2].raiting = RED;
            color[2].color = Qt::red;
        }
    } else if (stateDev == RUN) {
        color[0] = dev->getStateColor();

        if (state[1]) {
            color[2].raiting = GREEN;
            color[2].color = Qt::green;
        } else {
            color[2].raiting = RED;
            color[2].color = Qt::red;
        }
    }
    if (state[0]) {
        color[1].raiting = GREEN;
        color[1].color = Qt::green;
    } else {
        color[1].raiting = RED;
        color[1].color = Qt::red;
    }
}

Qt::ItemFlags Model::flags(const QModelIndex &index) const {
    Q_UNUSED(index)
    return Qt::ItemIsEnabled;

}


QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0) {
            return header[0];
        } else if (section == 1) {
            return header[1];
        }
    }
    return QVariant();
}
