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

    void data(QSqlDatabase db, float value, int id_tu, int id_trip, QString data_time, int year, int month, int day,
              int hour, int minute, int origin);
    void update_next_utc(QSqlDatabase db, QString next_utc, int id_ship);
};

#endif // SAVE_DB_H
