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
        file->write("Initialization                                          [DONE]\n");
    }
    else{
        file->write("Initialization                                          [FAILED]\n");
    }

    return (QSqlDatabase) db;
}

void init_mysql::tulis_setting(){
    QString pth = "setting.ini";
    QSettings sett(pth, QSettings::IniFormat);

    sett.setValue("status", "connect");
}
