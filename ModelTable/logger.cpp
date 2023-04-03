#include "logger.h"

Logger::Logger(QString filename)
{
//    file.setFileName("D:/5507-1/tvc5507-1/log/" + filename + ".log");

    file.setFileName(QDir::currentPath() + "/log/" + filename + ".log");
#ifdef Q_OS_UNIX
    file.setFileName(QDir::currentPath() + "/log/" + filename + ".log");
#endif
    file.open(QIODevice::WriteOnly | QIODevice::Text);
}

Logger::~Logger() {
    file.write("close");
    file.flush();
    file.close();
}

void Logger::accept(const QString& msg) {
    file.write(msg.toLocal8Bit() + "\n");
    file.flush();
};
