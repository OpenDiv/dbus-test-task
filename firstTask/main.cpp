#include "permissionsbus.h"
#include <QCoreApplication>
#include <QDebug>
#include <QtDBus/QDBusContext>
#include <QtDBus/qdbusconnection.h>

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  if (!QDBusConnection::sessionBus().isConnected()) {
    qDebug() << "Unable to connect to session bus";
    return 1;
  }

  if (!QDBusConnection::sessionBus().registerService(SERVICE_NAME)) {
    qDebug() << "Unable to register service";
    return 1;
  }

  permissionsBus receiveObj;

  if (!QDBusConnection::sessionBus().registerObject(
          "/", &receiveObj, QDBusConnection::ExportAllSlots)) {
    receiveObj.sendErrorReply(QDBusError::Failed, "Unable to register object");
    return 1;
  }

  return a.exec();
}
