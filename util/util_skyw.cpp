#include "util_skyw.h"

util_skyw::util_skyw(QObject *parent) :
    QObject(parent)
{
}

void util_skyw::parse_xml(QString skyw, QSqlQuery *q, int id_ship, int SIN, int MIN, struct utama *marine, int urut)  {
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
                            save.data(q, data_f, id_tu, 0, epochtime, dat_time);
                        }
                        else{
                            printf("\nbelum di set parsing refnya");
                        }
                        cnt_tu++;
                    }
                }
            }


#if 0
            if (xml.name() == "MessageUTC"){
                MessageUTC = xml.readElementText();
                printf("MessageUTC : %s \n", MessageUTC.toLocal8Bit().data());
            }

            if (xml.name() == "SIN"){
                sin_xml = xml.readElementText().toInt();
                printf ("\n%d == %d\n", SIN, marine->kapal[urut].SIN);
            }

            if (xml.name() == "MobileID"){
                MobileID = xml.readElementText();
                cek_mobile.sprintf("%s", marine->kapal[urut].modem_id);
                printf("\n%s == %s\n", MobileID.toUtf8().data(), cek_mobile.toUtf8().data());
            }

            // Cek SIN
            if (MobileID == cek_mobile && sin_xml == 128){;
                q->clear();
                //save.update_next_utc(q, MessageUTC, id_ship);
                //strcpy(marine->kapal[urut].nextutc, MessageUTC.toLatin1());

                /* jika rawpayload - Imani Prima */
                if (xml.name() == "RawPayload"){
                    QString decode = parse.decode_base64(xml.readElementText());
                    //printf("%s", decode.toUtf8().data());

                    //QString bin = parse.hex_to_bin_conversion(decode);
                    //QString f_5c32g = parse.format_5cut_32get(bin);
                    //parse.parse_data(q, f_5c32g, id_ship);
                }

                /* jika Payload - Kuraygeo */
                if (xml.name() == "Fields"){
                    cnt = 0;
                    cnt_tu = 1;
                }
                if (xml.name() == "Field"){
                    QXmlStreamAttributes attributes = xml.attributes();
                    int value = attributes.value("Value").toString().toInt();

                    float data_f = *(float *) &value;

                    //printf("%d, %.2f", value, data_f);
#if 0
                    if (cnt == 0){
                        epochtime = (int) data_f;
                        printf("%d\n", epochtime);

                        const QDateTime time = QDateTime::fromTime_t((int)data_f);
                        dat_time = time.toString("yyyy-MM-dd hh:mm:ss").toLocal8Bit().data();

                        cnt = 1;
                    }
                    else{
                        q->clear();
                        int id_tu = get.id_tu_ship(q, id_ship, cnt_tu);
                        if (id_tu != 0){
                            printf("\n%d", id_tu);
                            q->clear();
                            save.data(q, data_f, id_tu, 0, epochtime, dat_time);
                        }
                        else{
                            printf("\nbelum di set parsing refnya");
                        }
                        cnt_tu++;
                    }
#endif
                }
            }
            else{
                printf("\nData tidak Valid ---> UTC : %s\n", MessageUTC.toUtf8().data());
                printf("Mobile Id : %s  --> harusnya : %s\n", MobileID.toUtf8().data(), cek_mobile.toUtf8().data());
                printf("SIN : %d --> harusnya : %d\n", sin_xml, 128);
            }
#endif
        }
    }
}
