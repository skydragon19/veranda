#include "veranda_log.h"

veranda_log::veranda_log()
{
    nomer = 0;
    buf_lay = (char *) malloc(1024);
    buf_no = (char *) malloc(1024);
    buf_dateTime = (char *) malloc(32);
}

void veranda_log::write(QFile *file, const char *text, ...){

    va_list args;
    int i;

    va_start (args, text);

       /* For this to work, printbuffer must be larger than
        * anything we ever want to print.
        */
    i = vsprintf (buf_lay, text, args);
    va_end (args);

    /* Print the string */
    sprintf (buf_dateTime, " [%s]\r\n", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toUtf8().data(), args);
    sprintf(buf_no, "%d. ", nomer++);

    strcat(buf_lay, buf_dateTime);
    strcat(buf_no, buf_lay);

    QTextStream printLog(file);
    printLog << buf_no;
}
