#ifndef UTIL_SKYW_H
#define UTIL_SKYW_H

#include <QObject>
#include <QXmlStreamReader>
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>

#include <model/get_db.h>
#include <model/save_db.h>
#include <util/parsing_fuction.h>
#include <util/utama.h>

class util_skyw : public QObject
{
    Q_OBJECT
public:
    explicit util_skyw(QObject *parent = 0);
    void parse_xml(QString skyw, QSqlQuery *q, int id_ship, int SIN, int MIN, struct utama *kapal, int urut);

signals:
    
public slots:

private:
    get_db get;
    save_db save;
    parsing_function parse;

    int jum_dat;
};

#endif // UTIL_SKYW_H
