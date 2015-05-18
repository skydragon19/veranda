#include "get_db.h"

get_db::get_db()
{
}

int get_db::sum_ship(QSqlDatabase db){
    QSqlQuery q(db);

    q.prepare("SELECT COUNT(id_ship) FROM ship");
    if (!q.exec()){
        printf("err():\n");
        return (int) 0;
    }
    else{
        while(q.next()){
            return (int) q.value(0).toInt();
        }
    }
}

int get_db::id_tu_ship(QSqlQuery *q, int id_ship, int urutan){
    QString query;

    query.sprintf("SELECT count(id_tu), id_tu FROM parsing_ref WHERE id_ship = %d and urutan_data_monita = %d", id_ship, urutan);

    q->prepare(query);
    if(!q->exec()){
        printf("err():\n");
        return (int) 0;
    }
    else{
        while(q->next()){
            if (q->value(0).toInt() == 0){
                return(int) 0;
            }
            else{
                return (int) q->value(1).toInt();
            }
        }
    }
}

void get_db::modem_info(QSqlQuery *q, utama *marine){
    int count = 0;

    q->prepare("SELECT s.id_ship, s.name, s.modem_id, s.access_id, s.password, s.nextutc, s.SIN, s.MIN, g.url FROM ship s join gateway g on g.id = s.gateway where s.status = 1");
    if(!q->exec()){
        printf("Initialization                                          [FAILED]\n");
    }
    else{
        while(q->next()){
            marine->kapal[count].id_ship =  q->value(0).toInt();
            strcpy(marine->kapal[count].name, q->value(1).toString().toLatin1());
            strcpy(marine->kapal[count].modem_id, q->value(2).toString().toLatin1());
            strcpy(marine->kapal[count].access_id, q->value(3).toString().toLatin1());
            strcpy(marine->kapal[count].password, q->value(4).toString().toLatin1());
            if(q->value(5).toString() == ""){
                strcpy(marine->kapal[count].nextutc, QDateTime::currentDateTime().toString("yyyy-MM-dd 00:00:00").toLatin1());
            }
            else{
                strcpy(marine->kapal[count].nextutc, q->value(5).toDateTime().toString("yyyy-MM-dd hh:mm:ss").toLatin1());
            }
            marine->kapal[count].SIN = q->value(6).toInt();
            marine->kapal[count].MIN = q->value(7).toInt();
            strcpy(marine->kapal[count].gateway, q->value(8).toString().toLatin1());

            printf("\n %s", marine->kapal[count].name);

            count++;
        }
        marine->sum_ship = count;
        printf("Initialization                                          [DONE]\n");
    }
}
