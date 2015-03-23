TEMPLATE    = lib
CONFIG      += staticlib
QT          -= gui

SOURCES     += message.cpp \
               signalmessage.cpp \
               signalackmessage.cpp \
               setstatemessage.cpp \
               setstateackmessage.cpp \
               requeststatemessage.cpp \
               stateresponsemessage.cpp

HEADERS     += message.h \
               signalmessage.h \
               signalackmessage.h \
               setstatemessage.h \
               setstateackmessage.h \
               requeststatemessage.h \
               stateresponsemessage.h

INCLUDEPATH += ../../../externals/include/

DESTDIR     = ../../../build/lib/
