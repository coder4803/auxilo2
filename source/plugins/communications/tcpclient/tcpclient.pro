QT              -= gui
QT              += network

TARGET          = tcpclient
TEMPLATE        = lib
CONFIG          += plugin

INCLUDEPATH     += ../../../core/interface \
                   ../../../utils/messages \
                   ../../../core/plugininterfaces \
                   ../../../core/plugininterfaces/communication

SOURCES         += tcpclientplugin.cpp \
                   tcpclient.cpp

HEADERS         += tcpclientplugin.h \
                   tcpclient.h

LIBS            += -L../../../../build/lib/ -lcommunication \
                   -L../../../../build/lib/ -lmessages

DESTDIR         = ../../../../build/bin/plugins/communication/
