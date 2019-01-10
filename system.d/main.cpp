#include <stdio.h>
#include <stdlib.h>
#include <QCoreApplication>
#include <QtDBus/QtDBus>
#include "dbus_system.h"
#include <QObject>



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if (!crea_conexionSQLite()){
        fprintf(stderr, "No puedo conectarme a la base de datos.\n");
        return 1;
        }
    GrxDbus_system grx_dbus_system;
    return a.exec();
}
