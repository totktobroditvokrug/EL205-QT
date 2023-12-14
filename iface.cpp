#include "iface.h"



QStringList RegnumClass::regnumList() // вернет строку со списком enum
{
    QStringList regnumList;
    regnumList.clear();

     QMetaObject metaObject = RegnumClass::staticMetaObject;
     QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("regnum"));
     int length = metaEnum.keyCount(); // без пустых строк. Так нельзя, пропускаем последние строки
     int addEmpty = 0; // добавляем при пустых линиях enum
     for (int i = 0; i <= (length + addEmpty) && i<= 255; i++){
         QString value = metaEnum.valueToKey(i);
         if(metaEnum.valueToKey(i) == nullptr){
             value = "Reserved";
             addEmpty++;
         }

         regnumList.append(QString::number(i, 10) + ": " + value);
         // qDebug() << value;
     }
     return (regnumList);
}

QVector<QString> RegnumClass::regnumArray()
{
    QVector<QString> regnumArray(IREG_INV_ALL_END_REGISTERS);
//    regnumArray.clear();

     QMetaObject metaObject = RegnumClass::staticMetaObject;
     QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("regnum"));
     int length = metaEnum.keyCount(); // без пустых строк. Так нельзя, пропускаем последние строки
     int addEmpty = 0; // добавляем при пустых линиях enum
     for (int i = 0; i <= (length + addEmpty) && i<= 255; i++){
         QString value = metaEnum.valueToKey(i);
         if(metaEnum.valueToKey(i) == nullptr){
             value = "Reserved";
             addEmpty++;
         }

         regnumArray[i] = value;
       //  qDebug() << i << " " << value;
     }
     return (regnumArray);
}


//-----------------Все данные стандартного CAN из парсинга uart-----------
void handleAllStandartDataCan(
        quint8 idBody,
        quint8 idHdr,
        QByteArray standartArrayDATA,
        registerFields *regDataArray,
//        quint8 regNum, заменить на standartArrayDATA[0]
//        QString regName , заменить на вектор строк
        QVector<QString> regNumList
){
    //__________ Общие поля стандартного CAN____________
    quint8 regNum = quint8(standartArrayDATA[0]);

    regDataArray[regNum].id.Body = idBody;
    regDataArray[regNum].id.Hdr = idHdr;
    quint16 idWhole = regDataArray[regNum].id.Whole;
    //__________________________________________________

    QString regName;

    switch (idWhole) {  // проверяем пришедшие данные на принадлежность к SHOW, SCALES или SAMPLE
        case REGISTER_SHOW_ID: { // если пришел show регистр
            regName = regNumList[regNum]; // у него будет имя из загруженного regNumList
            regDataArray[regNum].regData7 = standartArrayDATA.mid(1, 7); // и поле данных
            regDataArray[regNum].flagReg = quint8(regDataArray[regNum].regData7[0]); // заполняем поле флагов
            // формируем строку дисплея регистра
            regDataArray[regNum].displayString = regName + " data: " + QString::fromUtf8(regDataArray[regNum].regData7.toHex(' '));
            if(regDataArray[regNum].flagReg & IREGF_SCALE_PRESENT){ // если есть шкала
                regDataArray[regNum].scale.UpperByte = quint8(regDataArray[regNum].regData7[1]);
                regDataArray[regNum].scale.LowerByte = quint8(regDataArray[regNum].regData7[2]);
                quint16 scaleReg16 = regDataArray[regNum].scale.Reg16;
                regDataArray[regNum].displayString.append(" scale: " +  QString::number(scaleReg16, 16));
            }

//            regDataArray[regNum].displayString.append(" flag: " + QString::number(regDataArray[regNum].flagReg, 16) + " bin: " +
//                                                      QString::number(regDataArray[regNum].flagReg, 2));
            break;
         }
         case REGISTER_SHOW_SCALES_ID: { // если пришел scales регистр
            regDataArray[regNum].regScales7 = standartArrayDATA.mid(1, 7); // и поле масштабов
          //  regDataArray[regNum].displayString.append(" scale: " + QString::fromUtf8(regDataArray[regNum].regScales7.toHex(' ')));
            break;
         }
    }


    QString data= "handleRegData. ID=" + QString::number(idWhole, 16) +
            " data=" + QString::fromUtf8(regDataArray[regNum].regData7.toHex(' ')) +
            " i= "  +QString::number(regNum, 10) + " :" + regName;
};


