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
                //save.update_next_utc(q, MessageUTC, marine->kapal[n].id_ship);

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

    QString attributes_name;
    int attributes_SIN;
    int attributes_MIN;
    int tracking_data = 0;

    xml.clear();
    xml.addData(skyw);

    while(!xml.atEnd() &&  !xml.hasError()){
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartElement){
#if 1
            bool id_match = false;

            if (xml.name() == "MessageUTC"){
                MessageUTC.sprintf("%s", xml.readElementText().toUtf8().data());
                save.update_next_utc_gateway(q, MessageUTC, id_gateway);

                strcpy(acc->gway[id_gateway-1].nextutc, MessageUTC.toLatin1());
                tracking_data = 0;
                cnt_df = 0;
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
                save.update_next_utc(q, MessageUTC, marine->kapal[n].id_ship);
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

                if(xml.name() == "Payload"){
                    QXmlStreamAttributes attributes = xml.attributes();

                    attributes_name = attributes.value("Name").toString();
                    attributes_SIN = attributes.value("SIN").toString().toInt();
                    attributes_MIN = attributes.value("MIN").toString().toInt();

                    if(attributes_name == "noEIO" && attributes_SIN == 19 && attributes_MIN == 4){
                        tracking_data = 1;
                    }
                }

                if(xml.name() == "Field"){
                    if(tracking_data){
                        int data_ke = 0;
                        int get_data = 0;
                        int data_float = 0;
                        int id_tu = 0;

                        QXmlStreamAttributes field_attributes = xml.attributes();

                        QString field_name = field_attributes.value("Name").toString();
                        float field_value;
                        int epochTime;

                        if(field_name == "latitude" || field_name == "longitude"){         
                            field_value = (float) field_attributes.value("Value").toString().toFloat() / (float) 60000.0;
                            name_df[cnt_df] = field_name;
                            dat_f[cnt_df] = (float) field_value;

                            if(field_name == "latitude") data_ke = 1;
                            else data_ke = 2;

                            get_data = 1;
                            data_float = 1;
                        }
                        else if(field_name == "speed" || field_name == "heading"){
                            field_value = (float) field_attributes.value("Value").toString().toFloat() / (float) 10.0;
                            name_df[cnt_df] = field_name;
                            dat_f[cnt_df] = (float) field_value;

                            if(field_name == "speed") data_ke = 3;
                            else data_ke = 4;

                            get_data = 1;
                            data_float = 1;
                        }
                        else if(field_name == "fixTime"){
                            epochTime = (int) field_attributes.value("Value").toString().toInt();

                            data_ke = 0;
                            get_data = 1;
                            data_float = 0;
                        }

                        if(get_data){
                            if(data_float){
                               id_tu = get.id_tu_ship(q, marine->kapal[n].id_ship, data_ke);
                               tu_df[cnt_df] = id_tu;
                               cnt_df++;
                            }
                            else{
                                for(int i = 0; i < cnt_df; i++){
                                    const QDateTime time = QDateTime::fromTime_t((((int) epochTime)));

                                    printf("\n data : %s , id_tu : %d => value : %.2f ; epochtime : %d , datetime : %s", name_df[i].toUtf8().data(),
                                           tu_df[i], dat_f[i], epochTime, time.toString("yyyy-MM-dd hh:mm:ss").toUtf8().data());

                                    save.data(q, dat_f[i], tu_df[i], 0, epochTime, time.toString("yyyy-MM-dd hh:mm:ss").toUtf8().data());
                                }
                                cnt_df = 0;
                            }
                        }


                    }
                }
            }
#endif
        }
    }
}
