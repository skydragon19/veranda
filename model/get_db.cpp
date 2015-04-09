#include "get_db.h"

get_db::get_db()
{
}

int get_db::sum_ship(QSqlDatabase db){
    QSqlQuery q(db);

    q.prepare("SELECT COUNT(id_ship) FROM ship");
    if (!q.exec()){
        qDebug() << "err():";
        return (int) 0;
    }
    else{
        while(q.next()){
            return (int) q.value(0).toInt();
        }
    }
}

int get_db::id_tu_ship(QSqlDatabase db, int id_ship, int urutan){
    QSqlQuery q(db);
    QString query;

    query.sprintf("SELECT id_tu FROM parsing_ref WHERE id_ship = %d and urutan_data_monita = %d", id_ship, urutan);

    q.prepare(query);
    if(!q.exec()){
        qDebug() << "err():";
        return (int) 0;
    }
    else{
        while(q.next()){
            return (int) q.value(0).toInt();
        }
    }
}
