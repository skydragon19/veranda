#ifndef GET_DB_H
#define GET_DB_H

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

#include <QDateTime>

#include <util/utama.h>

class get_db
{
public:
    get_db();

    int sum_ship(QSqlDatabase db);
    int id_tu_ship(QSqlQuery *q, int id_ship, int urutan);
    void modem_info(QSqlQuery *q, struct utama *marine);
    void modem_getway(QSqlQuery *q, struct account *acc);
};

#endif // GET_DB_H
