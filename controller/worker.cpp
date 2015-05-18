#include "worker.h"

Worker::Worker(QObject *parent) : QObject(parent){
    this->initNetworkManager();

    printf("\nInitialization Database .. \n");
    db = mysql.connect_db();

    qsql = new QSqlQuery(db);

    connect(&timer, SIGNAL(timeout()), this, SLOT(doWork()));

    timer.start(1000 * 60 * 10); /* 10 menit */

    ship_count = 0;

    marine = (struct utama *) malloc( sizeof (struct utama));
    memset((char *) marine, 0, sizeof(struct utama));

    printf("\nInitialization memory .. \n");
    qsql->clear();
    get.modem_info(qsql, marine);
    this->doWork();
}

void Worker::initNetworkManager(){
    manager = new QNetworkAccessManager();
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply *)));
}

void Worker::CheckForRequest(){

}

void Worker::doWork(){
    this->getResponSkyW();

    //skywaveNetwork skw;
    //skw.requestData("wdwd");
    //skw.wait();
}


void Worker::getResponSkyW(){
    printf("\nRequest --> %s ; MobileID [%s]\n", marine->kapal[ship_count].name, marine->kapal[ship_count].modem_id);

    QNetworkRequest request;

    urls.sprintf("%s/get_return_messages.xml/?access_id=%s&password=%s&start_utc=%s&mobile_id=%s",
                  marine->kapal[ship_count].gateway, marine->kapal[ship_count].access_id, marine->kapal[ship_count].password,
                  marine->kapal[ship_count].nextutc, marine->kapal[ship_count].modem_id);

    QUrl url =  QUrl::fromEncoded(urls.toLocal8Bit().data());
    request.setUrl(url);

    manager->get(request);

    printf("Waiting for reply .. \n");
}

void Worker::replyFinished(QNetworkReply* reply){
    printf("\nGet Respond for %s[%s]\n", marine->kapal[ship_count].name, marine->kapal[ship_count].modem_id);
    QString readAll=reply->readAll();
    read.parse_xml(readAll, qsql, marine->kapal[ship_count].id_ship, SIN, MIN, marine);

    ship_count++;
#if 1
    if (ship_count < marine->sum_ship){
        this->doWork();        
    }else{
        ship_count = 0;
    }
#endif


}
