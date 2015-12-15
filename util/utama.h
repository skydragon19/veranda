#ifndef UTAMA_H
#define UTAMA_H

#include <QString>

#include "QFile"
#include "QTextStream"
#include "QTime"
#include "QDateTime"

#include <QIODevice>

#include "../model/veranda_log.h"

#define  MODEM_KURAYGEO   1
#define  MODEM_IMANIPRIMA 2

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
    int SIN;
    int MIN;
};

struct account{
    int sum_getway;
    struct getway gway[10];
};

#endif // UTAMA_H
