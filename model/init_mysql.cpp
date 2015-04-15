#include "init_mysql.h"

init_mysql::init_mysql()
{
}

QSqlDatabase init_mysql::connect_db(){

    QString pth = "/var/www/marine/public_html/vts/veranda/setting.ini";
    QSettings sett(pth, QSettings::IniFormat);

    QString host = sett.value("hostname").toString();
    QString dbname = sett.value("databasename").toString();
    QString user = sett.value("username").toString();
    QString password = sett.value("password").toString();

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "monita_marine");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(password);

    db.open();

    return (QSqlDatabase) db;
}

void init_mysql::tulis_setting(){
    QString pth = "setting.ini";
    QSettings sett(pth, QSettings::IniFormat);

    sett.setValue("status", "connect");
}
