#include "veranda_log.h"

veranda_log::veranda_log()
{
}

void veranda_log::write(QString log){
    QString tulis;

    tulis.sprintf("echo %s >> veranda.log", log.toUtf8().data());

    system(tulis.toUtf8().data());
}
