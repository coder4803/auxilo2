QT              -= gui
QT              += network

TARGET          = tcpserver
TEMPLATE        = lib
CONFIG          += plugin

INCLUDEPATH     += ../ \
                   ../../../core/interface \
                   ../../../utils/messages \
                   ../../../core/plugininterfaces \
                   ../../../core/plugininterfaces/communication

SOURCES         += tcpserver.cpp

HEADERS         += tcpserver.h

LIBS            += -L../../../../build/lib/ -lcommunication

DESTDIR         = ../../../../build/bin/plugins/communication/
