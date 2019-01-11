#!/bin/bash
set -e
RUTA=$(pwd)
cd $RUTA/system.d/
make clean
make
cp grx.dbus $RUTA/grx-dbus/usr/sbin/grx.dbus.service
cd $RUTA/session.d/
make clean
make
cp grx.dbus $RUTA/grx-dbus/usr/bin/grx.dbus.service
cd $RUTA
dpkg -b grx-dbus
mv grx-dbus.deb ../
