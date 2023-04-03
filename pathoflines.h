#ifndef PathOfLines_H
#define PathOfLines_H

#include <QGraphicsRectItem>

#include "ModelTable/device.h"

class Component;

class PathOfLines : public QGraphicsPathItem
{
public:
    QString atr;

    PathOfLines();
    void setPoints(const QList<QPointF> &items);
    void highlight();
    void offHighlight();
    void setStateColor(Component* comp, Color color);
    Component *getComponentColor();
    const Color& getStateColor();

private:
    int widthOutLine, widthLine;
    bool isHighlight = false;
    QList<QPointF> points;
    QColor colorOutLine;
    QColor colorLine;
    Color stateColor;
    Component *comp;

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // PathOfLines_H
