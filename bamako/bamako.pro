#-------------------------------------------------
#
# Project created by QtCreator 2012-08-29T16:01:05
#
#-------------------------------------------------

QT       += core gui

TARGET = bamako
TEMPLATE = app


SOURCES += main.cpp\
    Simulator.cpp \
    SimulatorScene.cpp \
    SimulatorView.cpp \
    Robot.cpp \
    MainWindow.cpp \
    qcustomplot.cpp

HEADERS  += \
    Simulator.h \
    SimulatorScene.h \
    SimulatorView.h \
    Robot.h \
    MainWindow.h \
    qcustomplot.h

FORMS    += \
    MainWindow.ui

INCLUDEPATH +=/usr/local/include/Box2D/
LIBS += -lBox2D #utile ?
