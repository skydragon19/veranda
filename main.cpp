#if QT_VERSION >= 0x050000
    #include <QApplication>
#else
    #include <QtGui/QApplication>
#endif

#include <QtCore/QCoreApplication>

#include <stdio.h>

#include "controller/worker.h"
#include "model/init_mysql.h"
#include "view/skywavenetwork.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << "main thread ID: " << a.thread()->currentThreadId();

    init_mysql mysql;
    Worker worker(0, mysql.connect_db());

    return a.exec();
}
