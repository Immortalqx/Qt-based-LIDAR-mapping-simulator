#-------------------------------------------------
#
# Project created by QtCreator 2021-04-09T12:20:27
#
#-------------------------------------------------

QT       += core gui

# 如果 QT 版本大于 4（Qt5 或更高版本），则需要添加 widgets 模块，该模块包含所有控件类。
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Simulation
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/simulator.cpp \
    src/car.cpp \
    src/rmap.cpp \
    src/viewer.cpp \
    src/mapper.cpp \
    src/logprinter.cpp \
    src/console.cpp \
    src/lidar.cpp

HEADERS += \
    include/mainwindow.h \
    include/simulator.h \
    include/car.h \
    include/rmap.h \
    include/viewer.h \
    include/mapper.h \
    include/logprinter.h \
    include/console.h \
    include/lidar.h

RESOURCES += \
    source/car.qrc
