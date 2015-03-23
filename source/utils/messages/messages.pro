TEMPLATE    = lib
CONFIG      += staticlib
QT          -= gui

SOURCES     += message.cpp \
               signalmessage.cpp \
               signalackmessage.cpp

HEADERS     += message.h \
               signalmessage.h \
               signalackmessage.h

INCLUDEPATH += ../../../externals/include/

DESTDIR     = ../../../build/lib/
