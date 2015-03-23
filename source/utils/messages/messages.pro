TEMPLATE    = lib
CONFIG      += staticlib
QT          -= gui

SOURCES     += message.cpp \
               signalmessage.cpp \
               signalackmessage.cpp \
               setstatemessage.cpp \
               setstateackmessage.cpp

HEADERS     += message.h \
               signalmessage.h \
               signalackmessage.h \
               setstatemessage.h \
               setstateackmessage.h

INCLUDEPATH += ../../../externals/include/

DESTDIR     = ../../../build/lib/
