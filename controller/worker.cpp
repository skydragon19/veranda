#include "worker.h"

Worker::Worker(QObject *parent) : QObject(parent){
    fileName.sprintf("%s/log/log.txt", QDir::currentPath().toUtf8().data());

    QFile file(fileName);

    files = &file;
    files->open(QIODevice::WriteOnly);

    this->initNetworkManager(files);

    db = mysql.connect_db();

    connect(&timer, SIGNAL(timeout()), this, SLOT(doWork()));
    timer.start((1000 * 60 * 10) / 2); /* 5 menit */

    marine = (struct utama *) malloc( sizeof (struct utama));
    memset((char *) marine, 0, sizeof(struct utama));

    acc = (struct account *) malloc ( sizeof (struct account));
    memset((char *) acc, 0, sizeof(struct account));

    get.modem_info(db, marine);
    get.modem_getway(db, acc);

    ship_count = 0;
    gateway_count = 0;
    cnt_panggil = 0;

    this->doWork();
}

void Worker::initNetworkManager(QFile *file){
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

    QUrl url =  QUrl::fromEncoded(urls.toLocal8Bit().data());

    request.setUrl(url);

    manager->get(request);
}

void Worker::replyFinished(QNetworkReply* reply){
    QString xmlStr;
    xmlStr.clear();

    xmlStr=reply->readAll();
    read.parse_xml_account_methode(xmlStr, db, marine, acc, acc->gway[gateway_count].id, gateway_count);

    gateway_count++;

    if (gateway_count < acc->sum_getway){
        this->getResponSkyW();
    }
    else{
        gateway_count = 0;
        timer.start((1000 * 60 * 10) / 2);
    }
}
