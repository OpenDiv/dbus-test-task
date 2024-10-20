#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusContext>
#include <QDebug>

#include <systemtimebus.h>

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  qDebug() << "begin second";
  if (!QDBusConnection::sessionBus().isConnected()) {
    qDebug() << "Unable to connect to session bus";
    return 1;
  }

  if (!QDBusConnection::sessionBus().registerService(SERVICE_NAME)) {
    qDebug() << "Unable to register service";
    return 1;
  }

  systemTimeBus timeBus;

  if (!QDBusConnection::sessionBus().registerObject(
          "/", &timeBus, QDBusConnection::ExportAllSlots)) {
    timeBus.sendErrorReply(QDBusError::Failed, "Unable to register object");
    return 1;
  }

  return a.exec();
}
