QT              -= gui
QT              += network

TARGET          = orvibosockets20
TEMPLATE        = lib
CONFIG          += plugin

INCLUDEPATH     += ../../../core/interface \
                   ../../../utils/messages \
                   ../../../core/plugininterfaces \
                   ../../../core/plugininterfaces/protocol

SOURCES         += orvibosockets20plugin.cpp \
                   orvibosockets20.cpp

HEADERS         += orvibosockets20plugin.h \
                   orvibosockets20.h

LIBS            += -L../../../../build/lib/ -lprotocol

DESTDIR         = ../../../../build/bin/plugins/protocol/
