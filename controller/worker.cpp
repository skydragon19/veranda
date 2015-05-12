#include "worker.h"

Worker::Worker(QObject *parent) : QObject(parent){
    files = new QFile();

    QFile file;
    QString name;
    name.sprintf("verandalog_%s", QDateTime::currentDateTimeUtc().toString("dd-MM-yyyy hh:mm:ss").toLocal8Bit().data());

    file.setFileName(name.toLocal8Bit());
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    files = &file;

    files->write("START");

    printf("Initialization Database .. \n");
    db = mysql.connect_db();

    connect(&timer, SIGNAL(timeout()), this, SLOT(doWork()));
    timer.start(1000 * 60 * 10); /* 10 menit */

    count = 0;
    ship_count = 0;

    marine = (struct utama *) malloc( sizeof (struct utama));
    memset((char *) marine, 0, sizeof(struct utama));

    printf("Initialization memory .. \n");
    this->get_modem_info(db);
    this->doWork();
}

void Worker::CheckForRequest(){

}

void Worker::doWork() {
    this->getResponSkyW();

    //skywaveNetwork skw;
    //skw.requestData("wdwd");
    //skw.wait();
}


void Worker::getResponSkyW(){
    printf("\nRequest --> %s ; MobileID [%s]\n", marine->kapal[ship_count].name, marine->kapal[ship_count].modem_id);

    QNetworkAccessManager *manager = new QNetworkAccessManager();
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply *)));

    QNetworkRequest request;

    modem_id.sprintf("%s", marine->kapal[ship_count].modem_id);
    access_id.sprintf("%s", marine->kapal[ship_count].access_id);
    password.sprintf("%s", marine->kapal[ship_count].password);
    nextutc.sprintf("%s", marine->kapal[ship_count].nextutc);
    gateway.sprintf("%s", marine->kapal[ship_count].gateway);

    urls.sprintf("%s/get_return_messages.xml/?access_id=%s&password=%s&start_utc=%s&mobile_id=%s",
                 gateway.toLocal8Bit().data(), access_id.toLocal8Bit().data(), password.toLocal8Bit().data(),
                 nextutc.toLocal8Bit().data(), modem_id.toLocal8Bit().data());

    QUrl url =  QUrl::fromEncoded(urls.toLocal8Bit().data());
    request.setUrl(url);
    manager->get(request);
    printf("Waiting for reply .. \n");
}

void Worker::replyFinished(QNetworkReply* reply){
    printf("\nGet Respond for %s[%s]\n", marine->kapal[ship_count].name, marine->kapal[ship_count].modem_id);
    QString readAll=reply->readAll();
    read->parse_xml(readAll, db, marine->kapal[ship_count].id_ship, SIN, MIN, marine);

    ship_count++;
#if 1
    if (ship_count < count){
        this->doWork();        
    }else{
        ship_count = 0;
    }
#endif


}

void Worker::get_modem_info(QSqlDatabase db){
    QSqlQuery q(db);

    q.prepare("SELECT s.id_ship, s.name, s.modem_id, s.access_id, s.password, s.nextutc, s.SIN, s.MIN, g.url FROM ship s join gateway g on g.id = s.gateway where s.status = 1");
    if(!q.exec()){
        printf("Initialization                                          [FAILED]\n");
    }
    else{
        while(q.next()){
            marine->kapal[count].id_ship =  q.value(0).toInt();
            strcpy(marine->kapal[count].name, q.value(1).toString().toLatin1());
            strcpy(marine->kapal[count].modem_id, q.value(2).toString().toLatin1());
            strcpy(marine->kapal[count].access_id, q.value(3).toString().toLatin1());
            strcpy(marine->kapal[count].password, q.value(4).toString().toLatin1());
            if(q.value(5).toString() == ""){
                strcpy(marine->kapal[count].nextutc, QDateTime::currentDateTime().toString("yyyy-MM-dd 00:00:00").toLatin1());
            }
            else{
                strcpy(marine->kapal[count].nextutc, q.value(5).toDateTime().toString("yyyy-MM-dd hh:mm:ss").toLatin1());
            }
            marine->kapal[count].SIN = q.value(6).toInt();
            marine->kapal[count].MIN = q.value(7).toInt();
            strcpy(marine->kapal[count].gateway, q.value(8).toString().toLatin1());

            count++;
        }
        printf("Initialization                                          [DONE]\n");
    }
}
