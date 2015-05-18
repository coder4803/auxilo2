QT              -= gui

TARGET          = protocol
TEMPLATE        = lib
CONFIG          += staticlib

INCLUDEPATH     += ../../../utils/messages/ \
                   ../communication/

SOURCES         += protocol.cpp

HEADERS         += protocol.h

DESTDIR         = ../../../../build/lib/
