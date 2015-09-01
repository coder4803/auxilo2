QT                  += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET              = messagetool
TEMPLATE            = app
CONFIG              += c++11

INCLUDEPATH         += ../../../externals/include/ \
                       ../../utils/messagegroup/ \
                       ../../utils/messages/ \
                       ./messagewidgets

SOURCES             += main.cpp \
                       mainwindow.cpp \
                       messageviewer.cpp \
                       messagesender.cpp \
                       groupmodel.cpp \
                       devicenameloader.cpp \
                       messagemodel.cpp \
                       detailmodel.cpp \
                       messagefilterproxymodel.cpp \
    messagewidgets/signalmessagewidget.cpp \
    messagewidgets/messagewidgetpool.cpp \
    messagewidgets/confresponsemessagewidget.cpp

HEADERS             += mainwindow.h \
                       messageviewer.h \
                       messagesender.h \
                       groupmodel.h \
                       devicenameloader.h \
                       messagemodel.h \
                       detailmodel.h \
                       globals.h \
                       messagefilterproxymodel.h \
    messagewidgets/messagewidget.h \
    messagewidgets/signalmessagewidget.h \
    messagewidgets/messagewidgetpool.h \
    messagewidgets/confresponsemessagewidget.h

LIBS                += -L../../../build/lib/ -lmessages \
                       -L../../../build/lib/ -lmessagegroup

DESTDIR             = ../../../build/bin/
