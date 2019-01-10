QT += core dbus sql network
QT -= gui

CONFIG += c++11

TARGET = grx.dbus
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app
SUBDIRS = session.d 
SOURCES += main.cpp \
           dbus_session.cpp \
           simplecrypt.cpp
           
HEADERS += dbus_session.h \
           simplecrypt.h
