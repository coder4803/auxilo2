QT              -= gui
QT              += network
QT              += sql
QT              += xml

TARGET          = stateholder

TEMPLATE        = app
CONFIG          += console

INCLUDEPATH     += ../../../externals/include/ \
                   ../../utils/messagegroup/ \
                   ../../utils/messages/

SOURCES         += main.cpp \
                   stateholder.cpp \
                   stateloader.cpp \
                   states/state.cpp \
                   states/booleanstate.cpp \
                   states/integerstate.cpp

HEADERS         += stateholder.h \
                   stateloader.h \
                   states/state.h \
                   states/booleanstate.h \
                   states/integerstate.h

LIBS            += -L../../../build/lib/ -lmessages \
                   -L../../../build/lib/ -lmessagegroup

DESTDIR         = ../../../build/bin/
