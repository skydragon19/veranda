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
#include "../util/utama.h"

#include "../model/get_db.h";
#include "../model/init_mysql.h"
#include "../model/veranda_log.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);
    QXmlStreamReader xml;

signals:
    
public slots:

private slots:
    void doWork();
    void getResponSkyW();
    void CheckForRequest();
    void replyFinished(QNetworkReply* reply);

private:

    struct utama *marine;
    struct account *acc;

    QNetworkAccessManager *manager;

    init_mysql mysql;
    QTimer timer;
    util_skyw read;
    get_db get;
    veranda_log vlog;

    //skywaveNetwork skw;

    int cnt_panggil;

    int idx_ship;
    int sum_ship;
    int cek_replay;

    int ship_count;
    int gateway_count;

    QString urls;

    int SIN;
    int MIN;
    int xml_ver;

    QSqlDatabase db;
    QSqlQuery *qsql;

    void initNetworkManager(QFile *file);

    /** log **/
    QString fileName;
    QFile *files;
    QTextStream *outStreams;
};

#endif // TIMER_H
