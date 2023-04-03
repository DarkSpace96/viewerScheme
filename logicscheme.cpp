#include "logicscheme.h"
#include "scheme.h"

#include <QDebug>

/**
 * @brief LogicScheme::LogicScheme - инициализация класса логики
 */
LogicScheme::LogicScheme() {
    scheme = nullptr;
}

/**
* @brief LogicScheme::initScheme - инцилизация основной сцены
* @param parent - (QWidget *)
*/
void LogicScheme::initScheme(QWidget *parent) {
    scheme = new Scheme(parent);
    blocks = scheme->getBlocks();
}

/**
* @brief LogicScheme::showShceme - показать основную сцену
*/
void LogicScheme::showShceme() {
    if (scheme) {
        scheme->setGeometry(0, 0, 0, 0);
        scheme->show();
    }
}
/**
* @brief LogicScheme::visibleNotShceme - скрыть сцену
*/
void LogicScheme::visibleNotShceme() {
    if (scheme)
        scheme->setVisible(false);
}

/**
 * @brief LogicScheme::getScheme - возвращение ссылка на сцену
 * @return - (Scheme *)
 */
Scheme *LogicScheme::getScheme() {
    return scheme;
}

/**
* @brief LogicScheme::addModel - добавить модели компонентов
* @param model - (Model *)
*/
void LogicScheme::addModel(Model *model) {
QString nameModel = model->getName();
qDebug() << "LogicScheme::addModel" << nameModel;
    for (Block* block: blocks) {
        QList<Component*> components = block->getComponents();
        QString nameBlock = block->getName();
        qDebug() << "nameBlock" << nameBlock;
        if (nameBlock == nameModel && nameModel == "CPU") {
            qDebug() << "ok finding" << nameBlock;
            block->setModel(model);
            return;
        }
        for (Component* comp : components) {
            QString fullName =  nameBlock + "_" + QString::number(comp->getID());
            qDebug() << "fullName" << fullName;
            if (fullName == nameModel) {
                qDebug() << "ok finding" << fullName;
                comp->setModel(model);
                return;
            }
        }
    }
}


