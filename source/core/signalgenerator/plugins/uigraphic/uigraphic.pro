
QT += core gui sql

CONFIG += plugin c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = uigraphic
TEMPLATE = lib

DESTDIR = ../../../../../build/plugins/userinterfaces/signalgenerator

FORMS += \
    mainwindow.ui \
    tasklistdialog.ui

HEADERS += \
    mainwindow.h \
    signalgeneratorgraphicuiplugin.h \
    tasklistdialog.h

INCLUDEPATH += ../../

SOURCES += \
    mainwindow.cpp \
    signalgeneratorgraphicuiplugin.cpp \
    tasklistdialog.cpp
