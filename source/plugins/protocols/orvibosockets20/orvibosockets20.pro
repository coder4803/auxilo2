QT              -= gui

TARGET          = orvibosockets20
TEMPLATE        = lib
CONFIG          += plugin

INCLUDEPATH     += ../ \
                   ../../../core/interface \
                   ../../../utils/messages \
                   ../../../core/plugininterfaces \
                   ../../../core/plugininterfaces/protocol

SOURCES         += orvibosockets20.cpp

HEADERS         += orvibosockets20.h

LIBS            += -L../../../../build/lib/ -lprotocol \
                   -L../../../build/lib/ -lmessages

DESTDIR         = ../../../../build/bin/plugins/protocol/
