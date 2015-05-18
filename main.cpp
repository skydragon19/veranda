#include <QtCore/QCoreApplication>
#include <stdio.h>

#include <QObject>
#include <QDateTime>

#include "controller/worker.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString utc, localtime;

    printf(":: Veranda Satalite Parser Started :: \n\n");

    utc.sprintf(" * UTC Time   : %s\n", QDateTime::currentDateTimeUtc().toString("dd-MM-yyyy hh:mm:ss").toLocal8Bit().data());
    printf(utc.toLocal8Bit());

    localtime.sprintf(" * Local TIme : %s\n", QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss").toLocal8Bit().data());
    printf(localtime.toLocal8Bit());

    Worker worker;

    return a.exec();
}
