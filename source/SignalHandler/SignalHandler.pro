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

INCLUDEPATH     += /ScriptLangWrapper
INCLUDEPATH     += /exceptions
INCLUDEPATH     += /interfaces
INCLUDEPATH     += /communication
INCLUDEPATH     += ../../externals/include/
INCLUDEPATH     += ../utils/messagegroup/
INCLUDEPATH     += ../utils/messages/

LIBS            += -L../../build/lib/ -lmessagegroup
LIBS            += -L../../build/lib/ -lmessages
LIBS            += -L$$PWD/../../externals/lib/ -lqamqp


TEMPLATE = app


SOURCES += main.cc \
    signal.cc \
    scriptbank.cc \
    scriptbankbuilder.cc \
    exceptions/unknownscript.cc \
    signalqueue.cc \
    exceptions/badmessage.cc \
    ScriptLangWrapper/scriplangwrapperfactory.cc \
    ScriptLangWrapper/scriptlangwrapperpool.cc \
    communication/signalreader.cc

HEADERS += \
    signal.hh \
    scriptbank.hh \
    interfaces/scriptbankbuilder.hh \
    interfaces/scriptprioritylibrary.hh \
    interfaces/scriptlibrary.hh \
    exceptions/unknownscript.hh \
    interfaces/scriptupdateobserver.hh \
    interfaces/scriptupdatesubject.hh \
    exceptions/badmessage.hh \
    interfaces/scriptbankinterface.hh \
    interfaces/priorityupdateobserver.hh \
    interfaces/priorityupdatesubject.hh \
    signalqueue.hh \
    ScriptLangWrapper/scriptlangwrapper.hh \
    ScriptLangWrapper/scriptlangwrapperpool.hh \
    ScriptLangWrapper/scriptlangwrapperfactory.hh \
    communication/signalreader.hh
