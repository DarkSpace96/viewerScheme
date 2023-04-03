#ifndef COMPONENT_H
#define COMPONENT_H

#include "pathoflines.h"

#include <ModelTable/model.h>


class Component : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Component(QGraphicsItem *parent = nullptr);
    void setID(int id);
    void setRect(qreal x, qreal y, qreal width, qreal height);
    void appendSymbol(QGraphicsSimpleTextItem *item);
    void highlightPaths();
    void offHighlightPaths();
    void setModel(Model *model);
    virtual void addPathToOtherCompFrom(PathOfLines *path);
    virtual void addPathToOtherCompTo(PathOfLines *path);
    int getID();

private slots:
    void udpateColor();
    void udpateColorCI();
    void udpateColorNT102();

protected:
    Model *model;
    Color stateColor;

private:
    int id;
    QList<PathOfLines*> pathCompFrom, pathCompTo;
    QList<QGraphicsSimpleTextItem*> symbols;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // COMPONENT_H
