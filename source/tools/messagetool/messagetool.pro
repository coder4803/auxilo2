QT                  += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET              = messagetool
TEMPLATE            = app

INCLUDEPATH         += ../../../externals/include/ \
                       ../../utils/messagegroup/ \
                       ../../utils/messages/

SOURCES             += main.cpp \
                       mainwindow.cpp \
                       messageviewer.cpp \
                       messagesender.cpp \
                       groupmodel.cpp \
                       devicenameloader.cpp \
                       messagemodel.cpp \
                       detailmodel.cpp \
                       messagefilterproxymodel.cpp

HEADERS             += mainwindow.h \
                       messageviewer.h \
                       messagesender.h \
                       groupmodel.h \
                       devicenameloader.h \
                       messagemodel.h \
                       detailmodel.h \
                       globals.h \
                       messagefilterproxymodel.h

LIBS                += -L../../../build/lib/ -lmessages \
                       -L../../../build/lib/ -lmessagegroup

DESTDIR             = ../../../build/bin/
