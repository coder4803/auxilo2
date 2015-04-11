TEMPLATE    = lib
CONFIG      += staticlib
QT          -= gui
QT          += network

SOURCES     += messagegroup.cpp
SOURCES     += connection/connection.cpp

HEADERS     += messagegroup.h
HEADERS     += connection/connection.h

INCLUDEPATH += ../../../externals/include/

LIBS        += -L$$PWD/../../../externals/lib/ -lqamqp

DESTDIR     = ../../../build/lib/
