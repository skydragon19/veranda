#ifndef PARSING_FUCTION_H
#define PARSING_FUCTION_H

#include <QString>
#include <QDebug>
#include <QDateTime>

#include <model/get_db.h>
#include <model/save_db.h>

#define  GMT    25200

class parsing_function
{
public:
    parsing_function();

    QString decode_base64(QString data);
    QString hex_to_bin_conversion(QString hex);
    QString hex_to_bin(char dat);
    QString format_5cut_32get(QString biner);
    QString bin_to_hex(QString biner);

    void parse_data(QSqlDatabase db, QString dat, int id_ship);

    int bin_to_decimal(QString dat32);
    int operasi_pangkat(int n);

private:
    get_db get;
    save_db save;

};

#endif // PARSING_FUCTION_H
