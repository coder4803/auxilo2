
QT       += core gui

CONFIG += c++11
CONFIG += plugin

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = uigraphic
TEMPLATE = lib

HEADERS += graphicalui.hh \
    graphicaluiplugin.hh

SOURCES += graphicalui.cc \
    graphicaluiplugin.cc

FORMS += graphicalui.ui

DESTDIR = ../../../../../build/plugins/userinterfaces/signalhandler
