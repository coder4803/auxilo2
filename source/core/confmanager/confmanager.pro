QT              -= gui
QT              += network
QT              += xml

TARGET          = confmanager

TEMPLATE        = app
CONFIG          += console

INCLUDEPATH     += ../../../externals/include/ \
                   ../../utils/messagegroup/ \
                   ../../utils/messages/

SOURCES         += main.cpp \
                   confmanager.cpp \
                   confreader.cpp

HEADERS         += confmanager.h \
                   confreader.h

LIBS            += -L../../../build/lib/ -lmessages \
                   -L../../../build/lib/ -lmessagegroup \
                   -L$$PWD/../../../externals/lib/ -lqamqp

DESTDIR         = ../../../build/bin/
