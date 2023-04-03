#-------------------------------------------------
#
# Project created by QtCreator 2020-04-25T12:27:18
#
#-------------------------------------------------

QT += serialport xml
QT += core gui
QT += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = viewerScheme
TEMPLATE = app

QMAKE_CFLAGS += -Wall -Wextra -O3 -mtune=generic
#QMAKE_CXXFLAGS += -00
#QMAKE_CXXFLAGS -= -01
#QMAKE_CXXFLAGS -= -02
#QMAKE_CXXFLAGS -= -02
QMAKE_CXXFLAGS += -Wall -Wextra -O3 -mtune=generic -std=c++11

SOURCES += \
    ModelTable/DevCam.cpp \
#    ModelTable/DevClient.cpp \
#    ModelTable/DevCpu.cpp \
    ModelTable/DevDisk.cpp \
    ModelTable/DevLamps.cpp \
    ModelTable/DevLor.cpp \
    ModelTable/DevNT102_3.cpp \
#    ModelTable/DevNT16.cpp \
#    ModelTable/DevNT19.cpp \
#    ModelTable/DevNT223_2.cpp \
    ModelTable/DevOnOff.cpp \
    ModelTable/DevPort.cpp \
    ModelTable/DevSensor.cpp \
#    ModelTable/DevVideoCapture.cpp \
#    ModelTable/DevZenit.cpp \
    ModelTable/Devices.cpp \
    ModelTable/logger.cpp \
    ModelTable/model.cpp \
    block.cpp \
    component.cpp \
    graphicsscene.cpp \
    logicscheme.cpp \
    main.cpp \
    pathoflines.cpp \
    scheme.cpp

HEADERS  += \
    ModelTable/DevCam.h \
#    ModelTable/DevClient.h \
#    ModelTable/DevCpu.h \
    ModelTable/DevDisk.h \
    ModelTable/DevLamps.h \
    ModelTable/DevLor.h \
    ModelTable/DevNT102_3.h \
#    ModelTable/DevNT16.h \
#    ModelTable/DevNT19.h \
#    ModelTable/DevNT223_2.h \
    ModelTable/DevOnOff.h \
    ModelTable/DevPort.h \
    ModelTable/DevSensor.h \
#    ModelTable/DevVideoCapture.h \
#    ModelTable/DevZenit.h \
    ModelTable/Devices.h \
    ModelTable/device.h \
    ModelTable/logger.h \
    ModelTable/model.h \
    block.h \
    component.h \
    graphicsscene.h \
    logicscheme.h \
    pathoflines.h \
    scheme.h \
    struct.h

DISTFILES += \
    SVG/Scheme_new.svg

RESOURCES += \
    ressource.qrc

