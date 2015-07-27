QT              -= gui
QT              += network

TARGET          = interface

TEMPLATE        = app
CONFIG          += console

INCLUDEPATH     += ../../../externals/include/ \
                   ../../utils/messagegroup/ \
                   ../../utils/messages/ \
                   ../../core/plugininterfaces/ \
                   ../../core/plugininterfaces/communication/ \
                   ../../core/plugininterfaces/protocol/

SOURCES         += main.cpp \
                   interface.cpp \
                   device.cpp

HEADERS         += interface.h \
                   device.h

LIBS            += -L../../../build/lib/ -lmessages \
                   -L../../../build/lib/ -lmessagegroup \
                   -L../../../build/lib/ -lcommunication \
                   -L../../../build/lib/ -lprotocol

DESTDIR         = ../../../build/bin/
