#ifndef LOGICSCHEME_H
#define LOGICSCHEME_H

#include "scheme.h"
#include <QWidget>

#include <ModelTable/model.h>

class LogicScheme
{
public:
    LogicScheme();
    void initScheme(QWidget *parent);
    void showShceme();
    void addModel(Model *model);
    void visibleNotShceme();
    Scheme *getScheme();

private:
    Scheme *scheme;
    QList<Block *> blocks;
};

#endif // LOGICSCHEME_H
