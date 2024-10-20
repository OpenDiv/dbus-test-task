#ifndef SYSTEMPERMISSIONSBUS_H
#define SYSTEMPERMISSIONSBUS_H

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusContext>
#include <QDBusReply>
#include <QDBusInterface>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>
#include <QFile>
#include <QObject>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#define SERVICE_NAME "com.system.permissions"

enum Permissions { SystemTime = 0 };

class systemPermissionsBus : public QObject, public QDBusContext
{
    Q_OBJECT
public:
    systemPermissionsBus(QObject* parent = nullptr ) : QObject(parent)
    {
        qDebug()<<"Constructor worked, setupSqlTable() result: "<<setupSqlTable();
    }

private:
    bool setupSqlTable()
    {
        if(!dataBase.database("QSQLITE", false).isOpen())
        {
            dataBase = QSqlDatabase::addDatabase("QSQLITE");
        }
        else
        {
            dataBase = QSqlDatabase::database("QSQLITE", false);
        }

        dataBasePath = QCoreApplication::applicationDirPath() + QString("/%1.sqlite").arg(dataBaseFileName);
        dataBase.setDatabaseName(dataBasePath);

        if(!dataBase.open())
        {
            qDebug()<<"Can't open database: "<<dataBase.lastError().text();
            return false;
        }

        QSqlQuery dataBaseQuery(dataBase);
        if(!dataBaseQuery.exec(QString("CREATE TABLE IF NOT EXISTS %1"
                                        "(filepath TEXT,"
                                        "permission_type int,"
                                        "UNIQUE(filepath, permission_type));").arg(tableName)))
        {
            qDebug()<<dataBaseQuery.lastError().text();
        }

        return true;
    }

    QSqlDatabase dataBase;
    QString dataBasePath;
    QString dataBaseFileName = "permissions";
    QString tableName = "requests";

public slots:
    bool checkApplicationHasPermission(QString applicationExecPath, int permissionEnumType)
    {
        qDebug()<<"application has permission CALLED";
        QSqlQuery dataBaseQuery(dataBase);
        dataBaseQuery.prepare("SELECT COUNT(*) FROM "+tableName+" WHERE filepath = ? AND permission_type = ?");
        dataBaseQuery.addBindValue(applicationExecPath);
        dataBaseQuery.addBindValue(permissionEnumType);
        if(!dataBaseQuery.exec())
        {
            sendErrorReply(QDBusError::Failed, "Unable to run sql request");
            return false;
        }

        if(dataBaseQuery.next())
        {
            bool permissionExists = dataBaseQuery.value(0).toInt() > 0;
            return permissionExists;
        }
        return false;
    }

    void requestPermission(int permissionEnumCode)
    {
        QString serviceName = message().service();

        QDBusInterface dbusInterface("org.freedesktop.DBus",
                                     "/org/freedesktop/DBus",
                                     "org.freedesktop.DBus",
                                     QDBusConnection::sessionBus());

        QDBusReply<uint> pidReply = dbusInterface.call("GetConnectionUnixProcessID", serviceName);
        if (pidReply.isValid())
        {
            uint pid = pidReply.value();
            QString execPath = QString("/proc/%1/exe").arg(pid);
            QString realPath = QFile(execPath).symLinkTarget();
            QSqlQuery dataBaseQuery(dataBase);
            dataBaseQuery.prepare("INSERT INTO " +tableName+ " (filepath, permission_type) VALUES (?, ?);");
            dataBaseQuery.addBindValue(realPath);
            dataBaseQuery.addBindValue(permissionEnumCode);

            if(!dataBaseQuery.exec())
            {
                sendErrorReply(QDBusError::Failed,
                               QString("Unable to run sql request: " + dataBaseQuery.lastError().text()));
            }
        }
        else
        {
            sendErrorReply(QDBusError::InvalidObjectPath, "PID is invalid");
        }
    }
};

#endif // SYSTEMPERMISSIONSBUS_H
