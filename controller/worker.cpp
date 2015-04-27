#include "worker.h"

Worker::Worker(QObject *parent) : QObject(parent){
    db = mysql.connect_db();

    connect(&timer, SIGNAL(timeout()), this, SLOT(doWork()));
    timer.start(1000 * 60 * 10); /* 10 menit */

    count = 0;
    ship_count = 0;

    marine = (struct utama *) malloc( sizeof (struct utama));
    memset((char *) marine, 0, sizeof(struct utama));

    this->get_modem_info(db);
    this->doWork();
}

void Worker::CheckForRequest(){
    //qDebug() << QDateTime::currentDateTime().toString("mm").toInt();
}

void Worker::doWork() {
    //qDebug() << "::Request Data Moobile ID :" << marine->kapal[ship_count].modem_id;
    this->getResponSkyW();

    //skywaveNetwork skw;
    //skw.requestData("wdwd");
    //skw.wait();
}


void Worker::getResponSkyW(){
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply *)));
    //qDebug()<< __FUNCTION__;

    QNetworkRequest request;

    modem_id.sprintf("%s", marine->kapal[ship_count].modem_id);
    access_id.sprintf("%s", marine->kapal[ship_count].access_id);
    password.sprintf("%s", marine->kapal[ship_count].password);
    nextutc.sprintf("%s", marine->kapal[ship_count].nextutc);
    gateway.sprintf("%s", marine->kapal[ship_count].gateway);

    urls.sprintf("%s/get_return_messages.xml/?access_id=%s&password=%s&start_utc=%s&mobile_id=%s",
                 gateway.toLocal8Bit().data(), access_id.toLocal8Bit().data(), password.toLocal8Bit().data(),
                 nextutc.toLocal8Bit().data(), modem_id.toLocal8Bit().data());

    //qDebug() << urls;

    QUrl url =  QUrl::fromEncoded(urls.toLocal8Bit().data());
    request.setUrl(url);
    manager->get(request);
}

void Worker::replyFinished(QNetworkReply* reply){
    QString readAll=reply->readAll();
    read->parse_xml(readAll, db, marine->kapal[ship_count].id_ship, SIN, MIN, marine);
    //qDebug() << "Next UTC :" << marine->kapal[ship_count].nextutc;
    ship_count++;
#if 1
    if (ship_count < count){
        //qDebug() << "HABIS";
        this->doWork();        
    }else{
        ship_count = 0;
        //qDebug() << "STOP\n\::Waiting Next Request::";
    }
#endif


}

void Worker::get_modem_info(QSqlDatabase db){
    QSqlQuery q(db);

    q.prepare("SELECT s.id_ship, s.modem_id, s.access_id, s.password, s.nextutc, s.SIN, s.MIN, g.url FROM ship s join gateway g on g.id = s.gateway");
    if(!q.exec()){
        qDebug() << "Err()";
    }
    else{
        while(q.next()){
            marine->kapal[count].id_ship =  q.value(0).toInt();
            strcpy(marine->kapal[count].modem_id, q.value(1).toString().toLatin1());
            strcpy(marine->kapal[count].access_id, q.value(2).toString().toLatin1());
            strcpy(marine->kapal[count].password, q.value(3).toString().toLatin1());
            if(q.value(4).toString() == ""){
                strcpy(marine->kapal[count].nextutc, QDateTime::currentDateTime().toString("yyyy-MM-dd 00:00:00").toLatin1());
            }
            else{
                strcpy(marine->kapal[count].nextutc, q.value(4).toDateTime().toString("yyyy-MM-dd hh:mm:ss").toLatin1());
            }
            marine->kapal[count].SIN = q.value(5).toInt();
            marine->kapal[count].MIN = q.value(6).toInt();
            strcpy(marine->kapal[count].gateway, q.value(7).toString().toLatin1());

            count++;
        }
    }
}
