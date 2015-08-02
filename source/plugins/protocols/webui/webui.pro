QT              -= gui
QT              += network

TARGET          = webui
TEMPLATE        = lib
CONFIG          += plugin

INCLUDEPATH     += ../../../core/interface \
                   ../../../utils/messages \
                   ../../../core/plugininterfaces \
                   ../../../core/plugininterfaces/protocol

SOURCES         += webuiplugin.cpp \
                   webui.cpp

HEADERS         += webuiplugin.h \
                   webui.h

LIBS            += -L../../../../build/lib/ -lprotocol

DESTDIR         = ../../../../build/bin/plugins/protocol/
