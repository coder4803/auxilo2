
QT       += core script

CONFIG += c++11
CONFIG += plugin

TARGET = qtscriptinterpreter
TEMPLATE = lib

LIBS += -L../../../../../build/lib/ -lmessages

HEADERS += qtscriptinterpreter.hh \
           scriptapiadapter.hh \
    qtscriptinterpreterplugin.hh

SOURCES += qtscriptinterpreter.cc \
           scriptapiadapter.cc \
    qtscriptinterpreterplugin.cc

INCLUDEPATH += ../../ScriptInterpreter \
               ../../../../utils/messages/

DESTDIR = ../../../../../build/plugins/scriptinterpreters
