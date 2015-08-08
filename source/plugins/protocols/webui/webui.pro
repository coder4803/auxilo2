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

LIBS            += -L../../../../build/lib/ -lprotocol \
                   -L../../../build/lib/ -lmessages

DESTDIR         = ../../../../build/bin/plugins/protocol/