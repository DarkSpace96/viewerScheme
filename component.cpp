#include "component.h"
#include "graphicsscene.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

/**
* @brief Component::Component - конструктор компонента
* @param parent - графический родитель (QGraphicsItem *)
*/
Component::Component(QGraphicsItem *parent) : QGraphicsRectItem(parent) {
    model = nullptr;
    stateColor.raiting = GRAY;
}

/**
* @brief Component::setID - вставка индентификатора
* @param id - индентификатор (int)
*/
void Component::setID(int id) {
    this->id = id;
}

/**
* @brief Component::addPathToOtherCompFrom - вставка пути от компонента, по которму при нажатии на компонент будет видется путь
* @param path - путь (PathOfLines *)
*/
void Component::addPathToOtherCompFrom(PathOfLines *path) {
    pathCompFrom.append(path);
}

/**
* @brief Component::addPathToOtherCompFrom - вставка пути к компоненту, по которму при нажатии на компонент будет видется путь
* @param path - путь (PathOfLines *)
*/
void Component::addPathToOtherCompTo(PathOfLines *path) {
    pathCompTo.append(path);
}

/**
* @brief Component::getID - вернуть id
* @return - id (int)
*/
int Component::getID() {
    return id;
}

/**
* @brief Component::setModel - модель, которая отображает логику компонента в графическом представлении
* @param model - ссылка на модель (Model *)
*/
void Component::setModel(Model *model) {
    this->model = model;
    if (this->model->getName().contains("Cam_"))
        QObject::connect(model, &Model::dataChanged, this, &Component::udpateColorCI);
    else if (this->model->getName().contains("HT-102-2_"))
        QObject::connect(model, &Model::dataChanged, this, &Component::udpateColorNT102);
    else
        QObject::connect(model, &Model::dataChanged, this, &Component::udpateColor);
}

/**
* @brief Component::udpateColorCI - Обновить цвет, берется из модели
*/
void Component::udpateColorCI() {
int n = -1;
    stateColor = model->getStateColor(1);
    setBrush(QBrush(stateColor.color));

    Color stateColorCI[3];
    model->getStateColorCI(stateColorCI);

    int size = pathCompTo.size();
    for (int i = 0; i < size; ++i) {
        PathOfLines* path = pathCompTo[i];
        Color pathStateColor = path->getStateColor();
        Component* gettingComp = path->getComponentColor();
        if (path->atr.contains("HT-203_"))
            n = 0;
        else if (path->atr.contains("HT-211-"))
            n = 1;
        else if (path->atr.contains("devNT-16_"))
            n = 2;
        if (gettingComp == nullptr) {
            path->setStateColor(this, stateColorCI[n]);
            continue;
        }
        if (pathStateColor.raiting < stateColorCI[n].raiting) {
            path->setStateColor(this, stateColorCI[n]);
        } else if (gettingComp == this) {
            path->setStateColor(this, stateColorCI[n]);
        }
    }
}

/**
* @brief Component::udpateColorNT102 - Обновить цвет, берется из модели
*/
void Component::udpateColorNT102() {
int n = -1;
    stateColor = model->getStateColor(1);
    setBrush(QBrush(stateColor.color));

    Color stateColorCI[2];
    stateColorCI[0].color = Qt::gray;
    stateColorCI[0].raiting = GRAY;
    stateColorCI[1] = stateColor;

    int size = pathCompTo.size();
    for (int i = 0; i < size; ++i) {
        PathOfLines* path = pathCompTo[i];
        Color pathStateColor = path->getStateColor();
        Component* gettingComp = path->getComponentColor();
        if (path->atr.contains("HT-03-2_"))
            n = 0;
        else
            n = 1;
        if (gettingComp == nullptr) {
            path->setStateColor(this, stateColorCI[n]);
            continue;
        }
        if (pathStateColor.raiting < stateColorCI[n].raiting) {
            path->setStateColor(this, stateColorCI[n]);
        } else if (gettingComp == this) {
            path->setStateColor(this, stateColorCI[n]);
        }
    }
    stateColor = model->getStateColor(0);
    if (stateColor.raiting == GREEN && stateColor.color != Qt::green) {
        stateColor.color = Qt::green;
    }
    size = pathCompFrom.size();
    for (int i = 0; i < size; ++i) {
        PathOfLines* path = pathCompFrom[i];
        Color pathStateColor = path->getStateColor();
        Component* gettingComp = path->getComponentColor();
        if (gettingComp == nullptr) {
            path->setStateColor(this, stateColor);
            continue;
        }
        if (pathStateColor.raiting < stateColor.raiting) {
            path->setStateColor(this, stateColor);
        } else if (gettingComp == this) {
            path->setStateColor(this, stateColor);
        }
    }
}

/**
* @brief Component::udpateColor - обновить цвет
*/
void Component::udpateColor() {
    stateColor = model->getStateColor(1);
    setBrush(QBrush(stateColor.color));
    if (stateColor.raiting == GREEN && stateColor.color != Qt::green) {
        stateColor.color = Qt::green;
    }

    int size = pathCompTo.size();
    for (int i = 0; i < size; ++i) {
        PathOfLines* path = pathCompTo[i];
        Color pathStateColor = path->getStateColor();
        Component* gettingComp = path->getComponentColor();
        if (gettingComp == nullptr) {
            path->setStateColor(this, stateColor);
            continue;
        }
        if (pathStateColor.raiting < stateColor.raiting) {
            path->setStateColor(this, stateColor);
        } else if (gettingComp == this) {
            path->setStateColor(this, stateColor);
        }
    }
    stateColor = model->getStateColor(0);
    if (stateColor.raiting == GREEN && stateColor.color != Qt::green) {
        stateColor.color = Qt::green;
    }
    size = pathCompFrom.size();
    for (int i = 0; i < size; ++i) {
        PathOfLines* path = pathCompFrom[i];
        Color pathStateColor = path->getStateColor();
        Component* gettingComp = path->getComponentColor();
        if (gettingComp == nullptr) {
            path->setStateColor(this, stateColor);
            continue;
        }
        if (pathStateColor.raiting < stateColor.raiting) {
            path->setStateColor(this, stateColor);
        } else if (gettingComp == this) {
            path->setStateColor(this, stateColor);
        }
    }
}

/**
* @brief Component::setRect - компоенент - прямоуголник, настройка его параметрова
* @param x - (qreal)
* @param y - (qreal)
* @param width - (qreal)
* @param height - (qreal)
*/
void Component::setRect(qreal x, qreal y, qreal width, qreal height) {
    QGraphicsRectItem::setRect(x, y, width, height);
}

/**
* @brief Component::appendSymbol - добавление символов
* @param item - (QGraphicsSimpleTextItem *)
*/
void Component::appendSymbol(QGraphicsSimpleTextItem *item) {
    item->setParentItem(this);
    symbols.append(item);
}

/**
* @brief Component::highlightPaths - пометить путь
*/
void Component::highlightPaths() {
    for (PathOfLines *lines : pathCompFrom) {
        lines->highlight();
    }
    for (PathOfLines *lines : pathCompTo) {
        lines->highlight();
    }
}

/**
* @brief Component::highlightPaths - сбросить путь при расфокусировки
*/
void Component::offHighlightPaths() {
    for (PathOfLines *lines : pathCompFrom) {
        lines->offHighlight();
    }
    for (PathOfLines *lines : pathCompTo) {
        lines->offHighlight();
    }
}

/**
* @brief Component::mousePressEvent - обработка щелчка мыши
* @param event - (QGraphicsSceneMouseEvent *)
*/
void Component::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (pathCompFrom.size() == 0 && pathCompTo.size() == 0) {
        QGraphicsRectItem::mousePressEvent(event);
        return;
    }
    GraphicsScene* sceneGraphics = dynamic_cast<GraphicsScene*>(this->scene());
    if (sceneGraphics->isHighlightComp()) {
        QGraphicsRectItem::mousePressEvent(event);
        return;
    }
    if (sceneGraphics)
        sceneGraphics->setHighlightComp(this);
    this->highlightPaths();
    sceneGraphics->close();
    QGraphicsRectItem::mousePressEvent(event);
}
/**
* @brief Component::mouseDoubleClickEvent - обработка двойного щелчка
* @param event - (QGraphicsSceneMouseEvent *)
*/
void Component::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    if (!model || model->getCountRows()) {
        QGraphicsRectItem::mousePressEvent(event);
        return;
    }
    GraphicsScene *sceneGraphics = dynamic_cast<GraphicsScene*>(this->scene());
    sceneGraphics->setModelComponent(model);
    sceneGraphics->showInfoComp();
    QGraphicsRectItem::mousePressEvent(event);
}
