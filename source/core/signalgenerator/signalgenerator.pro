#-------------------------------------------------
#
# Project created by QtCreator 2015-09-27T18:55:02
#
#-------------------------------------------------

QT       += core xml network

QT       -= gui

TARGET = signalgenerator
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app


LIBS            += -L../../../build/lib/ -lmessagegroup \
                   -L../../../build/lib/ -lmessages


INCLUDEPATH     += ../../utils/messagegroup/ \
                   ../../utils/messages/


SOURCES += main.cpp \
    confreader.cpp \
    confxmlhandler.cpp

HEADERS += \
    confreader.h \
    confxmlhandler.h

DESTDIR         = ../../../build/bin/
