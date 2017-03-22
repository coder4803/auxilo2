QT              -= gui
QT              += network

TARGET          = udp
TEMPLATE        = lib
CONFIG          += plugin

INCLUDEPATH     += ../ \
                   ../../../core/interface \
                   ../../../utils/messages \
                   ../../../core/plugininterfaces \
                   ../../../core/plugininterfaces/communication

SOURCES         += udp.cpp

HEADERS         += udp.h

LIBS            += -L../../../../build/lib/ -lcommunication \
                   -L../../../../build/lib/ -lmessages

DESTDIR         = ../../../../build/bin/plugins/communication/
