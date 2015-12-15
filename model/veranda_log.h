#ifndef VERANDA_LOG_H
#define VERANDA_LOG_H

#include "util/utama.h"


class veranda_log
{
public:
    veranda_log();

    int nomer;
    char *buf_lay;
    char *buf_no;
    char *buf_dateTime;

    void write(QFile *file, const char *text, ...);
};

#endif // VERANDA_LOG_H
