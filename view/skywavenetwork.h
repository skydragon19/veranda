#ifndef SKYWAVENETWORK_H
#define SKYWAVENETWORK_H

//#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include <QtNetwork>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>



class skywaveNetwork : public QThread
{
    Q_OBJECT
    QNetworkAccessManager manager;
public:
    //explicit skywaveNetwork(QObject *parent = 0);
    skywaveNetwork(QObject *parent = 0);
    ~skywaveNetwork();

    void requestData(const QString idmodem);
    //void doDownload(const QUrl &url);
protected:
    void run();

signals:
    
public slots:
    void replyFinished(QNetworkReply* reply);
private:
    QMutex mutex;
    QWaitCondition cond;
    QUrl skyw;
    QNetworkReply *reply;

    //QNetworkAccessManager *nam;
};

#endif // SKYWAVENETWORK_H
