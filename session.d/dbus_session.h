#ifndef GRX_DBUS_SESSION
#define GRX_DBUS_SESSION
#define SERVICE_NAME  "grx.dbus"
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
#include <QFile>
#include <QNetworkProxy>
#include <QNetworkAccessManager>
#include <QAuthenticator>
#include "simplecrypt.h"

class GrxDbus_session: public QObject
{
    Q_OBJECT
public slots:

    Q_SCRIPTABLE bool copias();
    Q_SCRIPTABLE void temporizador(int i);
    Q_SCRIPTABLE QString xauthority();
public:
    explicit GrxDbus_session();
    ~GrxDbus_session();

signals:

    void copias_seguridad(const QString &);

private slots:

    void activaProxy();
    void onProxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *auth);
    void replyFinished(QNetworkReply*);

private:
  QNetworkAccessManager *manager;
 SimpleCrypt *cifra = new SimpleCrypt(Q_UINT64_C(0x0c2ad4c2acb9f023));
};

#endif
