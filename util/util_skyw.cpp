#include "util_skyw.h"

util_skyw::util_skyw(QObject *parent) :
    QObject(parent)
{
}

void util_skyw::parse_xml(QString skyw, QSqlQuery *q, int id_ship, int SIN, int MIN, struct utama *marine, int urut){
#if 0
    printf("\nChecking for => id : %d --> SIN : %d --> modem id : %s --> kapal :%s\n",
           id_ship, SIN, marine->kapal[urut].modem_id, marine->kapal[urut].name);

    int cnt = 0;
    int cnt_tu = 1;

    int epochtime;
    QString dat_time;

    QXmlStreamReader xml;

    QString MessageUTC;
    int sin_xml;
    QString MobileID;
    QString RawPayload;

    QString cek_mobile;

    QString field_name;
    int value;

    QString decode;
    QString bin;
    QString f_5c32g;

    xml.clear();
    xml.addData(skyw);

    while(!xml.atEnd() &&  !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartElement) {
            if (xml.name() == "MessageUTC"){
                MessageUTC.sprintf("%s", xml.readElementText().toUtf8().data());
            }
            if (xml.name() == "SIN"){
                sin_xml = xml.readElementText().toInt();
            }
            if (xml.name() == "MobileID"){
                MobileID.sprintf("%s", xml.readElementText().toUtf8().data());
            }

            /* validasi 0 --> match | -1 --> not match */
            if(strcmp(MobileID.toUtf8().data(), marine->kapal[urut].modem_id) == 0  && (sin_xml == marine->kapal[urut].SIN)){
                /* simpan data masuk */
                save.update_next_utc(q, MessageUTC, id_ship);

                /* SkyWave - Imani Prima */
                if (xml.name() == "RawPayload"){
                    printf("\nDate Time : %s (+07:00)\n", MessageUTC.toUtf8().data());
                    RawPayload.sprintf("%s", xml.readElementText().toUtf8().data());

                    decode.clear();
                    decode = parse.decode_base64(RawPayload);

                    bin.clear();
                    bin = parse.hex_to_bin_conversion(decode);

                    f_5c32g.clear();
                    f_5c32g = parse.format_5cut_32get(bin);

                    parse.parse_data(q, f_5c32g, id_ship);
                }

                /* SkyWave - KurayGeo */
                if (xml.name() == "Payload"){
                    printf("\nDate Time : %s (+07:00)\n", MessageUTC.toUtf8().data());
                    cnt = 0;
                    cnt_tu = 1;
                }
                if (xml.name() == "Field"){
                    QXmlStreamAttributes attributes = xml.attributes();
                    field_name = attributes.value("Name").toString();
                    value = attributes.value("Value").toString().toInt();

                    float data_f = *(float *) &value;

                    if (cnt == 0){
                        epochtime = (int) data_f;

                        const QDateTime time = QDateTime::fromTime_t((int)data_f);
                        dat_time = time.toString("yyyy-MM-dd hh:mm:ss").toLocal8Bit().data();

                        cnt = 1;
                    }
                    else{
                        q->clear();
                        int id_tu = get.id_tu_ship(q, id_ship, cnt_tu);
                        if (id_tu != 0){
                            q->clear();
                            printf("%d --> %.2f\n", id_tu, data_f);
                            save.data(q, data_f, id_tu, 0, epochtime, dat_time);
                        }
                        else{
                            printf("\nbelum di set parsing refnya");
                        }
                        cnt_tu++;
                    }
                }
            }
        }
    }
#endif
}

void util_skyw::parse_xml_account_methode(QString skyw, QSqlQuery *q, utama *marine, account *acc, int id_gateway){

    printf("ID Gateway : %d\n", id_gateway);

#if 1
    if(id_gateway == MODEM_KURAYGEO){
        parse_kureyGeo(skyw, q, marine, acc, id_gateway);
    }
    else if(id_gateway == MODEM_IMANIPRIMA){
        parse_imaniPrima(skyw, q, marine, acc, id_gateway);
    }
#endif

#if 0
    int cnt = 0;
    int cnt_tu = 1;
    int n;

    int epochtime;
    QString dat_time;

    QString MobileID;

    QXmlStreamReader xml;

    xml.clear();
    xml.addData(skyw);

    while(!xml.atEnd() &&  !xml.hasError()){
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartElement){
            if (id_gateway == MODEM_KURAYGEO){
                bool id_match = false;

                if (xml.name() == "MobileID"){
                    MobileID.sprintf("%s", xml.readElementText().toUtf8().data());
                }

                /* Filtering Mobile ID */
                for(int i = 0; i < marine->sum_ship; i++){
                    if(marine->kapal[i].modem_id == MobileID){
                        n = i;
                        id_match = true;
                    }
                }

                if(id_match){
                    if (xml.name() == "Payload"){
                        QXmlStreamAttributes attributes = xml.attributes();

                        QString name = attributes.value("Name").toString();
                        QString sin = attributes.value("SIN").toString();
                        QString min = attributes.value("MIN").toString();

                        cnt = 0;
                        cnt_tu = 1;
                    }
                    if (xml.name() == "Field"){
                        QXmlStreamAttributes attributes = xml.attributes();

                        QString name = attributes.value("Name").toString();
                        int value = attributes.value("Value").toString().toInt();

                        float data_f = *(float *) &value;

                        if (cnt == 0){
                            epochtime = (int) data_f;

                            const QDateTime time = QDateTime::fromTime_t((int)data_f);
                            dat_time = time.toString("yyyy-MM-dd hh:mm:ss").toLocal8Bit().data();

                            cnt++;
                        }
                        else{
                            q->clear();
                            int id_tu = get.id_tu_ship(q, marine->kapal[n].id_ship, cnt_tu);
                            if (id_tu != 0){
                                q->clear();
                                printf("%d --> %.2f\n", id_tu, data_f);
                                save.data(q, data_f, id_tu, 0, epochtime, dat_time);
                            }
                            else{
                                printf("\nbelum di set parsing refnya");
                            }
                            cnt_tu++;
                        }
                    }
                }
            }
            else if(id_gateway == MODEM_IMANIPRIMA){
                if (xml.name() == "MobileID"){
                    MobileID.sprintf("%s", xml.readElementText().toUtf8().data());
                    printf("MobileID => %s [Imani Prima]\n", MobileID.toUtf8().data());
                }
            }
        }

    }
#endif
}

void util_skyw::parse_kureyGeo(QString skyw, QSqlQuery *q, utama *marine, account *acc, int id_gateway){
    int cnt = 0;
    int cnt_tu = 1;
    int n;

    int SIN;

    int epochtime;
    QString dat_time;

    QString MobileID;
    QString MessageUTC;

    QXmlStreamReader xml;

    xml.clear();
    xml.addData(skyw);

    while(!xml.atEnd() &&  !xml.hasError()){
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartElement){
            bool id_match = false;

            if (xml.name() == "MessageUTC"){
                MessageUTC.sprintf("%s", xml.readElementText().toUtf8().data());
                save.update_next_utc_gateway(q, MessageUTC, id_gateway);

                strcpy(acc->gway[id_gateway-1].nextutc, MessageUTC.toLatin1());
            }

            if (xml.name() == "SIN"){
                SIN = xml.readElementText().toInt();
            }

            if (xml.name() == "MobileID"){
                MobileID.sprintf("%s", xml.readElementText().toUtf8().data());
            }

            /* Filtering Mobile ID */
            for(int i = 0; i < marine->sum_ship; i++){
                if(marine->kapal[i].modem_id == MobileID){
                    n = i;
                    id_match = true;
                }
            }

            if(id_match){
                if (xml.name() == "Payload"){
                    QXmlStreamAttributes attributes = xml.attributes();

                    QString name = attributes.value("Name").toString();
                    QString sin = attributes.value("SIN").toString();
                    QString min = attributes.value("MIN").toString();

                    cnt = 0;
                    cnt_tu = 1;
                }
                if (xml.name() == "Field"){
                    QXmlStreamAttributes attributes = xml.attributes();

                    QString name = attributes.value("Name").toString();
                    int value = attributes.value("Value").toString().toInt();

                    float data_f = *(float *) &value;

                    if (cnt == 0){
                        epochtime = (int) data_f;

                        const QDateTime time = QDateTime::fromTime_t((int)data_f);
                        dat_time = time.toString("yyyy-MM-dd hh:mm:ss").toLocal8Bit().data();

                        cnt++;
                    }
                    else{
                        q->clear();
                        int id_tu = get.id_tu_ship(q, marine->kapal[n].id_ship, cnt_tu);
                        if (id_tu != 0){
                            q->clear();
                            printf("%d --> %.2f\n", id_tu, data_f);
                            save.data(q, data_f, id_tu, 0, epochtime, dat_time);
                        }
                        else{
                            printf("\nbelum di set parsing refnya");
                        }
                        cnt_tu++;
                    }
                }
            }
        }
    }
}

void util_skyw::parse_imaniPrima(QString skyw, QSqlQuery *q, utama *marine, account *acc, int id_gateway){
    int cnt = 0;
    int cnt_tu = 1;
    int n;

    int SIN;

    int epochtime;
    QString dat_time;

    QString MobileID;
    QString MessageUTC;
    QString RawPayload;

    QString decode;
    QString bin;
    QString f_5c32g;

    QXmlStreamReader xml;

    xml.clear();
    xml.addData(skyw);

    while(!xml.atEnd() &&  !xml.hasError()){
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartElement){
            bool id_match = false;

            if (xml.name() == "MessageUTC"){
                MessageUTC.sprintf("%s", xml.readElementText().toUtf8().data());
                save.update_next_utc_gateway(q, MessageUTC, id_gateway);

                strcpy(acc->gway[id_gateway-1].nextutc, MessageUTC.toLatin1());
            }

            if (xml.name() == "SIN"){
                SIN = xml.readElementText().toInt();
            }

            if (xml.name() == "MobileID"){
                MobileID.sprintf("%s", xml.readElementText().toUtf8().data());
            }

            /* Filtering Mobile ID */
            for(int i = 0; i < marine->sum_ship; i++){
                if(marine->kapal[i].modem_id == MobileID){
                    n = i;
                    id_match = true;
                }
            }

            if(id_match){
                if(xml.name() == "RawPayload"){
                    RawPayload.clear();
                    RawPayload.sprintf("%s", xml.readElementText().toUtf8().data());

                    decode.clear();
                    decode = parse.decode_base64(RawPayload);

                    bin.clear();
                    bin = parse.hex_to_bin_conversion(decode);

                    f_5c32g.clear();
                    f_5c32g = parse.format_5cut_32get(bin);

                    parse.parse_data(q, f_5c32g, marine->kapal[n].id_ship);
                }
            }
        }
    }
}
