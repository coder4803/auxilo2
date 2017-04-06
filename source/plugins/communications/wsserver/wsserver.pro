QT              -= gui
QT              += websockets

TARGET          = wsserver
TEMPLATE        = lib
CONFIG          += plugin

INCLUDEPATH     += ../ \
                   ../../../core/interface \
                   ../../../utils/messages \
                   ../../../core/plugininterfaces \
                   ../../../core/plugininterfaces/communication

SOURCES         += wsserver.cpp

HEADERS         += wsserver.h

LIBS            += -L../../../../build/lib/ -lcommunication

DESTDIR         = ../../../../build/bin/plugins/communication/
