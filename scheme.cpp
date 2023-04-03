#include "scheme.h"

#include <QDebug>

/**
* @brief Scheme::Scheme - основное приложение
* @param parent - (QWidget *)
*/
Scheme::Scheme(QWidget *parent) : QGraphicsView(parent) {
QString path = "D:/5507-1/viewerScheme/SVG/Scheme_new.svg";
QString stile;
    stile = QString::fromUtf8("QWidget {\n"
                              "background-color: rgb(50, 50, 50);\n"
                              "color: rgb(220, 220, 220);\n"
                              "font: bold 12pt\"Arial\";\n"
                              "margin:0px;"
                              "padding:0px;"
                              "}\n"
                              );
    setParent(parent, Qt::Tool);
    setVisible(false);
    setStyleSheet(stile);
    setWindowTitle("Параметры");
    mainScene = new GraphicsScene(this, this);
    mainScene->setSceneRect(getSizes(path));
    setGeometry(0, 0, 1910, 1015);
    setFixedSize(1910, 1015);
    this->setScene(mainScene);
    writeContextIntoDocument(path);
    QDomElement KI = doc.elementsByTagName("g").at(3).toElement();
    qDebug() << KI.text();
    QDomElement root = doc.documentElement();
    QDomElement tagContainer = root.firstChild().toElement();
    while (!tagContainer.isNull()) {
        QString attributeOfID = tagContainer.attribute("id").remove("_x5F");
        if (attributeOfID.contains("from")) {
            qDebug() << attributeOfID;
            PathOfLines* pathFor = createPath(tagContainer);
            paths[attributeOfID] = pathFor;
            mainScene->addItem(pathFor);
        } else if (attributeOfID.contains("superblock_")) {
            qDebug() << "superBlock";
            QDomElement element = tagContainer.firstChild().toElement();
            while (!element.isNull()) {
                Block* block = createBlock(element);
                blocks.append(block);
                mainScene->addItem(block);
                element = element = element.nextSibling().toElement();
            }
        } else if (attributeOfID.contains("block_")) {
                Block* block = createBlock(tagContainer);
                blocks.append(block);
                mainScene->addItem(block);
        }
        tagContainer = tagContainer.nextSibling().toElement();
    }

    Component* compFrom = nullptr;
    Component* compTo = nullptr;
    for (QString attribute : paths.keys()) {
        findingComponentForPath(attribute, compFrom, compTo);
        PathOfLines* pathFor = paths[attribute];
        pathFor->atr = attribute;
        if (compFrom && compTo) {
            compFrom->addPathToOtherCompFrom(pathFor);
            compTo->addPathToOtherCompTo(pathFor);
        }
    }
}

/**
* @brief Scheme::getSizes - размер самого виджета
* @param filename - (const QString)
* @return - (QRectF)
*/
QRectF Scheme::getSizes(const QString filename) {
QDomDocument doc;       // инициализируем в стеке объект QDomDocument
QFile file(filename);   // Открываем наш SVG-файл
    // Если он не открылся или не удалось передать содержимое в QDocDocument
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return QRectF(0,0,200,200); // то возвратим значения для сцены по умолчанию

    /* Далее забираем список элементов с тегом svg.
     * В случае, если список элементов будет не пустой,
     * то заберём размеры графической сцены.
     * */
    QDomNodeList list = doc.elementsByTagName("svg");
    if (list.length() > 0) {
        QDomElement svgElement = list.item(0).toElement();
        QStringList parameters = svgElement.attribute("viewBox").split(" ");
        return QRectF(parameters.at(0).toDouble(),
                      parameters.at(1).toDouble(),
                      parameters.at(2).toDouble(),
                      parameters.at(3).toDouble());
    }
    return QRectF(0,0,200,200);
}

/**
* @brief Scheme::createBlock - выделить из файла и создать компонент-блок
* @param parent - (const QDomElement &)
* @return - (Block *)
*/
Block *Scheme::createBlock(const QDomElement &parent) {
Block *block = new Block();
QString blockName = parent.attribute("id").remove("block_x5F_");
    block->setName(blockName);
    QDomElement element = parent.firstChild().toElement();
    while (!element.isNull()) {
        QString tagName = element.tagName();
        if (tagName == "text") {
            QGraphicsSimpleTextItem* symbol = createSymbol(element);
            block->appendSymbol(symbol);
        } else if (element.tagName() == "rect") {
            qreal x = element.attribute("x").toDouble();
            qreal y = element.attribute("y").toDouble();
            qreal width = element.attribute("width").toDouble();
            qreal height = element.attribute("height").toDouble();
            block->setRect(x, y, width, height);
        } else if (tagName == "g") {
            Component* component = createComponent(element);
            component->setParentItem(block);
            block->addComponent(component);
        }
        element = element.nextSibling().toElement();
    }
    if (block->getCountComp() == 0) {
        Component* comp = block;
        comp->setID(0);
        block->addComponent(comp);
    }
    return block;
}

/**
* @brief Scheme::createSymbol - выделить из файла и создать компонент-символ
* @param element - (const QDomElement &)
* @return - (QGraphicsSimpleTextItem *)
*/
QGraphicsSimpleTextItem *Scheme::createSymbol(const QDomElement &element) {
QGraphicsSimpleTextItem *symbol = new QGraphicsSimpleTextItem();
    symbol->setText(element.text());
    int fontSize = element.attribute("font-size").remove("px").toInt();
    qreal x = element.attribute("transform").section(' ', 4, 4).toDouble();
    qreal y = element.attribute("transform").section(' ', 5, 5).remove(')').toDouble() - double(fontSize);
    qDebug() << "y=" << y;
    QString fontString = element.attribute("font-family").remove("'");
    qDebug() <<  "fontSize: "<< fontSize << "fontSizeString:" << element.attribute("font-size");
    QFont font(fontString);
    font.setPixelSize(fontSize);
    symbol->setFont(font);
    symbol->setX(x);
    symbol->setY(y);
    return symbol;
}

/**
* @brief Scheme::createComponent - выделить из файла и создать компонент
* @param parent - (const QDomElement &)
* @return - (Component *)
*/
Component *Scheme::createComponent(const QDomElement &parent) {
QDomElement element = parent.firstChild().toElement();
Component *component = new Component();
bool ok, ok_2;
int id = parent.attribute("id").remove("component_x5F_").mid(0, 1).toInt(&ok);
int idForTowNum = parent.attribute("id").remove("component_x5F_").mid(0, 2).toInt(&ok_2);
    ok_2 ? component->setID(idForTowNum) :
    ok ? component->setID(id) : component->setID(-1);
    while (!element.isNull()) {
        QString tagName = element.tagName();
        if (tagName == "rect") {
            qreal x = element.attribute("x").toDouble();
            qreal y = element.attribute("y").toDouble();
            qreal width = element.attribute("width").toDouble();
            qreal height = element.attribute("height").toDouble();
            component->setRect(x, y, width, height);
        } else if (tagName == "text") {
            QGraphicsSimpleTextItem* symbol = createSymbol(element);
            component->appendSymbol(symbol);
        }
        element = element.nextSibling().toElement();
    }
    return component;
}

/**
* @brief Scheme::createPointForPath - выделить из файла и создать точки путей
* @param mapLine - (const QMap<int, QLineF> &)
* @return - (QList<QPointF>)
*/
QList<QPointF> Scheme::createPointForPath(const QMap<int, QLineF> &mapLine) {
QList<QPointF> pointsForPath;
    for (int i = 1; i < mapLine.size() + 1; ++i) {
        if (i == 1) {
            pointsForPath << mapLine[i].p1()
                   << mapLine[i].p2();
        } else {
            pointsForPath << mapLine[i].p2();
        }
    }
    return pointsForPath;
}

/**
* @brief Scheme::createPath - создать путь
* @param parent - (const QDomElement &)
* @return - (PathOfLines *)
*/
PathOfLines *Scheme::createPath(const QDomElement &parent) {
QDomElement containerChild = parent.firstChild().toElement();
QMap<int, QLineF> mapLines;
    while (!containerChild.isNull()) {
        if (containerChild.tagName() != "line")
            continue;
        qreal x1 = containerChild.attribute("x1").toDouble();
        qreal y1 = containerChild.attribute("y1").toDouble();
        qreal x2 = containerChild.attribute("x2").toDouble();
        qreal y2 = containerChild.attribute("y2").toDouble();
        int idLine =  containerChild.attribute("id").remove("_x3").mid(0,1).toInt();
        mapLines[idLine] = QLineF(x1, y1, x2, y2);
        containerChild = containerChild.nextSibling().toElement();
    }
    QList<QPointF> pointsForPath = createPointForPath(mapLines);
    PathOfLines *path = new PathOfLines();
    path->setPoints(pointsForPath);
    return path;
}

/**
* @brief Scheme::findingComponentForPath - сопостовление компонентов и путей
* @param attribute - (const QString &)
* @param compFrom - изходящие пути (Component *&)
* @param compTo - входящие путей (Component *&)
*/
void Scheme::findingComponentForPath(const QString &attribute, Component *&compFrom, Component *&compTo) {
int indexTo = attribute.indexOf("to");
QStringList listOfFrom = attribute.left(indexTo - 1).split('_');
    listOfFrom.removeFirst();
    QStringList listOfTo = attribute.right(attribute.size() - indexTo).split('_');
    listOfTo.removeFirst();

    compFrom = nullptr;
    compTo = nullptr;

    qDebug() << attribute;

    Block* blockFrom = nullptr;
    int sizeListOfFrom = listOfFrom.size();
    for (int i = 0; i < sizeListOfFrom; ++i) {
        if (i == 0) {
            for (Block* block : blocks) {
                if (block->getName() == listOfFrom[0]) {
                    blockFrom = block;
                }
            }
        }
        if (i == 1 && blockFrom) {
            bool ok;
            int idComp = listOfFrom[i].toInt(&ok);
            if (ok) {
                compFrom = blockFrom->getComponent(idComp);
            }
        }
    }
    if (sizeListOfFrom == 1)
        compFrom = blockFrom;

    Block *blockTo = nullptr;
    int sizeListOfTo = listOfTo.size();
    for (int i = 0; i < sizeListOfTo; ++i) {
        if (i == 0) {
            for (Block *block : blocks) {
                if (block->getName() == listOfTo[0]) {
                    blockTo = block;
                }
            }
        }
        if (i == 1 && blockTo) {
            bool ok;
            int idComp = listOfTo[i].toInt(&ok);
            if (ok) {
                compTo = blockTo->getComponent(idComp);
            }
        }
    }
    if (sizeListOfTo == 1)
        compTo = blockTo;
}

/**
* @brief Scheme::getBlocks - вернуть блоки
* @return - (QList<Block *>)
*/
QList<Block *> Scheme::getBlocks() {
    return blocks;
}

/**
* @brief Scheme::writeContextIntoDocument - открываем файл для чтения
* @param fileName - (const QString &)
*/
void Scheme::writeContextIntoDocument(const QString &fileName) {
    file.setFileName(fileName);   // Открываем наш SVG-файл
    // Если он не открылся или не удалось передать содержимое в QDocDocument
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        qDebug() << "файл не открывается!!!";    // то возвратим список, но пустой
}

/**
* @brief Scheme::closeEvent - закрытие программы
* @param event - (QCloseEvent *)
*/
void Scheme::closeEvent(QCloseEvent *event) {
    emit closingOfWidget();
    mainScene->close();
    QWidget::closeEvent(event);
}
