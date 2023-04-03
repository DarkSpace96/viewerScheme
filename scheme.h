#ifndef SCHEME_H
#define SCHEME_H

#include "block.h"
#include "graphicsscene.h"

#include <QDomDocument>
#include <QFile>
#include <QGraphicsView>
#include <QRectF>

class Scheme : public QGraphicsView
{
    Q_OBJECT

signals:
    void closingOfWidget();

public:
    explicit Scheme(QWidget *parent = nullptr);

private:
    QDomDocument doc;    // объект документа
    QFile file;
    QMap<QString,PathOfLines*> paths;
    QList<Block*> blocks;
    GraphicsScene *mainScene;  // Графическая сцена

    void writeContextIntoDocument(const QString &fileName);
    void findingComponentForPath(const QString &attribute, Component *&compFrom, Component *&compTo);
    QRectF getSizes(const QString filename);
    QGraphicsSimpleTextItem *createSymbol(const QDomElement &element);
    QList<QPointF> createPointForPath(const QMap<int, QLineF> &mapLine);
    Block *createBlock(const QDomElement &parent);
    Component *createComponent(const QDomElement &element);
    PathOfLines *createPath(const QDomElement &parent);

public:
    QList<Block*> getBlocks();

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // SCHEME_H
