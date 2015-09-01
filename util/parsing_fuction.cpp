#include "parsing_fuction.h"

parsing_function::parsing_function()
{
}

QString parsing_function::decode_base64(QString data){

    QString text_akhir;
    QByteArray ba;

    ba.append(data);

    text_akhir = QByteArray::fromBase64(ba).toHex();

    return (QString) text_akhir;
}

QString parsing_function::hex_to_bin_conversion(QString hex){
    QString biner = "";

    char dat[hex.size()+1];

    strcpy(dat, hex.toLatin1());

    /* 4 bit pertama di buang, karena hanya sbg penanda
       SIN / MIN dari data SkyWave */

    for (int i = 4; i < hex.size(); i++){
        biner.sprintf("%s%s", biner.toLocal8Bit().data(), hex_to_bin(dat[i]).toLocal8Bit().data());
    }

    return (QString) biner;
}

QString parsing_function::hex_to_bin(char dat){
    QString biner;

    if (dat == 'F' || dat == 'f') biner = "1111";
    else if (dat == 'E' || dat == 'e') biner = "1110";
    else if (dat == 'D' || dat == 'd') biner = "1101";
    else if (dat == 'C' || dat == 'c') biner = "1100";
    else if (dat == 'B' || dat == 'b') biner = "1011";
    else if (dat == 'A' || dat == 'a') biner = "1010";
    else if (dat == '9') biner = "1001";
    else if (dat == '8') biner = "1000";
    else if (dat == '7') biner = "0111";
    else if (dat == '6') biner = "0110";
    else if (dat == '5') biner = "0101";
    else if (dat == '4') biner = "0100";
    else if (dat == '3') biner = "0011";
    else if (dat == '2') biner = "0010";
    else if (dat == '1') biner = "0001";
    else if (dat == '0') biner = "0000";

    return (QString) biner;
}


QString parsing_function::format_5cut_32get(QString biner){

    char dat[(biner.size()+1)];

    int c_5bit  = 0;
    int c_32bit = 0;

    bool buang_5bit  = false;
    bool ambil_32bit = false;

    QString dat_5bit  = ""; /* 5 bit yang di buang */
    QString dat_32bit = ""; /* 32 bit yang di ambil */
    QString all_32bit = ""; /* gabung semua 32 bit */

    strcpy(dat, biner.toLatin1());

    for (int i = 0; i < biner.size(); i++){
        if (buang_5bit == false){
            dat_5bit.sprintf("%s%c", dat_5bit.toLocal8Bit().data(), dat[i]);

            if (c_5bit == 4){
                buang_5bit = true;
                c_32bit = 0;
                dat_5bit = "";
            }
            c_5bit++;
        }
        else if(buang_5bit == true){
            dat_32bit.sprintf("%s%c", dat_32bit.toLocal8Bit().data(), dat[i]);

            if (c_32bit == 31){
                all_32bit.sprintf("%s%s", all_32bit.toLocal8Bit().data(), dat_32bit.toLocal8Bit().data());

                buang_5bit = false;
                c_5bit = 0;
                dat_32bit = "";
            }
            c_32bit++;
        }
    }
    return (QString) all_32bit;
}

void parsing_function::parse_data(QSqlQuery *q, QString dat, int id_ship){
    int epochtime;
    QString dat_time;

    float data_f;
    int cnt_p = 0;
    int cnt_d = 0;
    int decimal;
    QString data = "";

    char dats[dat.size()+1];
    strcpy(dats, dat.toLatin1());

    for (int i = 0; i < dat.size(); i++){
        cnt_p++;
        data.sprintf("%s%c", data.toLocal8Bit().data(), dats[i]);

        if (cnt_p == 32){
            cnt_d++;

            decimal = bin_to_decimal(data);
            data_f = *(float *) &decimal;
    /* hasil parsing n data float */
            if (cnt_d == 1){
                epochtime = (int) data_f;
                const QDateTime time = QDateTime::fromTime_t((((int)data_f)));
                dat_time = time.toString("yyyy-MM-dd hh:mm:ss").toLocal8Bit().data();
            }
            else{
                q->clear();
                int id_tu = get.id_tu_ship(q, id_ship, cnt_d-1);
                if (id_tu != 0){
                    printf("%d --> %.2f\n", id_tu, data_f);
                    q->clear();
                    save.data(q, data_f, id_tu, 0, epochtime, dat_time);
                }
                else{
                    printf("\nbelum di set parsing_ref nya");
                }
            }
            data = "";
            cnt_p = 0;
        }
    }
}

int parsing_function::bin_to_decimal(QString dat32){
    int length = dat32.size();
    int pangkat = length-1;
    int decimal = 0;

    char x[length+1];
    strcpy(x, dat32.toLatin1());

    for (int i = 0; i < length; i++){
        if (x[i] == '1') {decimal += operasi_pangkat(pangkat);}
        pangkat--;
    }

    return (int) decimal;
}

int parsing_function::operasi_pangkat(int n){
    int hasil = 1;

    for(int i = 0; i < n; i++){
        hasil *= 2;
    }

    return (int) hasil;
}

QString parsing_function::bin_to_hex(QString biner){
    char bin[biner.size()+1];
}
