#include "pathoflines.h"

#include <QDebug>
#include <QPainter>

#include "component.h"

/**
* @brief PathOfLines::PathOfLines - инцилизация пути. Путь, показывает преодоления между компонентами.
*/
PathOfLines::PathOfLines() {
    widthOutLine = 3;
    colorOutLine = QColor(Qt::blue);
    widthLine = 3;
    stateColor.color = QColor(Qt::black);
    stateColor.raiting = GRAY;
    setZValue(stateColor.raiting);
    comp = nullptr;
}

/**
* @brief PathOfLines::setPoints - вставить точки, которые соединяются в прямые линии.
* @param items - (const QList<QPointF> &)
*/
void PathOfLines::setPoints(const QList<QPointF> &items) {
QPainterPath path;
    points = items;
    for (int i = 0; i < points.size(); ++i) {
        if (i == 0) {
            path.moveTo(points.at(i));
        } else {
            path.lineTo(points.at(i));
        }
    }
    QGraphicsPathItem::setPath(path);
    QGraphicsPathItem::setPen(QPen(stateColor.color, widthLine));
}

/**
* @brief PathOfLines::highlight - выделить
*/
void PathOfLines::highlight() {
    isHighlight = true;
    this->setZValue(stateColor.raiting);
}

/**
* @brief PathOfLines::offHighlight - отключить выделения
*/
void PathOfLines::offHighlight() {
    isHighlight = false;
    this->setZValue(stateColor.raiting);
    this->setPen(QPen(stateColor.color, widthLine));
    this->update();
}

/**
* @brief PathOfLines::setStateColor - изменить цвет компонента
* @param comp - (Component *)
* @param color - (Color)
*/
void PathOfLines::setStateColor(Component *comp, Color color) {
    this->comp = comp;
    stateColor = color;
    this->setZValue(color.raiting);
    this->setPen(QPen(stateColor.color, widthLine));
    this->update();
}

/**
* @brief PathOfLines::getStateColor - вернуть последний выделенный цвет элемента
* @return - (const Color &)
*/
const Color &PathOfLines::getStateColor() {
    return stateColor;
}

/**
* @brief PathOfLines::getComponentColor - венруть крайний элемент изменивший цвет
* @return - (Component *)
*/
Component *PathOfLines::getComponentColor() {
    return comp;
}

/**
* @brief PathOfLines::paint - обрабатывает логику нажатия
* @param painter - (QPainter *)
* @param option - (const QStyleOptionGraphicsItem *)
* @param widget - (QWidget *)
*/
void PathOfLines::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
QPainterPath path = QGraphicsPathItem::path();
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    if (isHighlight)
        painter->strokePath(path, QPen(colorOutLine, 2 * widthLine + widthOutLine));
    painter->restore();
    QGraphicsPathItem::paint(painter, option, widget);
}
