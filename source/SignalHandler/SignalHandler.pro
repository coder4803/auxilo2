#-------------------------------------------------
#
# Project created by QtCreator 2015-03-04T21:45:11
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = SignalHandler
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app


SOURCES += main.cc \
    signal.cc \
    scriptbank.cc \
    scriptbankbuilder.cc \
    unknownscript.cc \
    signalqueue.cc \
    badmessage.cc

HEADERS += \
    signal.hh \
    scriptbank.hh \
    scriptbankbuilder.hh \
    scriptprioritylibrary.hh \
    scriptlibrary.hh \
    unknownscript.hh \
    scriptupdateobserver.hh \
    scriptupdatesubject.hh \
    badmessage.hh \
    scriptbankinterface.hh \
    priorityupdateobserver.hh \
    priorityupdatesubject.hh \
    signalqueue.hh
