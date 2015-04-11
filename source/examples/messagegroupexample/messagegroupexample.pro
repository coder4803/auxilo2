#-------------------------------------------------
#
# Project created by QtCreator 2015-03-11T18:33:55
#
#-------------------------------------------------

QT              += core
QT              -= gui

TARGET          = messagegroupexample
CONFIG          += console
CONFIG          -= app_bundle

TEMPLATE        = app

INCLUDEPATH     += ../../../externals/include/
INCLUDEPATH     += ../../utils/messagegroup/

LIBS            += -L../../../build/lib/ -lmessagegroup
LIBS            += -L$$PWD/../../../externals/lib/ -lqamqp

SOURCES         += main.cpp \
                   messager.cpp

HEADERS         += messager.h

DESTDIR         = ../../../build/bin/

