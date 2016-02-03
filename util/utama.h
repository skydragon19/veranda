#ifndef UTAMA_H
#define UTAMA_H

#include <QString>

#include "QFile"
#include "QTextStream"
#include "QTime"
#include "QDateTime"

#include <QIODevice>

#include <../model/init_mysql.h>
#include <../model/veranda_log.h>

#define  MODEM_KURAYGEO   1
#define  MODEM_IMANIPRIMA 2

#define  MAX_MEASUREMENT_POINT 999

struct ship{
    int id_ship;
    char name[32];
    char modem_id[32];
};

struct utama{
    int sum_ship;
    struct ship kapal[50]; /* masih hardoce untuk alokasi 50 kapal */
};

struct getway{
    int id;
    char link[512];
    char nextutc[32];
    char access_id[16];
    char password[16];
    int SIN;
    int MIN;
    int status;

};

struct account{
    int sum_getway;
    struct getway gway[10];
};

#endif // UTAMA_H
