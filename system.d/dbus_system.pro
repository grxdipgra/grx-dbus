QT += core dbus sql network
QT -= gui

CONFIG += c++11

TARGET = grx.dbus
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app
SOURCES += main.cpp \
           dbus_system.cpp


HEADERS += dbus_system.h
