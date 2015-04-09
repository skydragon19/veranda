#include "util_skyw.h"

util_skyw::util_skyw(QObject *parent) :
    QObject(parent)
{
}

void util_skyw::parse_xml(QString skyw, QSqlDatabase db, int id_ship, int SIN, int MIN)  {
    int cnt = 0;
    int cnt_tu = 1;
    QString epochtime;

    int year;
    int month;
    int day;
    int hour;
    int minute;
    int origin;

    int sin_xml;
    int min_xml;

    QString el;
    QXmlStreamReader xml;
    QByteArray text;

    QString ID;
    QString MessageUTC;
    QString MobileID;

    xml.clear();
    xml.addData(skyw);

    while(!xml.atEnd() &&  !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        /*
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }
        //*/
        if(token == QXmlStreamReader::StartElement) {

#if 0
            if(xml.name() == "ID") {
                ID = xml.readElementText();
            }
            if(xml.name() == "ReceiveUTC") {
                ReceiveUTC = xml.readElementText();
            }
            if(xml.name() == "SIN") {
                qDebug()<<"  xmlname: "<<xml.name().toString()<<":"<<xml.readElementText();
            }
            if(xml.name() == "MobileID") {
                MobileID = xml.readElementText();
            }
            if(xml.name() == "Payload"){
                cnt = 0;
                cnt_tu = 1;
            }
#endif
            if (xml.name() == "MobileID"){
                qDebug() << "  MobileID : "<<xml.readElementText();
            }
            if (xml.name() == "MessageUTC"){
                MessageUTC = xml.readElementText();
                save.update_next_utc(db, MessageUTC, id_ship);
            }
            if (xml.name() == "Payload"){
                QXmlStreamAttributes attributes = xml.attributes();
                sin_xml = attributes.value("SIN").toString().toInt();
                min_xml = attributes.value("MIN").toString().toInt();
            }
            if (sin_xml == SIN && min_xml == MIN){
                if(xml.name() == "Fields"){
                    cnt = 0;
                    cnt_tu = 1;
                }
                if(xml.name() == "Field") {
                    QXmlStreamAttributes attributes = xml.attributes();
                    int value = attributes.value("Value").toString().toInt();

                    float data_f = *(float *) &value;

                    if (cnt == 0){
                        const QDateTime time = QDateTime::fromTime_t((int)data_f);

                        epochtime = time.toString("yyyy-MM-dd hh:mm:ss").toAscii().data();

                        year = time.toString("yyyy").toInt();
                        month = time.toString("MM").toInt();
                        day = time.toString("dd").toInt();
                        hour = time.toString("hh").toInt();
                        minute = time.toString("mm").toInt();
                        origin = time.toString("ss").toInt();

                        cnt = 1;
                    }
                    else{
                        int id_tu = get.id_tu_ship(db, id_ship, cnt_tu);
                        if (id_tu != 0){
                            save.data(db, data_f, id_tu, 0, epochtime, year, month, day, hour, minute, origin);
                        }
                        cnt_tu++;
                    }
                }
            }
        }
    }
}
