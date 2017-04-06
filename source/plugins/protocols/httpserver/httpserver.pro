QT              -= gui

TARGET          = httpserver
TEMPLATE        = lib
CONFIG          += plugin

INCLUDEPATH     += ../ \
                   ../../../core/interface \
                   ../../../utils/messages \
                   ../../../core/plugininterfaces \
                   ../../../core/plugininterfaces/protocol

SOURCES         += httpserver.cpp \
                   httpmessage.cpp \
                   httprequest.cpp \
    httpreply.cpp

HEADERS         += httpserver.h \
                   httpmessage.h \
                   httprequest.h \
    httpreply.h

LIBS            += -L../../../../build/lib/ -lprotocol \
                   -L../../../build/lib/ -lmessages

DESTDIR         = ../../../../build/bin/plugins/protocol/
