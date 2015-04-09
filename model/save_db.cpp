#include "save_db.h"

save_db::save_db()
{
}

void save_db::data(QSqlDatabase db, float value, int id_tu, int id_trip, QString data_time, int year, int month, int day,
                   int hour, int minute, int origin){
    QSqlQuery q(db);

    q.prepare("INSERT INTO data(id_titik_ukur, value, id_trip, data_time, year, month, day, hour, minute, origin) VALUES(:id_tu, :value, :id_trip, \
              :data_time, :year, :month, :day, :hour, :minute, :origin)");

    q.bindValue(":id_tu", id_tu);
    q.bindValue(":value", value);
    q.bindValue(":id_trip", id_trip);
    q.bindValue(":data_time", data_time.toAscii().data());
    q.bindValue(":year", year);
    q.bindValue(":month", month);
    q.bindValue(":day", day);
    q.bindValue(":hour", hour);
    q.bindValue(":minute", minute);
    q.bindValue(":origin", origin);

    q.exec();
}

void save_db::update_next_utc(QSqlDatabase db, QString next_utc, int id_ship){
    QSqlQuery q(db);

    q.prepare("UPDATE ship SET nextutc = :nextutc where id_ship = :id_ship");

    q.bindValue(":nextutc", next_utc.toAscii().data());
    q.bindValue(":id_ship", id_ship);

    q.exec();
}
