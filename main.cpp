#include <QtCore/QCoreApplication>
#include <stdio.h>

#include <QObject>
#include <QDateTime>

#include "controller/worker.h"
#include "model/veranda_log.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    printf("Veranda Satalite Parser Started :: \n");

    Worker worker;

    return a.exec();
}
