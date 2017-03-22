QT              -= gui
QT              += network

TARGET          = lesensor
TEMPLATE        = lib
CONFIG          += plugin

INCLUDEPATH     += ../ \
                   ../../../core/interface \
                   ../../../utils/messages \
                   ../../../core/plugininterfaces \
                   ../../../core/plugininterfaces/protocol

SOURCES         += lesensor.cpp

HEADERS         += lesensor.h

LIBS            += -L../../../../build/lib/ -lprotocol \
                   -L../../../build/lib/ -lmessages

DESTDIR         = ../../../../build/bin/plugins/protocol/
