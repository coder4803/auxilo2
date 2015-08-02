#-------------------------------------------------
#
# Project created by QtCreator 2015-07-26T08:35:00
#
#-------------------------------------------------

QT       += xml testlib

QT       -= gui

TARGET = tst_xmlhandlertest
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

HEADERS += ../../confxmlhandler.hh 

SOURCES += \
    ../../confxmlhandler.cc \
    tst_xmlhandlertest.cc

DEPENDPATH += ../../

INCLUDEPATH += ../../
 
DEFINES += SRCDIR=\\\"$$PWD/\\\"
