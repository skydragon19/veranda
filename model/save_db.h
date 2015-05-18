#ifndef SAVE_DB_H
#define SAVE_DB_H

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

#include <QDateTime>

class save_db
{
public:
    save_db();

    void data(QSqlQuery *q, float value, int id_tu, int id_trip, int epochtime, QString data_time);
    void update_next_utc(QSqlQuery *q, QString next_utc, int id_ship);
};

#endif // SAVE_DB_H
