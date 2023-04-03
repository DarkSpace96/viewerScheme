#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QTableView>
#include <QWidget>
#include "component.h"

class GraphicsScene : public QGraphicsScene
{
public:
    explicit GraphicsScene(QObject *parent = nullptr, QWidget *parent1 = nullptr);

public:
    void setHighlightComp(Component *comp);
    void setModelComponent(Model *model);
    void showInfoComp();
    void close();
    bool isHighlightComp();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    bool isHighlightComp_;
    Component *highlightComp;
    QTableView *view;
};

#endif // GRAPHICSSCENE_H
