QT              -= gui
QT              += network

TARGET          = tcpclient
TEMPLATE        = lib
CONFIG          += plugin

INCLUDEPATH     += ../ \
                   ../../../core/interface \
                   ../../../utils/messages \
                   ../../../core/plugininterfaces \
                   ../../../core/plugininterfaces/communication

SOURCES         += tcpclient.cpp

HEADERS         += tcpclient.h

LIBS            += -L../../../../build/lib/ -lcommunication \
                   -L../../../../build/lib/ -lmessages

DESTDIR         = ../../../../build/bin/plugins/communication/
