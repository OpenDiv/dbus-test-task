#ifndef SYSTEMTIMEBUS_H
#define SYSTEMTIMEBUS_H

#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusContext>
#include <QDBusInterface>
#include <QDBusMessage>
#include <QDBusReply>
#include <QDebug>
#include <QFile>
#include <QObject>
#include <QDateTime>

#define SERVICE_NAME "com.system.time"

class systemTimeBus : public QObject, public QDBusContext {
  Q_OBJECT
public slots:
  quint64 getSystemTime() {
    qDebug() << "getSystemTime() begin";
    QString serviceName = message().service();

    QDBusInterface dbusInterface(
        "org.freedesktop.DBus", "/org/freedesktop/DBus", "org.freedesktop.DBus",
        QDBusConnection::sessionBus());

    QDBusReply<uint> pidReply =
        dbusInterface.call("GetConnectionUnixProcessID", serviceName);
    if (!pidReply.isValid()) {
      qDebug() << "Return: 0 [Invalid PID]";
      return static_cast<quint64>(QDBusError::Failed);
    }

    uint pid = pidReply.value();
    QString execPath = QString("/proc/%1/exe").arg(pid);
    QString realPath = QFile(execPath).symLinkTarget();
    QDBusInterface permissionsBusInterface(
        "com.system.permissions", "/", "local.dbusTestTaskFirst.permissionsBus",
        QDBusConnection::sessionBus());

    QDBusReply<bool> reply = permissionsBusInterface.call(
        "checkApplicationHasPermission", realPath, 1);

    if (!reply.isValid()) {
      qDebug() << "Return: " << static_cast<quint64>(QDBusError::NoReply)
               << " [Invalid reply]";
      return static_cast<quint64>(QDBusError::NoReply);
    }

    quint64 timestampMs =
        static_cast<quint64>(QDateTime::currentMSecsSinceEpoch());

    if (reply.value()) {

      qDebug() << "Access granted - returning timestamp: " << timestampMs;
      return timestampMs;
    } else {
      qDebug() << "Return: " << static_cast<quint64>(QDBusError::AccessDenied)
               << " [Access denied]";
      return static_cast<quint64>(QDBusError::AccessDenied);
    }
  }
};

#endif // SYSTEMTIMEBUS_H
