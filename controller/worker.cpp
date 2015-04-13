#include "worker.h"

extern struct xmll xmls;

Worker::Worker(QObject *parent) : QObject(parent){
    qDebug() << "yang penting kebaca dulu yah";

#if 1
    QSqlDatabase db = mysql.connect_db();

    connect(&timer, SIGNAL(timeout()), this, SLOT(doWork()));
    timer.start(1000 * 60 * 10); /* 10 menit */

    Qdb = db;
    cek_replay = 0;

    idx_ship = 1;
    sum_ship = get->sum_ship(db);

    qDebug() << "start";

    this->doWork();
#endif
}

void Worker::CheckForRequest(){
    qDebug() << QDateTime::currentDateTime().toString("mm").toInt();

}

void Worker::doWork() {
    QDateTime dateTime = QDateTime::currentDateTime();

    this->get_modem_info(Qdb, idx_ship);
    //this->getResponSkyW();

    //skywaveNetwork skw;
    //skw.requestData("wdwd");
    //skw.wait();
}


void Worker::getResponSkyW(){
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply *)));
    qDebug()<< __FUNCTION__;

    QNetworkRequest request;

    QString urls;

    if (xml_ver == 1){
        urls.sprintf("http://isatdatapro.skywave.com/GLGW/GWServices_v1/RestMessages.svc/get_return_messages.xml/?access_id=%s&password=%s&start_utc=%s&mobile_id=%s", access_id.toLocal8Bit().data(), password.toLocal8Bit().data(), nextutc.toLocal8Bit().data(), modem_id.toLocal8Bit().data());
    }
    else if ( xml_ver == 2){
        qDebug() << "xml baru";
    }

    QUrl url =  QUrl::fromEncoded(urls.toLocal8Bit().data());
    request.setUrl(url);
    manager->get(request);
}

void Worker::replyFinished(QNetworkReply* reply){
    QString readAll=reply->readAll();
    read->parse_xml(readAll, Qdb, idx_ship, SIN, MIN);

    if (idx_ship != sum_ship){
        idx_ship++;
        qDebug() << "HABIS";
        this->doWork();        
    }else{
        idx_ship = 1;
        qDebug() << "STOP\n\nNunggu Timer";
    }
}

void Worker::get_modem_info(QSqlDatabase db, int id){
    QSqlQuery q(db);
    QString query;
    query.sprintf("SELECT modem_id, access_id, password, nextutc, SIN, MIN, xml_ver FROM ship WHERE id_ship = %d", id);
    q.prepare(query);
    if(!q.exec()){
        qDebug() << "err():";
        return;
    }
    else{
        while(q.next()){
            modem_id = q.value(0).toString();
            access_id = q.value(1).toString();
            password = q.value(2).toString();
            SIN = q.value(4).toInt();
            MIN = q.value(5).toInt();
            xml_ver = q.value(6).toInt();

            if(q.value(3).toString() == ""){
                nextutc = QDateTime::currentDateTime().toString("yyyy-MM-dd 00:00:00");
            }
            else{
                nextutc = q.value(3).toDateTime().toString("yyyy-MM-dd hh-mm-ss");
            }
            qDebug() << modem_id << access_id << password << nextutc << SIN << MIN << xml_ver;
        }
    }
}
