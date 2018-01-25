QT              -= gui
QT              += network script

TARGET          = script
TEMPLATE        = lib
CONFIG          += plugin

INCLUDEPATH     += ../ \
                   ../../../core/interface \
                   ../../../utils/messages \
                   ../../../core/plugininterfaces \
                   ../../../core/plugininterfaces/protocol

SOURCES         += script.cpp \
                   coreapi.cpp \
                   communicationapi.cpp \
                   bytearray/bytearrayclass.cpp \
                   bytearray/bytearrayprototype.cpp \
                   parameters/parametersclass.cpp \
                   parameters/parametersprototype.cpp

HEADERS         += script.h \
                   coreapi.h \
                   communicationapi.h \
                   bytearray/bytearrayclass.h \
                   bytearray/bytearrayprototype.h \
                   parameters/parametersclass.h \
                   parameters/parametersprototype.h

LIBS            += -L../../../../build/lib/ -lprotocol \
                   -L../../../build/lib/ -lmessages

DESTDIR         = ../../../../build/bin/plugins/protocol/
