#ifndef BLOCK_H
#define BLOCK_H

#include "component.h"

#include <QList>
#include <QPen>
#include <QPainter>

template<class T>
void resizeList(QList<T> & list, int newSize) {
    int diff = newSize - list.size();
    T t;
    if (diff > 0) {
        list.reserve(diff);
        while (diff--)
            list.append(t);
    } else if (diff < 0)
        list.erase(list.end() + diff, list.end());
}

class Block : public Component
{
public:
    Block(QGraphicsItem *parent = nullptr);
    void addComponent(Component *item);
    void setName(const QString &name);
    void setPath(PathOfLines *path);
    void appendSymbol(QGraphicsSimpleTextItem *item);
    void setRect(qreal x, qreal y, qreal width, qreal height);
    virtual void addPathToOtherCompFrom(PathOfLines *path);
    int getCountComp();
    QString getName();
    Component *getComponent(int id);
    QList<Component*> getComponents();

    QList<Component*> listOfComponents;

private:
    QString name;
    QList<QGraphicsSimpleTextItem*> symbols;

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // BLOCK_H
