#include "save_db.h"

save_db::save_db()
{
}

void save_db::data(QSqlQuery *q, float value, int id_tu, int id_trip, int epochtime, QString data_time){
    q->prepare("REPLACE INTO data(id_titik_ukur, value, id_trip, epochtime, data_time) VALUES(:id_titik_ukur, :value, :id_trip, :epochtime, :data_time)");

    q->bindValue(":id_titik_ukur", id_tu);
    q->bindValue(":value", value);
    q->bindValue(":id_trip", id_trip);
    q->bindValue(":epochtime", epochtime);
    q->bindValue(":data_time", data_time.toUtf8().data());

    q->exec();
}

void save_db::data_harian(QSqlQuery *q, float value, int id_tu, int id_trip, int epochtime, QString data_time, int index){
    QString query;
    query.sprintf("REPLACE INTO data_%d(id_titik_ukur, value, id_trip, epochtime, data_time) VALUES(:id_titik_ukur, :value, :id_trip, :epochtime, :data_time)", index);

    q->prepare(query);

    q->bindValue(":id_titik_ukur", id_tu);
    q->bindValue(":value", value);
    q->bindValue(":id_trip", id_trip);
    q->bindValue(":epochtime", epochtime);
    q->bindValue(":data_time", data_time.toUtf8().data());

    q->exec();
}

void save_db::update_next_utc(QSqlQuery *q, QString next_utc, int id_ship){
    q->prepare("UPDATE ship SET nextutc = :nextutc where id_ship = :id_ship");

    q->bindValue(":nextutc", next_utc.toUtf8().data());
    q->bindValue(":id_ship", id_ship);

    q->exec();
}

void save_db::update_next_utc_gateway(QSqlQuery *q, QString next_utc, int id_gateway){
    q->prepare("UPDATE gateway set next_utc = :next_utc where id = :id");

    q->bindValue(":next_utc", next_utc.toUtf8().data());
    q->bindValue(":id", id_gateway);

    q->exec();
}

void save_db::create_tabel_data_harian(QSqlQuery *q, int index){
    QString query;

    query.clear();
    query.sprintf("CREATE TABLE if not exists data_%d (id_titik_ukur INT NOT NULL, value FLOAT NOT NULL, id_trip INT NULL DEFAULT NULL, \
                  epochtime INT NOT NULL, data_time DATETIME NOT NULL, PRIMARY KEY (id_titik_ukur, data_time))", index);
    q->exec(query);
}
