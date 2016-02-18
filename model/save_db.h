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

    void data(QSqlQuery *q, float value, int id_tu, int id_trip, int epochtime, QString data_time, int flag);
    void data_test(QSqlQuery *q, float value, int id_tu, int id_trip, int epochtime, QString data_time);
    void data_harian(QSqlQuery *q, float value, int id_tu, int id_trip, int epochtime, QString data_time, int index, int flag);

    void update_next_utc(QSqlDatabase db, QString next_utc, int id_ship);
    void update_next_utc_gateway(QSqlDatabase db, QString next_utc, int id_gateway);

    void create_tabel_data_harian(QSqlQuery *q, int index);
    void delete_data_periodic(QSqlQuery *q, int index);
};

#endif // SAVE_DB_H
