#ifndef INIT_MYSQL_H
#define INIT_MYSQL_H

#include <QDebug>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDir>

#include <QSettings>

class init_mysql
{
public:
    init_mysql();

    QSqlDatabase connect_db(QFile *file);
    void tulis_setting();
};

#endif // INIT_MYSQL_H
