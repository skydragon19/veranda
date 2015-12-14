#include "worker.h"

Worker::Worker(QObject *parent) : QObject(parent){
    this->initNetworkManager();

    printf("Initialization Database\n");
    db = mysql.connect_db();

    qsql = new QSqlQuery(db);

    printf("\nApplication Running : %s \n", QDateTime::currentDateTime().toString("hh:mm:ss").toUtf8().data());
    connect(&timer, SIGNAL(timeout()), this, SLOT(doWork()));
    timer.start((1000 * 60 * 10) / 2); /* 5 menit */

    marine = (struct utama *) malloc( sizeof (struct utama));
    memset((char *) marine, 0, sizeof(struct utama));

    acc = (struct account *) malloc ( sizeof (struct account));
    memset((char *) acc, 0, sizeof(struct account));

    printf("Initialization memory\n");
    qsql->clear();
    get.modem_info(qsql, marine);
    get.modem_getway(qsql, acc);

    ship_count = 0;
    gateway_count = 0;
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
    QNetworkRequest request;

    urls.sprintf("%s%s", acc->gway[gateway_count].link, acc->gway[gateway_count].nextutc);
    printf("\n%s\n", urls.toUtf8().data());

    QUrl url =  QUrl::fromEncoded(urls.toLocal8Bit().data());

    request.setUrl(url);

    manager->get(request);

    printf("Waiting for reply ..\n");
}

void Worker::replyFinished(QNetworkReply* reply){
    printf("\nStart : %s \n", QDateTime::currentDateTime().toString("hh:mm:ss").toUtf8().data());
    QString xmlStr;
    xmlStr.clear();

    if (acc->gway[gateway_count].id == 1) printf("Get Respond from --> KurayGeo\n");
    else if (acc->gway[gateway_count].id == 2) printf("Get Respond from --> ImaniPrima\n");

    xmlStr=reply->readAll();
    read.parse_xml_account_methode(xmlStr, qsql, marine, acc, acc->gway[gateway_count].id);

    gateway_count++;

    if (gateway_count < acc->sum_getway){
        this->getResponSkyW();
    }
    else{
        printf("---== Selsai ==---\n");

        printf("\nFinished : %s \n", QDateTime::currentDateTime().toString("hh:mm:ss").toUtf8().data());

        gateway_count = 0;
        timer.start((1000 * 60 * 10) / 2);
    }
}
