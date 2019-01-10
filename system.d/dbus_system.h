#ifndef GRX_DBUS_SYSTEM
#define GRX_DBUS_SYSTEM
#define SERVICE_NAME  "grx.dbus"
#define CONFINI "/etc/grx/grx-dbus.ini"
#define VELETA "10.1.1.185"
#define REPOSITORIO "10.1.1.91"
#define MYSQLDB "10.7.15.193"
#define BASE_DATOS "/var/lib/grx/grx.sqlite"
#define ARP_SCAN "/usr/sbin/arp-scan -onliip -G 187 -t50 2> /dev/null "
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtCore/QObject>
#include "QtDebug"
#include <QDBusVariant>
#include <QSqlRecord>
#include <QDBusMessage>
#include <QSettings>

class GrxDbus_system: public QObject
{
    Q_OBJECT
public slots:
    Q_SCRIPTABLE QString busca_router(const QString &ip);
    Q_SCRIPTABLE QString busca_routers_muestra_nombres(const QString &ips);
    Q_SCRIPTABLE QList<QVariant> ip_nodos();
    Q_SCRIPTABLE QString ip_nodos_string();
    Q_SCRIPTABLE QList<QVariant> consulta_sql_columna(const QString &tabla,int columna);
    Q_SCRIPTABLE QList<QVariant> consulta_sql_todo(const QString &arg);
    Q_SCRIPTABLE QString busca_todos_routers();
    Q_SCRIPTABLE QString consulta_sql_nombre(const QString &ip);
    Q_SCRIPTABLE QString consulta_sql_ip (const QString &nombre);
    Q_SCRIPTABLE QString busca_nodo_por_ip(const QString &ip);
    Q_SCRIPTABLE QString busca_nodo_por_nombre(const QString &nodo);
    Q_SCRIPTABLE bool esta_nodo_por_nombre(const QString &nodo);
    Q_SCRIPTABLE bool esta_nodo_por_ip(const QString &ip);
    Q_SCRIPTABLE QString crea_conexion (const QString &conexion);
    Q_SCRIPTABLE QString busca_todos_routers_nombre();
    Q_SCRIPTABLE bool esta_veleta();
    Q_SCRIPTABLE bool esta_repositorio();
    Q_SCRIPTABLE bool esta_mysqlDB();

    Q_SCRIPTABLE void guarda_veleta(QString veleta);
    Q_SCRIPTABLE void guarda_repositorio(QString repositorio);
    Q_SCRIPTABLE void guarda_mysqlDB(QString mysqlDB);
    Q_SCRIPTABLE void guarda_baseDatos(QString baseDatos);

    Q_SCRIPTABLE void temporizador();
    Q_SCRIPTABLE void mensaje(const QString &mensaje);
    Q_SCRIPTABLE bool iptables_parar();
    Q_SCRIPTABLE bool iptables_arrancar();
    Q_SCRIPTABLE QByteArray iptables_estado();
    Q_SCRIPTABLE bool iptables_reiniciar();
    Q_SCRIPTABLE QList<QVariant> iptables_listar_excepciones();
    Q_SCRIPTABLE bool iptables_anadir_excepcion(const QString &excepcion);
    Q_SCRIPTABLE bool iptables_borrar_excepcion(const QString &excepcion);

    Q_SCRIPTABLE void cambio_tiempo(int tmp);

public:
    explicit GrxDbus_system();
    ~GrxDbus_system();
    int tiempo = 6000; //usado para mandar las señales en ese tiempo (milisegundos)

signals:

    void esta_repositorio(const QString &);
    void esta_veleta(const QString &);
    void esta_mysqldb(const QString &);


private:
    QSqlDatabase db;
    QSqlQuery consultar;
    QString veleta=VELETA;
    QString repositorio=REPOSITORIO;
    QString mysqlDB=MYSQLDB;
    QString baseDatos=BASE_DATOS;

private slots:
 void carga_variables();

};


static bool crea_conexionSQLite()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(BASE_DATOS);
    if (!db.open()) {
       qDebug() << ("No puedo abrir la base de datos\n");
       return false;
    }

   return true;
}



#endif
