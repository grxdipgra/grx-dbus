#include <stdio.h>
#include <stdlib.h>
#include <QCoreApplication>
#include <QtDBus/QtDBus>
#include "dbus_session.h"
#include <QObject>



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    GrxDbus_session grx_dbus_session;
    return a.exec();
}
