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
    MainWindow.cpp

HEADERS  += \
    Simulator.h \
    SimulatorScene.h \
    SimulatorView.h \
    Robot.h \
    MainWindow.h

FORMS    += \
    MainWindow.ui

INCLUDEPATH +=/usr/local/include/Box2D/
#LIBPATH += /home/ahanssen/Box2D_v2.0.1/Box2D/Source/Gen/float
LIBS += -lBox2D
