#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QDir>
#include <QString>

class Logger
{
public:
    Logger(QString filename);
    ~Logger();

    void accept(const QString& msg);

private:
    QFile file;
};

#endif // LOGGER_H
