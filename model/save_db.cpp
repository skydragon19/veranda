#include "save_db.h"

save_db::save_db()
{
}

void save_db::data(QSqlQuery *q, float value, int id_tu, int id_trip, int epochtime, QString data_time){
    printf("\n%f, %d, %d, %d, %s", value, id_tu, id_trip, epochtime, data_time.toLocal8Bit().data());

    q->prepare("REPLACE INTO data(id_titik_ukur, value, id_trip, epochtime, data_time, year, month, day, hour, minute, origin) VALUES(:id_titik_ukur, :value, :id_trip, :epochtime, :data_time, :year, :month, :day, :hour, :minute, :origin)");

    q->bindValue(":id_titik_ukur", id_tu);
    q->bindValue(":value", value);
    q->bindValue(":id_trip", id_trip);
    q->bindValue(":epochtime", epochtime);
    q->bindValue(":data_time", data_time.toLocal8Bit().data());

    q->exec();
}

void save_db::update_next_utc(QSqlQuery *q, QString next_utc, int id_ship){
    q->prepare("UPDATE ship SET nextutc = :nextutc where id_ship = :id_ship");

    q->bindValue(":nextutc", next_utc.toLocal8Bit().data());
    q->bindValue(":id_ship", id_ship);

    q->exec();
}
