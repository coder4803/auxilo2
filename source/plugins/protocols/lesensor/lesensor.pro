QT              -= gui
QT              += network

TARGET          = lesensor
TEMPLATE        = lib
CONFIG          += plugin

INCLUDEPATH     += ../../../core/interface \
                   ../../../utils/messages \
                   ../../../core/plugininterfaces \
                   ../../../core/plugininterfaces/protocol

SOURCES         += lesensorplugin.cpp \
                   lesensor.cpp

HEADERS         += lesensorplugin.h \
                   lesensor.h

LIBS            += -L../../../../build/lib/ -lprotocol

DESTDIR         = ../../../../build/bin/plugins/protocol/
