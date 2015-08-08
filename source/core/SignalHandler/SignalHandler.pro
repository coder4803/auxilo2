#-------------------------------------------------
#
# Project created by QtCreator 2015-03-04T21:45:11
#
#-------------------------------------------------

QT       += core script xml network

QT       -= gui

TARGET = SignalHandler
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

INCLUDEPATH     += ../../utils/messagegroup/ \
                   ../../utils/messages/ \
                   /ScriptLangWrapper \
                   /exceptions \
                   /interfaces \
                   /communication \
                   /ScriptAPI



LIBS            += -L../../../build/lib/ -lmessagegroup \
                   -L../../../build/lib/ -lmessages

TEMPLATE = app

SOURCES += main.cc \
    signal.cc \
    scriptbank.cc \
    scriptbankbuilder.cc \
    exceptions/unknownscript.cc \
    signalqueue.cc \
    exceptions/badmessage.cc \
    ScriptLangWrapper/scriptlangwrapperpool.cc \
    communication/signalreader.cc \
    scriptrunner.cc \
    ScriptLangWrapper/qtscriptwrapper.cc \
    ScriptLangWrapper/scriptlangwrapperfactory.cc \
    ScriptAPI/scriptapiimplementation.cc \
    signalhandlerbuilder.cc \
    ScriptLangWrapper/scriptapiqobjectwrapper.cc \
    communication/configurationreader.cc \
    businesslogic.cc \
    confxmlhandler.cc \
    scriptrunnerpool.cc \
    UserInterface/consoleui.cc \
    UserInterface/uifactory.cc \
    UserInterface/userinterface.cc

HEADERS += \
    signal.hh \
    scriptbank.hh \
    scriptbankbuilder.hh \
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
    communication/signalreader.hh \
    scriptrunner.hh \
    exceptions/scriptrunexceptions.hh \
    ScriptLangWrapper/qtscriptwrapper.hh \
    ScriptAPI/scriptapi.hh \
    ScriptAPI/scriptapiimplementation.hh \
    interfaces/modelinterface.hh \
    signalhandlerbuilder.hh \
    ScriptLangWrapper/scriptapiqobjectwrapper.hh \
    communication/configurationreader.hh \
    businesslogic.hh \
    configuration.hh \
    confxmlhandler.hh \
    scriptrunnerpool.hh \
    UserInterface/viewinterface.hh \
    UserInterface/consoleui.hh \
    UserInterface/uifactory.hh \
    UserInterface/userinterface.hh

DESTDIR         = ../../../build/bin/
