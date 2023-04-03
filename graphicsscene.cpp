#include "graphicsscene.h"

#include <QDebug>

/**
 * @brief GraphicsScene::GraphicsScene
 * @param parent - (QObject *)
 * @param parent1 - (QObject *)
 */
GraphicsScene::GraphicsScene(QObject *parent, QWidget *parent1) : QGraphicsScene(parent) {
    view = new QTableView();
    view->setGeometry(50, 50, 370, 190);
    highlightComp = nullptr;
    view->setParent(parent1);
    view->setWindowFlags(Qt::Tool | Qt::Dialog);
}

/**
* @brief GraphicsScene::setHighlightComp - вставка выделенного компонента при прокрашивании пути
* @param comp - (Component *)
*/
void GraphicsScene::setHighlightComp(Component *comp) {
    highlightComp = comp;
}

/**
* @brief GraphicsScene::isHighlightComp - проверка, является ли путь прокрашенным
* @return - (bool)
*/
bool GraphicsScene::isHighlightComp() {
    return highlightComp != nullptr;
}

/**
* @brief GraphicsScene::setModelComponent - вставка в дерево модель компоента
* @param model - (Model *)
*/
void GraphicsScene::setModelComponent(Model *model) {
    view->setWindowTitle(model->getNameModel());
    view->setModel(model);
    view->setColumnWidth(0, 250);
    view->hide();
}

/**
* @brief GraphicsScene::showInfoComp - показать информацию о компоненте
*/
void GraphicsScene::showInfoComp() {
    view->show();
}

/**
* @brief GraphicsScene::close - закрыть сцену
*/
void GraphicsScene::close() {
    view->close();
}

/**
* @brief GraphicsScene::mousePressEvent - шелчок мыши по сцене, обработка логики
* @param event - (QGraphicsSceneMouseEvent *)
*/
void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (highlightComp) {
        highlightComp->offHighlightPaths();
    }
    highlightComp = nullptr;
    view->close();
    QGraphicsScene::mousePressEvent(event);

    this->update();
}
