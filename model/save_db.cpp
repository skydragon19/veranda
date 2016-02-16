#include "save_db.h"

save_db::save_db()
{
}

void save_db::data(QSqlQuery *q, float value, int id_tu, int id_trip, int epochtime, QString data_time, int flag){
    //printf("insert id_tu : %d --> value : %.2f\n", id_tu, value);

    QString query;
    query.sprintf("REPLACE INTO data(id_titik_ukur, value, id_trip, epochtime, data_time, flag_data) VALUES(%d, %.2f, %d, %d, '%s', %d)",
                  id_tu, value, id_trip, epochtime, data_time.toUtf8().data(), flag);

    printf("%s\n", query.toUtf8().data());

    q->prepare(query);
    q->exec();


    /*
    q->prepare("REPLACE INTO data(id_titik_ukur, value, id_trip, epochtime, data_time) VALUES(:id_titik_ukur, :value, :id_trip, :epochtime, :data_time)");

    q->bindValue(":id_titik_ukur", id_tu);
    q->bindValue(":value", value);
    q->bindValue(":id_trip", id_trip);
    q->bindValue(":epochtime", epochtime);
    q->bindValue(":data_time", data_time.toUtf8().data());
    */


}

void save_db::data_test(QSqlQuery *q, float value, int id_tu, int id_trip, int epochtime, QString data_time){
    q->prepare("REPLACE INTO data_test(id_titik_ukur, value, id_trip, epochtime, data_time) VALUES(:id_titik_ukur, :value, :id_trip, :epochtime, :data_time)");

    q->bindValue(":id_titik_ukur", id_tu);
    q->bindValue(":value", value);
    q->bindValue(":id_trip", id_trip);
    q->bindValue(":epochtime", epochtime);
    q->bindValue(":data_time", data_time.toUtf8().data());

    q->exec();
}

void save_db::data_harian(QSqlQuery *q, float value, int id_tu, int id_trip, int epochtime, QString data_time, int index, int flag){
    QString query;
    query.sprintf("REPLACE INTO data_%d(id_titik_ukur, value, id_trip, epochtime, data_time, flag_data) VALUES(:id_titik_ukur, :value, :id_trip, :epochtime, :data_time, :flag_data)", index);

    q->prepare(query);

    q->bindValue(":id_titik_ukur", id_tu);
    q->bindValue(":value", value);
    q->bindValue(":id_trip", id_trip);
    q->bindValue(":epochtime", epochtime);
    q->bindValue(":data_time", data_time.toUtf8().data());
    q->bindValue(":flag_data", flag);

    q->exec();
}

void save_db::update_next_utc(QSqlDatabase db, QString next_utc, int id_ship){
    QString query;
    query.sprintf("UPDATE ship SET nextutc = '%s' where id_ship = %d", next_utc.toUtf8().data(), id_ship);

    QSqlQuery q(db);
    q.first();
    q.exec(query);
    q.finish();
}

void save_db::update_next_utc_gateway(QSqlDatabase db, QString next_utc, int id_gateway){
    QString query;
    query.sprintf("UPDATE gateway set next_utc = '%s' where id = %d", next_utc.toUtf8().data(), id_gateway);

    QSqlQuery q(db);
    q.first();
    q.exec(query);
    q.finish();
}

void save_db::create_tabel_data_harian(QSqlQuery *q, int index){
    QString query;

    query.clear();
    query.sprintf("CREATE TABLE if not exists data_%d (id_titik_ukur INT NOT NULL, value FLOAT NOT NULL, id_trip INT NULL DEFAULT NULL, \
                  epochtime INT NOT NULL, data_time DATETIME NOT NULL, flag_data INT, PRIMARY KEY (id_titik_ukur, data_time))", index);
    q->exec(query);
}

void save_db::delete_data_periodic(QSqlQuery *q, int index){
    QString query;

    query.clear();
    query.sprintf("DELETE FROM data_test where epochtime < %d", index);

    q->exec(query);
}
