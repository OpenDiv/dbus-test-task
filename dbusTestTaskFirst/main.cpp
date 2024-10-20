#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusContext>
#include <QDebug>


#include "systempermissionsbus.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (!QDBusConnection::sessionBus().isConnected())
    {
        qDebug()<<"Unable to connect to session bus";
        return 1;
    }

    if (!QDBusConnection::sessionBus().registerService(SERVICE_NAME))
    {
        qDebug() << "Unable to register service";
        return 1;
    }

    systemPermissionsBus permissionsBus;

    if(!QDBusConnection::sessionBus().registerObject("/", &permissionsBus, QDBusConnection::ExportAllSlots)) {
        permissionsBus.sendErrorReply(QDBusError::Failed, "Unable to register object");
        return 1;
    }

    return a.exec();
}
