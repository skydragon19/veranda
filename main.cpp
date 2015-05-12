#include <QtCore/QCoreApplication>
#include <stdio.h>

#include <QObject>
#include <QDateTime>

#include "controller/worker.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Worker worker;

    return a.exec();
}
