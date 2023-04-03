DEPENDENCY_PROJECT += communication functions grid

# Check if the config file exists
! include( ../common.pri ) {
    error( "interfaceX Couldn't find the common.pri file!" )
}

TARGET = Device         #Name of application
TEMPLATE = lib          #Needs to be lib NOT app
CONFIG += staticlib

CONFIG += console
CONFIG -= app_bundle
CONFIG += qt

CONFIG += c++17

QT += widgets network serialport

SOURCES += \
    DevCam.cpp \
    DevClient.cpp \
    DevCpu.cpp \
    DevDisk.cpp \
    Devices.cpp \
    DevLamps.cpp \
    DevNT102_3.cpp \
    DevLor.cpp \
    DevOnOff.cpp \
    DevPort.cpp \
    DevSensor.cpp \
    DevVideoCapture.cpp \
    DevZenit.cpp \
    DevNT223_2.cpp

HEADERS += \
    DevCam.h \
    DevClient.h \
    DevCpu.h \
    DevDisk.h \
    Devices.h \
    DevLamps.h \
    DevLor.h \
    DevNT102_3.h \
    DevOnOff.h \
    DevPort.h \
    DevSensor.h \
    DevVideoCapture.h \
    DevZenit.h \
    DevNT223_2.h
