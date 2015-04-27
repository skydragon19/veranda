#include <QtCore/QCoreApplication>
#include <stdio.h>

#include <QObject>
#include <QDateTime>

#include "controller/worker.h"
#include "view/skywavenetwork.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << ":: Veranda Start ::";
    qDebug() << "   --> UTC Time   : " << QDateTime::currentDateTimeUtc().toString("dd-MM-yyyy hh:mm:ss");
    qDebug() << "   --> Local TIme : " << QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss");

    Worker worker;
    return a.exec();
}
