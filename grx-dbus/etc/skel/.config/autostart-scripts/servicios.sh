#/bin/bash

# Esto lo usamos para arrancar el servicio de session dbus

dbus-send --session --dest=grx.dbus --type=method_call --print-reply / org.freedesktop.DBus.Introspectable.Introspect

