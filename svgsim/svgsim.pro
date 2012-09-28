#-------------------------------------------------
#
# Project created by QtCreator 2012-09-23T16:07:22
#
#-------------------------------------------------

QT       += core xml network


TARGET = svgsim
CONFIG   += console
#CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

INCLUDEPATH +=/usr/local/include/Box2D

LIBS     += /usr/local/lib/libBox2D.a
