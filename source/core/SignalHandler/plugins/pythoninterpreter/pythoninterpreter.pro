
QT       += core

CONFIG += c++11
CONFIG += plugin

TARGET = pythoninterpreter
TEMPLATE = lib

LIBS += -L../../../../../build/lib/ -lmessages




INCLUDEPATH += ../../ScriptInterpreter \
               ../../../../utils/messages/

DESTDIR = ../../../../../build/plugins/scriptinterpreters

HEADERS += \
    pythoninterpreter.h \
    pythoninterpreterplugin.h \
    scriptapiadapter.h

SOURCES += \
    pythoninterpreterplugin.cpp \
    pythoninterpreter.cpp \
    scriptapiadapter.cpp
