#-------------------------------------------------
#
# Project created by QtCreator 2015-03-04T21:45:11
#
#-------------------------------------------------

QT       += core xml network

QT       -= gui widgets

TARGET = SignalHandler
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

INCLUDEPATH     += ../../utils/messagegroup/ \
                   ../../utils/messages/ \
                   /ScriptInterpreter \
                   /exceptions \
                   /interfaces \
                   /communication \



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
    communication/signalreader.cc \
    scriptrunner.cc \
    ScriptInterpreter/scriptapiimplementation.cc \
    signalhandlerbuilder.cc \
    communication/configurationreader.cc \
    businesslogic.cc \
    confxmlhandler.cc \
    scriptrunnerpool.cc \
    UserInterface/consoleui.cc \
    UserInterface/userinterface.cc \ 
    ScriptInterpreter/scriptinterpreterfactory.cc \
    ScriptInterpreter/scriptinterpreterpool.cc

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
    communication/signalreader.hh \
    scriptrunner.hh \
    ScriptInterpreter/scriptrunexceptions.hh \
    ScriptInterpreter/scriptapi.hh \
    ScriptInterpreter/scriptapiimplementation.hh \
    interfaces/modelinterface.hh \
    signalhandlerbuilder.hh \
    communication/configurationreader.hh \
    businesslogic.hh \
    configuration.hh \
    confxmlhandler.hh \
    scriptrunnerpool.hh \
    UserInterface/viewinterface.hh \
    UserInterface/consoleui.hh \
    UserInterface/userinterface.hh \
    UserInterface/uiplugin.hh \
    ScriptInterpreter/interpreterplugin.hh \
    ScriptInterpreter/scriptinterpreter.hh \
    ScriptInterpreter/scriptinterpreterfactory.hh \
    ScriptInterpreter/scriptinterpreterpool.hh

DESTDIR         = ../../../build/bin/

