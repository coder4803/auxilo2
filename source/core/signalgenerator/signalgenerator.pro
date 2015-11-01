#-------------------------------------------------
#
# Project created by QtCreator 2015-09-27T18:55:02
#
#-------------------------------------------------

QT       += core xml network sql

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
    confxmlhandler.cpp \
    evententity.cpp \
    eventmanager.cpp \
    userinterface.cpp \
    consoleui.cpp \
    businesslogic.cpp \
    eventreceiver.cpp \

HEADERS += \
    confreader.h \
    confxmlhandler.h \
    evententity.h \
    eventmanager.h \
    viewinterface.h \
    uiplugininterface.h \
    userinterface.h \
    consoleui.h \
    businesslogic.h \
    modelinterface.h \
    eventreceiver.h \

DESTDIR         = ../../../build/bin/
