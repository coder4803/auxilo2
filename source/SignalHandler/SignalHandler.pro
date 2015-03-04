#-------------------------------------------------
#
# Project created by QtCreator 2015-03-04T21:45:11
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = SignalHandler
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app


SOURCES += main.cc \
    signal.cc \
    signalparser.cc

HEADERS += \
    signal.hh \
    signalparser.hh
