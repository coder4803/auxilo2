#-------------------------------------------------
#
# Project created by QtCreator 2015-03-11T18:33:55
#
#-------------------------------------------------

QT              += core
QT              -= gui

TARGET          = signalgroupexample
CONFIG          += console
CONFIG          -= app_bundle

TEMPLATE        = app

INCLUDEPATH     += ../../../externals/include/
INCLUDEPATH     += ../../utils/signalgroup/

LIBS            += -L../../../build/lib/ -lsignalgroup
LIBS            += -L$$PWD/../../../externals/lib/ -lqamqp

SOURCES         += main.cpp \
                   messager.cpp

HEADERS         += messager.h

DESTDIR         = ../../../build/bin/

