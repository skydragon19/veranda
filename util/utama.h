#ifndef UTAMA_H
#define UTAMA_H

#include <QString>

struct ship{
    int id_ship;
    char modem_id[64];
    char access_id[64];
    char password[64];
    char nextutc[32];
    int SIN;
    int MIN;
    char gateway[128];
};

struct utama{
    struct ship kapal[50]; /* masih hardoce untuk alokasi 50 kapal */
};

#endif // UTAMA_H
