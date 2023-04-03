#include "block.h"

#include <QDebug>

/**
 * @brief Block::Block - инициализация класса для работы с блоками
 * @param obj - (QGraphicsItem *)
 */
Block::Block(QGraphicsItem *obj) : Component(obj) {
    model = nullptr;
    resizeList(listOfComponents, 0);
}

/**
 * @brief Block::addComponent - добавить блок для пути
 * @param item - (Component *)
 */
void Block::addComponent(Component *item) {
    listOfComponents.append(item);
}

/**
 * @brief Block::setName - имя блока
 * @param name - (const QString &)
 */
void Block::setName(const QString &name) {
    this->name = name;
}

/**
 * @brief Block::getName - вернуть имя
 * @return - (QString)
 */
QString Block::getName() {
    return name;
}

/**
 * @brief Block::getCountComp - вернуть количество блоков
 * @return - (int)
 */
int Block::getCountComp() {
    return listOfComponents.size();
}

/**
 * @brief Block::getComponent - вернуть компонент по id
 * @return - (Component *)
 */
Component *Block::getComponent(int id) {
    for (Component* component : listOfComponents) {
        if (component->getID() == id) return component;
    }
    return nullptr;
}

/**
 * @brief Block::addPathToOtherCompFrom - вставить путь
 * @param path - (PathOfLines *)
 */
void Block::addPathToOtherCompFrom(PathOfLines *path) {
    if (getCountComp() == 1) {
        Component::addPathToOtherCompFrom(path);
    } else {
        for (int i = 0; i < this->getCountComp(); ++i) {
            int id = i + 1;
            Component* comp = this->getComponent(id);
            if (comp) comp->addPathToOtherCompFrom(path);
        }
    }
}

/**
 * @brief Block::appendSymbol - добавить символ
 * @param item - (QGraphicsSimpleTextItem *)
 */
void Block::appendSymbol(QGraphicsSimpleTextItem *item) {
    item->setParentItem(this);
    if (name != "CPU")
        item->setBrush(QColor(Qt::gray));
    symbols.append(item);
}

/**
 * @brief Block::getComponents - вернуть список компонентов для пути
 * @return - (QList<Component*>)
 */
QList<Component*> Block::getComponents() {
    return listOfComponents;
}

/**
 * @brief Block::setRect - настроить блок
 * @param x - (qreal)
 * @param y - (qreal)
 * @param width - (qreal)
 * @param height - (qreal)
 */
void Block::setRect(qreal x, qreal y, qreal width, qreal height) {
    QGraphicsRectItem::setRect(x, y, width, height);
}

/**
 * @brief Block::paint - обрисовывание блока
 * @param painter - (QPainter *)
 * @param option - (const QStyleOptionGraphicsItem *)
 * @param widget - (QWidget *)
 */
void Block::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)
    if (model != nullptr) {
        stateColor = model->getStateColor(1);
        QPen pen(QColor(stateColor.color));
        pen.setWidth(2);
        painter->setPen(pen);
        painter->setBrush(QBrush(stateColor.color));
        painter->drawRect(sceneBoundingRect());
        return;
    }
    QPen pen(QColor(Qt::gray));
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawRect(sceneBoundingRect());
}
