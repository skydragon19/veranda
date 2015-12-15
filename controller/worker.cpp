#include "worker.h"

Worker::Worker(QObject *parent) : QObject(parent){
    fileName.sprintf("%s/log/log.txt", QDir::currentPath().toUtf8().data());

    QFile file(fileName);

    files = &file;
    files->open(QIODevice::WriteOnly);

    vlog.write(files, "Veranda Satelite Parser is Running");

    vlog.write(files, "Initialization Network");
    this->initNetworkManager(files);

    vlog.write(files, "Initialization Database");
    db = mysql.connect_db(files);
    qsql = new QSqlQuery(db);

    connect(&timer, SIGNAL(timeout()), this, SLOT(doWork()));
    timer.start((1000 * 60 * 10) / 2); /* 5 menit */

    marine = (struct utama *) malloc( sizeof (struct utama));
    memset((char *) marine, 0, sizeof(struct utama));

    acc = (struct account *) malloc ( sizeof (struct account));
    memset((char *) acc, 0, sizeof(struct account));

    vlog.write(files, "Initialization memory");
    qsql->clear();
    get.modem_info(qsql, marine, files);
    get.modem_getway(qsql, acc, files);

    ship_count = 0;
    gateway_count = 0;
    cnt_panggil = 0;

    vlog.write(files, "Start Process");
    this->doWork();
}

void Worker::initNetworkManager(QFile *file){
    manager = new QNetworkAccessManager();
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply *)));

    vlog.write(file, "Success Initialization Network Manager");
}

void Worker::CheckForRequest(){

}

void Worker::doWork(){
    timer.stop();
    vlog.write(files, "Stop Timer (temporary)");

    this->getResponSkyW();
}


void Worker::getResponSkyW(){
    QNetworkRequest request;

    urls.sprintf("%s%s", acc->gway[gateway_count].link, acc->gway[gateway_count].nextutc);
    vlog.write(files, "request to urls :\%s", urls.toUtf8().data());


    QUrl url =  QUrl::fromEncoded(urls.toLocal8Bit().data());

    request.setUrl(url);

    manager->get(request);

    vlog.write(files, "Waiting for replay :");
}

void Worker::replyFinished(QNetworkReply* reply){
    QString xmlStr;
    xmlStr.clear();

    xmlStr=reply->readAll();
    read.parse_xml_account_methode(xmlStr, qsql, marine, acc, acc->gway[gateway_count].id, files);

    gateway_count++;

    if (gateway_count < acc->sum_getway){
        this->getResponSkyW();
    }
    else{
        gateway_count = 0;
        timer.start((1000 * 60 * 10) / 2);
    }
}
