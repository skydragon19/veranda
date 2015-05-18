#include "util_skyw.h"

util_skyw::util_skyw(QObject *parent) :
    QObject(parent)
{
}

void util_skyw::parse_xml(QString skyw, QSqlQuery *q, int id_ship, int SIN, int MIN, struct utama *marine)  {
    int cnt = 0;
    int cnt_tu = 1;

    int epochtime;
    QString dat_time;

    int sin_xml;

    QXmlStreamReader xml;

    QString MessageUTC;

    xml.clear();
    xml.addData(skyw);

    while(!xml.atEnd() &&  !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartElement) {
            if (xml.name() == "MobileID"){

            }
            if (xml.name() == "MessageUTC"){
                MessageUTC = xml.readElementText();
                printf("    MessageUTC : %s \n", MessageUTC.toLocal8Bit().data());
            }
            if (xml.name() == "SIN"){
                sin_xml = xml.readElementText().toInt();
            }

            // Cek SIN
            if (sin_xml == 128){
                q->clear();
                save.update_next_utc(q, MessageUTC, id_ship);
                strcpy(marine->kapal[id_ship-1].nextutc, MessageUTC.toLatin1());

                // jika rawpayload
                if (xml.name() == "RawPayload"){
                    QString decode = parse.decode_base64(xml.readElementText());
                    QString bin = parse.hex_to_bin_conversion(decode);
                    QString f_5c32g = parse.format_5cut_32get(bin);
                    parse.parse_data(q, f_5c32g, id_ship);
                }

                // jika Payload
                if (xml.name() == "Fields"){
                    cnt = 0;
                    cnt_tu = 1;
                }
                if (xml.name() == "Field"){
                    QXmlStreamAttributes attributes = xml.attributes();
                    int value = attributes.value("Value").toString().toInt();

                    float data_f = *(float *) &value;

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
                }
            }
        }
    }
}
