#include "skywavenetwork.h"


skywaveNetwork::skywaveNetwork(QObject *parent)
    : QThread(parent)
{
    //connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(downloadFinished(QNetworkReply*)));
}


void skywaveNetwork::requestData(const QString idmodem) {
    //QMutexLocker locker(&mutex);
    qDebug() << "skywaveNetwork thread ID: " << currentThreadId();
    if (!isRunning())   {
        qDebug() << "!isRunning::start";
        start();
    }
    else {
        qDebug() << "!isRunning::cond.wakeOne";
        cond.wakeOne();
    }
}

void skywaveNetwork::run(){
    QNetworkAccessManager *nam = new QNetworkAccessManager();
    qDebug() << "skywaveNetwork thread ID: " << currentThreadId();
    //nam = new QNetworkAccessManager();

    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply *)));
    qDebug()<< "skywaveNetwork::"<<__FUNCTION__;

    QNetworkRequest request;
    QUrl url =  QUrl::fromEncoded("http://isatdatapro.skywave.com/GLGW/GWServices_v1/RestMessages.svc/get_return_messages.xml/?access_id=70000214&password=STSATI2010&from_id=1450235&start_utc=2015-01-8%2000:35:00&mobile_id=01020268SKY7559");
    //QUrl url =  QUrl::fromEncoded("http://localhost");
    request.setUrl(url);
    nam->get(request);
    QDateTime dateTime = QDateTime::currentDateTime();
    qDebug()<< "skywaveNetwork::NAM request"<<dateTime.toString();
}

void skywaveNetwork::replyFinished(QNetworkReply* reply)    {
    qDebug() << "skywaveNetwork thread ID: " << currentThreadId();
    QDateTime dateTime = QDateTime::currentDateTime();
    qDebug() << "-----> SELESAI replay waktu:"<<dateTime.toString();

    QString readAll=reply->readAll();

    //util_skyw bacaxml;
    //bacaxml.baca_xml(readAll);
}

skywaveNetwork::~skywaveNetwork()   {
/*
    mutex.lock();
    quit = true;
    cond.wakeOne();
    mutex.unlock();

//*/
    wait();
    QDateTime dateTime = QDateTime::currentDateTime();
    qDebug() << "~skywaveNetwork "<< dateTime.toString() <<"------------------";
}

