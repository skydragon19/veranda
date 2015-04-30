#include "init_mysql.h"

init_mysql::init_mysql()
{
}

QSqlDatabase init_mysql::connect_db(){
#if 0
    QString pth = "veranda_set.ini";
    QSettings sett(pth, QSettings::IniFormat);

    QString host = sett.value("hostname").toString();
    QString dbname = sett.value("databasename").toString();
    QString user = sett.value("username").toString();
    QString password = sett.value("password").toString();
#endif

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("marine_1");
    db.setUserName("root");
    db.setPassword("rqi");

    if(db.open()){
        printf("Initialization                                          [DONE]\n");
    }
    else{
        printf("Initialization                                          [FAILED]\n");
    }

    return (QSqlDatabase) db;
}

void init_mysql::tulis_setting(){
    QString pth = "setting.ini";
    QSettings sett(pth, QSettings::IniFormat);

    sett.setValue("status", "connect");
}
