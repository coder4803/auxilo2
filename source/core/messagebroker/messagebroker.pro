QT              += network
QT              -= gui

TARGET          = messagebroker

TEMPLATE        = app
CONFIG          += console

INCLUDEPATH     += ../../utils/messagegroup/

SOURCES         += main.cpp \
                   messagebroker.cpp \
                   group.cpp \
                   connection.cpp

HEADERS         += interface.h \
                   messagebroker.h \
                   group.h \
                   connection.h

DESTDIR         = ../../../build/bin/
