#include <stdio.h>
#include <stdlib.h>
#include <QCoreApplication>
#include <QtDBus/QtDBus>
#include <QList>
#include "dbus_session.h"

GrxDbus_session::GrxDbus_session()//CONSTRUCTOR

{
    if (!QDBusConnection::sessionBus().isConnected()) {
            fprintf(stderr, "No puedo conectarme al bus de sesion del sistema.\n");
            qApp->exit();
        }
    if (!QDBusConnection::sessionBus().registerService(SERVICE_NAME)) {
            fprintf(stderr, "%s\n",
                    qPrintable(QDBusConnection::sessionBus().lastError().message()));
            qApp->exit();
        }
    QDBusConnection::sessionBus().registerObject("/", this, QDBusConnection::ExportAllSlots);
    QDBusConnection::sessionBus().registerObject("/senales", this,QDBusConnection::ExportAllSignals);

    //Conectamos al bus del sistema
    if (!QDBusConnection::systemBus().isConnected()) {
            fprintf(stderr, "No puedo conectarme al bus del sistema.\n");
            qApp->exit();
    } else
        if (!QDBusConnection::systemBus().connect(SERVICE_NAME,"","grx.dbus","esta_veleta",this,SLOT(copias()))){
            fprintf(stderr, "No puedo conectarme al bus de sistema desde grx-session.\n");
        }

    //Activamos el proxy
    manager = new QNetworkAccessManager(this);
    QNetworkProxy proxy(QNetworkProxy::HttpCachingProxy, "195.57.47.66", 8080);
    manager->setProxy(proxy);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    connect(manager, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy&, QAuthenticator*)),this , SLOT(onProxyAuthenticationRequired(const QNetworkProxy&, QAuthenticator*)));

    setenv("http_proxy","http://195.57.47.66:8080",1);
    setenv("https_proxy","http://195.57.47.66:8080",1);

    this->temporizador(5000);
}
GrxDbus_session::~GrxDbus_session(){
}

void GrxDbus_session::replyFinished(QNetworkReply*)
{

}

void GrxDbus_session::onProxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *auth)
{

    QString fichero=QDir::homePath()+"/.grx/.proxy";
    QString nombre,clave;
    QFile file(fichero);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString tmp_nombre = in.readLine();
        nombre = cifra->decryptToString(QString(tmp_nombre));
        QString tmp_clave = in.readLine();
        clave = cifra->decryptToString(QString(tmp_clave));
    }
    auth->setUser(QString(nombre+"@grx").toUtf8() ) ;
    auth->setPassword(clave);
}

void GrxDbus_session::activaProxy()
{
     manager->get(QNetworkRequest(QUrl("http://www.gnu.org/")));
}


QString GrxDbus_session::xauthority(){
    QString tmp="";
    QFile file(QDir::homePath()+"/.Xauthority");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        tmp = in.readLine();
    }
return tmp;
}

bool GrxDbus_session::copias(){
    //Aqui ponemos lo que queremos hacer cuando la señal esta_veleta se active
    //Yo por ejemplo escribo en un fichero "señal esta_veleta activa" cada vez que se emitia una señal.


    QFile f(QDir::homePath()+"copias");
    if (f.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream out(&f);
        out << "copias activado" << "\n";
        return true;
    }
return false;
}

void GrxDbus_session::temporizador(int i){
    QTimer *timer_client = new QTimer(this);
    connect(timer_client, SIGNAL(timeout()), this, SLOT(activaProxy()));
    timer_client->start(i);
}


