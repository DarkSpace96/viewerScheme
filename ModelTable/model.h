#ifndef MODEL_H
#define MODEL_H

#include <QAbstractTableModel>
#include "device.h"

class Model : public QAbstractTableModel
{
public:
    Model(const QString &name, const QString &nameModel, Devices *dev_);

public:
    void upData();
    void setHeader(const QString &first, const QString &second);
    void clearData();
    void upStateDev();
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    bool getCountRows();
    QVariant data(const QModelIndex &index, int role) const;
    QString getName();
    QString getNameModel();
    Color getStateColor(int);

private:
    int num = 0;
    bool state[2];
    struct valueItem {
        Qt::GlobalColor color;
        double value;
    };
    QString header[2], nameModel;
    STATE_DEV stateDev = OFF;
    Devices *dev;

public:
    void getStateColorCI(Color *color);
    void setStateCI(int n, bool b);
    void setColorItem(const propertyDate &property, const QColor &color);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // MODEL_H
