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

    if(db.open()){
        vlog.write(file, "Success Initialization Database");
    }
    else{
        vlog.write(file, "Failed Initialization Database");
    }

    return (QSqlDatabase) db;
}

