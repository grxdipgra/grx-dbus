#include <stdio.h>
#include <stdlib.h>
#include <QCoreApplication>
#include <QtDBus/QtDBus>
#include <QList>
#include "dbus_system.h"
#include <QHostAddress>

int valida_ip(const QString &ip){
    QHostAddress myIP;
    QChar asterisco = QLatin1Char ('*');
    QChar punto = QLatin1Char('.');

    if (!ip.isEmpty()){
        if (( (ip.at(ip.count()-1)) == asterisco) && (ip.at(ip.count()-2))==punto)
            return true;
        if( myIP.setAddress(ip)&& (ip.count(punto)==3))
                return true;
    }

return false;
}

QString arp_scan(const QString &arg)
{
       QProcess process;
       process.start(arg);
       process.waitForFinished(-1);
       return process.readAllStandardOutput();
}

QList<QVariant> ipsNodos()
{
       QString consulta;
       QList<QVariant> vector;
       consulta.append("select ipLinea from NODO");
       QSqlQuery consultar;
       if (consultar.exec(consulta)){
           while (consultar.next()){
               vector.append(consultar.value(0).toString());
           }
       }
       else{
          vector.append("Error No se ha podido realizar la consulta "+consultar.lastError().text());
       }
       return vector;
}

QString ipsNodosString()
{
       QString vector;
       QSqlQuery consultar;
       if (consultar.exec("select ipLinea from NODO")){
           while (consultar.next()){
               vector.append(consultar.value(0).toString()+" ");
           }
       }
       else{
            vector.append("Error No se ha podido realizar la consulta "+consultar.lastError().text());
       }
       return vector;
}

QString GrxDbus_system::busca_router(const QString &ip)
{
       return arp_scan( ARP_SCAN + ip);
}


GrxDbus_system::GrxDbus_system()//CONSTRUCTOR

{
    carga_variables();
    if (!QDBusConnection::systemBus().isConnected()) {
            fprintf(stderr, "No puedo conectarme al bus del sistema.\n");
            qApp->exit();
        }
    if (!QDBusConnection::systemBus().registerService(SERVICE_NAME)) {
            fprintf(stderr, "%s\n",
                    qPrintable(QDBusConnection::systemBus().lastError().message()));
            qApp->exit();
        }
    QDBusConnection::systemBus().registerObject("/", this, QDBusConnection::ExportAllSlots);
    QDBusConnection::systemBus().registerObject("/senales", this,QDBusConnection::ExportAllSignals);
    this->temporizador();
}

GrxDbus_system::~GrxDbus_system(){

}


QString GrxDbus_system::crea_conexion (const QString &conexion)
{
   return conexion;
}


void GrxDbus_system::carga_variables(){

    QSettings s(CONFINI, QSettings::IniFormat);
    veleta = s.value("Configuracion/veleta").toString();
    repositorio = s.value("Configuracion/repositorio").toString();
    mysqlDB = s.value("Configuracion/mysqlDB").toString();
    tiempo = s.value("Configuracion/tiempo").toInt();
}

void GrxDbus_system::guarda_veleta(QString veleta){

    QSettings s(CONFINI, QSettings::IniFormat);
    s.setValue("Configuracion/veleta", veleta);
}
void GrxDbus_system::guarda_repositorio(QString repositorio){

    QSettings s(CONFINI, QSettings::IniFormat);
    s.setValue("Configuracion/repositorio", repositorio);
}
void GrxDbus_system::guarda_mysqlDB(QString mysqlDB){

    QSettings s(CONFINI, QSettings::IniFormat);
    s.setValue("Configuracion/mysqlDB", mysqlDB);
}
void GrxDbus_system::guarda_baseDatos(QString baseDatos){

    QSettings s(CONFINI, QSettings::IniFormat);
    s.setValue("Configuracion/baseDatos", baseDatos);
}


QString GrxDbus_system::busca_routers_muestra_nombres(const QString &ips)
{
       QString arp;
       arp = arp_scan(ARP_SCAN + ips);
       QStringList nodoList = arp.split('\n');
       nodoList.removeAll({}); //Quitamos vacios
       QString nombre_nodo;
       for (int i = 0; i < nodoList.size(); ++i){
            QString nombres, tmp;
            tmp = nodoList.value(i);
            nombres = tmp.remove(QRegExp("[\\t]"));
            if (i < nodoList.size()-1){
               nombre_nodo.append(consulta_sql_nombre(nombres)+"|");
            }
            else{
               nombre_nodo.append(consulta_sql_nombre(nombres));
            }

       }
return nombre_nodo;
}

QList<QVariant> GrxDbus_system::ip_nodos(){

       return ipsNodos();

}

QString GrxDbus_system::ip_nodos_string(){

    return ipsNodosString();

}

QString GrxDbus_system::busca_todos_routers_nombre()
{

    return consulta_sql_nombre(busca_todos_routers());

}

QString GrxDbus_system::busca_todos_routers()
{
        QString vector;
        QString arp , tmp;
        int i;
        vector=ipsNodosString();
        tmp = arp_scan(ARP_SCAN + vector);
        i = tmp.indexOf("\t"); //Quitamos \t y \n de la cadena
        arp = tmp.mid(0,i);
return arp;
}

//Realiza la consulta de una columna de una tabla
QList<QVariant> GrxDbus_system::consulta_sql_columna(const QString &tabla,int columna)
{
    QSqlQuery consultar;
    QList<QVariant> vector;

    consultar.prepare(QString("select * from "+tabla));

    if (consultar.exec()){
       while (consultar.next()){
          vector.append(consultar.value(columna).toString());
       }
    } else {
           vector.append("Error No se ha podido realizar la consulta "+consultar.lastError().text());
    }

return vector;
}

QString GrxDbus_system::consulta_sql_nombre (const QString &ip){
    QSqlQuery consultar;
    QString resultado;
    consultar.prepare(QString("select nombre from NODO where ipLinea = :ip"));
    consultar.bindValue(":ip", ip);
    if (consultar.exec() and consultar.first()){
            resultado.append(consultar.value(0).toString().toUtf8());

    } else {
           resultado.append("Nodo no encontrado en la DB "+consultar.lastError().text());
    }
return resultado;
}

QString GrxDbus_system::consulta_sql_ip (const QString &nombre){
    QSqlQuery consultar;
    QString resultado;
    consultar.prepare(QString("select ipLinea from NODO where nombre = :nombre"));
    consultar.bindValue(":nombre", nombre);
    if (consultar.exec() and consultar.first()){
            resultado.append(consultar.value(0).toString());

    } else {
          resultado.append("Error No se ha podido realizar la consulta "+consultar.lastError().text());
    }
return resultado;
}

QString GrxDbus_system::busca_nodo_por_ip(const QString &ip)
{
    QString arp, ips_encontradas;
    arp = arp_scan(ARP_SCAN + ip);
    QStringList nodoList = arp.split('\n');
    nodoList.removeAll({});
    for (int i = 0; i < nodoList.size(); ++i){
         QString nombre, tmp;
         tmp = nodoList.value(i);
         nombre = tmp.remove(QRegExp("[\\t]"));
         if (i < nodoList.size()-1){
            ips_encontradas.append(nombre+"|");
         }
         else{
            ips_encontradas.append(nombre);
         }
    }
    return ips_encontradas;
}

bool GrxDbus_system::esta_nodo_por_ip(const QString &ip)
{
    QString arp;
    arp = arp_scan(ARP_SCAN + ip);
    QStringList nodoList = arp.split('\n').first().split('\t'); //arp devuelve algo parecido a esto "192.168.1.1\tdd:dd:dd:dd:dd:dd"

    if (ip == nodoList.first()){
        return true;
    } else {
        return false;
    }

}

QString GrxDbus_system::busca_nodo_por_nombre(const QString &nodo)
{
    QString arp;
    QString ip_nodo;
    ip_nodo = consulta_sql_ip(nodo);
    qDebug() << ip_nodo;
    arp = arp_scan(ARP_SCAN + ip_nodo);
    QStringList nodoList = arp.split('\n').first().split('\t'); //arp devuelve algo parecido a esto "192.168.1.1\tdd:dd:dd:dd:dd:dd"
    return nodoList.first();//con esto cogemos solo la ip


}

void GrxDbus_system::mensaje(const QString &mensaje){
    qDebug()<< "Este es el mensaje"<< mensaje;
}

bool GrxDbus_system::esta_nodo_por_nombre(const QString &nodo)
{
    QString arp;
    QString ip_nodo;
    ip_nodo = consulta_sql_ip(nodo);
    arp = arp_scan(ARP_SCAN + ip_nodo);
    QStringList nodoList = arp.split('\n').first().split('\t');
    if (ip_nodo == nodoList.first()){
        return true;
    } else {
        return false;
    }
}

bool GrxDbus_system::esta_repositorio(){
    QProcess process;
    process.start("ping -c1 -w1 "+QString(repositorio));
    process.waitForFinished(-1);
    if (!process.exitCode()){
        QDBusMessage message = QDBusMessage::createSignal("/",SERVICE_NAME, "esta_repositorio");
        message << true;
        QDBusConnection::systemBus().send(message);

    }
    /*else
    {
        QDBusMessage message = QDBusMessage::createSignal("/",SERVICE_NAME, "esta_repositorio");
        message << false;
        QDBusConnection::systemBus().send(message);

    }*/

    return !process.exitCode();
}

bool GrxDbus_system::esta_veleta(){
    QProcess process;
    process.start("ping -c1 -w1 "+QString(veleta));
    process.waitForFinished(-1);
    if (!process.exitCode()){
        QDBusMessage message = QDBusMessage::createSignal("/",SERVICE_NAME, "esta_veleta");
        message << true;
        QDBusConnection::systemBus().send(message);

    }
return !process.exitCode();
}

bool GrxDbus_system::esta_mysqlDB(){
    QProcess process;
    process.start("ping -c1 -w1 "+QString(mysqlDB));
    process.waitForFinished(-1);
    if (!process.exitCode()){
        QDBusMessage message = QDBusMessage::createSignal("/",SERVICE_NAME, "esta_mysql");
        message << true;
        QDBusConnection::systemBus().send(message);

    }
    return !process.exitCode();
}

QList<QVariant> GrxDbus_system::consulta_sql_todo(const QString &sql)
{
    int col,i;
    QSqlQuery consultar;
    QList<QVariant> vector;
    if (consultar.exec(sql)){
       QSqlRecord record = consultar.record();
       col=record.count();
       while (consultar.next()){
          for (i=0; i< col;i++){
                vector.append(consultar.value(i).toString());
          }
       }
    }else{
          vector.append("Error No se ha podido realizar la consulta "+consultar.lastError().text());
    }
return vector;
}

bool GrxDbus_system::iptables_anadir_excepcion(const QString &excepcion){
    if (valida_ip(excepcion)){
        QSqlQuery consultar;
        consultar.prepare(QString("INSERT INTO excepciones (ip) VALUES (:excepcion)"));
        consultar.bindValue(":excepcion", excepcion);
        if (consultar.exec()){
            return iptables_reiniciar();
        }
    }
return 0;
}

bool GrxDbus_system::iptables_borrar_excepcion(const QString &excepcion){
    if (valida_ip(excepcion)){
        QSqlQuery consultar;
        consultar.prepare(QString("DELETE FROM excepciones WHERE ip =:excepcion"));
        consultar.bindValue(":excepcion", excepcion);
        if (consultar.exec()){
            return iptables_reiniciar();
        }
    }
return 0;
}
QList<QVariant> GrxDbus_system::iptables_listar_excepciones(){

   QSqlQuery consultar;
   QList<QVariant> vector;
   consultar.prepare(QString("select ip from excepciones"));
   if (consultar.exec()){
       while (consultar.next()){
          vector.append(consultar.value(0).toString());
       }
    } else {
           vector.append("Error No se ha podido realizar la consulta "+consultar.lastError().text());
    }

return vector;
}







bool GrxDbus_system::iptables_parar(){
    QProcess process;
    process.start("service grx.iptables stop");
    process.waitForFinished(-1);
    if (!process.exitCode()){
        return true;
    }else{
        return false;
    }
}

bool GrxDbus_system::iptables_arrancar(){
    QProcess process;
    process.start("service grx.iptables start");
    process.waitForFinished(-1);
    if (!process.exitCode()){
        return true;
    }else{
        return false;
    }
}

QByteArray GrxDbus_system::iptables_estado(){
    QProcess process;
    process.start("iptables -L");
    process.waitForFinished(-1);
    if (!process.exitCode()){
        return process.readAllStandardOutput();
    }
return NULL;
}

bool GrxDbus_system::iptables_reiniciar(){
    QProcess process;
    process.start("service grx.iptables restart");
    process.waitForFinished(-1);
    if (!process.exitCode()){
        return true;
    }else{
        return false;
    }

}

void GrxDbus_system::cambio_tiempo(int tiempo){
    /*bool estado;
    tiempo = tmp;
    estado=disconnect(this,0,0,0);
    temporizador();
    */

    QSettings s(CONFINI, QSettings::IniFormat);
    s.setValue("Configuracion/tiempo", tiempo);
}

void GrxDbus_system::temporizador(){


    QTimer *timer_estamysqlDB = new QTimer(this);
    connect(timer_estamysqlDB, SIGNAL(timeout()), this, SLOT(esta_mysqlDB()));
    timer_estamysqlDB->start(tiempo);

    QTimer *timer_estaveleta = new QTimer(this);
    connect(timer_estaveleta, SIGNAL(timeout()), this, SLOT(esta_veleta()));
    timer_estaveleta->start(tiempo);

    QTimer *timer_estarepositorio = new QTimer(this);
    connect(timer_estarepositorio, SIGNAL(timeout()), this, SLOT(esta_repositorio()));
    timer_estarepositorio->start(tiempo);


}


