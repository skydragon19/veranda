#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QDateTime>

#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QSqlQuery>

#include "../util/util_skyw.h"
#include "../view/skywavenetwork.h"
#include "../model/get_db.h";

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0, QSqlDatabase db = 0);
    QXmlStreamReader xml;
signals:
    
public slots:

private slots:
    void CheckForRequest();
    void doWork();
    void getResponSkyW();
    void replyFinished(QNetworkReply* reply);

private:
    QTimer timer;
    util_skyw *read;
    get_db *get;
    //skywaveNetwork skw;

    int idx_ship;
    int sum_ship;
    int cek_replay;

    QString modem_id;
    QString access_id;
    QString password;
    QString nextutc;
    int SIN;
    int MIN;

    QSqlDatabase Qdb;

    void get_modem_info(QSqlDatabase db, int id);
};

#endif // TIMER_H
