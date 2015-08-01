#-------------------------------------------------
#
# Project created by QtCreator 2015-07-12T22:37:24
#
#-------------------------------------------------

QT       += testlib network

QT       -= gui

TARGET = tst_configurationreadertest
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

HEADERS += \
    ../../communication/configurationreader.hh

SOURCES += \
    tst_configurationreadertest.cc \
    ../../communication/configurationreader.cc

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../../
INCLUDEPATH += ../../interfaces
INCLUDEPATH += ../../communication
INCLUDEPATH += ../../exceptions
INCLUDEPATH += ../../../utils/messagegroup/
INCLUDEPATH += ../../../utils/messages/

DEPENDPATH += ../../
DEPENDPATH += ../../interfaces
DEPENDPATH += ../../communication
DEPENDPATH += ../../exceptions

LIBS += -L../../../../build/lib/ -lmessagegroup
LIBS += -L../../../../build/lib/ -lmessages
