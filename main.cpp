#include <QtCore/QCoreApplication>
#include <stdio.h>

#include <QObject>
#include <QDateTime>

#include "controller/worker.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    printf(":: Veranda Service Start ::\n");
    printf("   --> UTC Time   : %s\n", QDateTime::currentDateTimeUtc().toString("dd-MM-yyyy hh:mm:ss").toLocal8Bit().data());
    printf("   --> Local TIme : %s\n", QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss").toLocal8Bit().data());

    Worker worker;

    return a.exec();
}
