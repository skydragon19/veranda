#include "worker.h"

Worker::Worker(QObject *parent) : QObject(parent){
    this->initNetworkManager();

    printf("\nInitialization Database .. \n");
    db = mysql.connect_db();

    qsql = new QSqlQuery(db);

    connect(&timer, SIGNAL(timeout()), this, SLOT(doWork()));
    timer.start(1000 * 60 * 10); /* 10 menit */

    marine = (struct utama *) malloc( sizeof (struct utama));
    memset((char *) marine, 0, sizeof(struct utama));

    printf("\nInitialization memory .. \n");
    qsql->clear();
    get.modem_info(qsql, marine);

    ship_count = 0;
    cnt_panggil = 0;

    this->doWork();
}

void Worker::initNetworkManager(){
    manager = new QNetworkAccessManager();
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply *)));
}

void Worker::CheckForRequest(){

}

void Worker::doWork(){
    timer.stop();
    this->getResponSkyW();
}


void Worker::getResponSkyW(){
    printf("\nRequest --> %s ; MobileID [%s]\n", marine->kapal[ship_count].name, marine->kapal[ship_count].modem_id);

    QNetworkRequest request;

    urls.sprintf("%sget_return_messages.xml/?access_id=%s&password=%s&start_utc=%s&mobile_id=%s",
                  marine->kapal[ship_count].gateway, marine->kapal[ship_count].access_id, marine->kapal[ship_count].password,
                  marine->kapal[ship_count].nextutc, marine->kapal[ship_count].modem_id);

    QUrl url =  QUrl::fromEncoded(urls.toLocal8Bit().data());

    printf("Get Url : \n %s \n\n", urls.toUtf8().data());

    request.setUrl(url);

    manager->get(request);

    printf("Waiting for reply .. \n");
}

void Worker::replyFinished(QNetworkReply* reply){            
    printf("\nGet Respond for --> kapal ke %d\n", ship_count+1);
    printf("id : %d --> modem id : %s --> kapal : %s\n",
           marine->kapal[ship_count].id_ship, marine->kapal[ship_count].modem_id, marine->kapal[ship_count].name);

    QString readAll;
    readAll.clear();

    readAll=reply->readAll();
    //printf("\n%s\n", readAll.toUtf8().data());

    read.parse_xml(readAll, qsql, marine->kapal[ship_count].id_ship, marine->kapal[ship_count].SIN, marine->kapal[ship_count].MIN, marine, ship_count);

    ship_count++;

    if (ship_count < marine->sum_ship){
        this->getResponSkyW();
    }else{
        cnt_panggil++;
        printf("HABIS --> pemanggilan ke %d \n", cnt_panggil);

        ship_count = 0;
        timer.start(1000 * 60 * 10);
    }
}
