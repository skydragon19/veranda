#include "init_mysql.h"

init_mysql::init_mysql()
{
}

QSqlDatabase init_mysql::connect_db(QFile *file){
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("marine_1");
    db.setUserName("root");
    db.setPassword("rqi");

    db.open();

    return (QSqlDatabase) db;
}

