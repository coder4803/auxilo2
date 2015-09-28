#-------------------------------------------------
#
# Project created by QtCreator 2015-09-28T11:09:32
#
#-------------------------------------------------

QT       += network xml testlib

QT       -= gui

TARGET = tst_confreadertest
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

LIBS            += -L../../../../../build/lib/ -lmessagegroup \
                   -L../../../../../build/lib/ -lmessages

INCLUDEPATH     += ../../../../utils/messagegroup/ \
                   ../../../../utils/messages/ \
                   ../../

HEADERS += ../../confreader.h \
           ../../confxmlhandler.h \
           ../../evententity.h

SOURCES += tst_confreadertest.cpp \
           ../../confreader.cpp \
           ../../confxmlhandler.cpp \
           ../../evententity.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
