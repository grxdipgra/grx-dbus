#!/bin/bash
set -e
chmod 755 grx-dbus/DEBIAN/*
RUTA=$(pwd)
cd $RUTA/system.d/
qmake
make clean
make
cp grx.dbus $RUTA/grx-dbus/usr/sbin/grx.dbus.service
cd $RUTA/session.d/
qmake
make clean
make
cp grx.dbus $RUTA/grx-dbus/usr/bin/grx.dbus.service
cd $RUTA
dpkg -b grx-dbus
mv grx-dbus.deb ../
