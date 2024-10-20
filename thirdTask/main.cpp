#include <QCoreApplication>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusError>
#include <QtDBus/QDBusPendingCall>
#include <QDebug>
#include <QDateTime>

#define QDBusLastErrorCode 26

QDBusReply<quint64> TimeMethodGetSystemTime()
{
    QDBusInterface timeBusInterface("com.system.time",
                                    "/",
                                    "local.dbusTestTaskSecond.systemTimeBus",
                                    QDBusConnection::sessionBus());
    return timeBusInterface.call("getSystemTime");
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug()<<"Third task app begin";

    QDBusReply<quint64> systemTimeReply = TimeMethodGetSystemTime();
    if(!systemTimeReply.isValid())
    {
        qDebug()<<"Something is wrong: "<<systemTimeReply.error().message();
        return 0;
    }
    else
    {
        if(systemTimeReply.value() > QDBusLastErrorCode)
        {
            QDateTime dateTime = QDateTime::fromMSecsSinceEpoch(systemTimeReply.value());
            qDebug()<<dateTime.toString("yyyy-MM-dd HH:mm:ss");
            return 0;
        }
        else if (systemTimeReply.value() == static_cast<quint64>(QDBusError::AccessDenied))
        {
            qDebug()<<"Permission denied";
            QDBusInterface permissionsBusInterface("com.system.permissions",
                                                   "/",
                                                   "local.dbusTestTaskFirst.permissionsBus",
                                                   QDBusConnection::sessionBus());
            permissionsBusInterface.call("requestPermission", 1);
            qDebug()<<"Requesting for permission";
            qDebug()<<"Trying again: ";
            systemTimeReply = TimeMethodGetSystemTime();
            if(systemTimeReply.isValid())
            {
                if(systemTimeReply.value() == static_cast<quint64>(QDBusError::AccessDenied))
                {
                    qDebug()<<"Something is wrong, permission wasn't granted";
                    return 0;
                }
                else
                {
                    QDateTime dateTime = QDateTime::fromMSecsSinceEpoch(systemTimeReply.value());
                    qDebug()<<dateTime.toString("yyyy-MM-dd HH:mm:ss");
                    return 1;
                }
            }
            else
            {
                qDebug()<<"Cant get reply from com.system.time";
                return 0;
            }
        }
        else
        {
            qDebug()<<"Something is wrong with com.system.time, check secondTask output for more information";
            return 0;
        }
    }
    return 1;
}
