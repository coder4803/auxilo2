TEMPLATE    = lib
CONFIG      += staticlib
QT          -= gui
QT          += network

SOURCES     += messagegroup.cpp
SOURCES     += connection/connection.cpp

HEADERS     += messagegroup.h
HEADERS     += connection/connection.h
HEADERS     += globals.h

INCLUDEPATH += ../../../externals/include/

DESTDIR     = ../../../build/lib/
