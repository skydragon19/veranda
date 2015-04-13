#include <QtCore/QCoreApplication>

#include <stdio.h>

#include "controller/worker.h"
#include "view/skywavenetwork.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Worker worker;

    return a.exec();
}
