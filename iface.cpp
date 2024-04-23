#include "iface.h"


//------------- формирование списка регистров из enum инвертора
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
        QByteArray arrayDataFromCAN,
        registerFields *regDataArray,
        QVector<QString> regNumList     // таблица названия регистров 256 значений
){
    //__________ Общие поля стандартного CAN____________
    quint8 firstByteDATA = quint8(arrayDataFromCAN[6]);

    //__________ Склеиваем два байта ID CAN____________
    regDataArray[firstByteDATA].id.Body = quint8(arrayDataFromCAN[0]);
    regDataArray[firstByteDATA].id.Hdr = quint8(arrayDataFromCAN[1]);
    quint16 idWhole = regDataArray[firstByteDATA].id.Whole;
    //__________________________________________________

    QString regName; // имя регистра по regNumList
    quint8 regNum = quint8(arrayDataFromCAN[6]);

    switch (idWhole) {  // проверяем пришедшие данные на принадлежность к SHOW, SCALES или SAMPLE
        case REGISTER_SHOW_ID: { // если пришел show регистр
            regName = regNumList[regNum].leftJustified(35, '_'); // у него будет имя из загруженного regNumList
            regDataArray[regNum].regData7 = arrayDataFromCAN.mid(7, 7); // и поле данных !!!!! проверить
            regDataArray[regNum].flagReg = quint8(regDataArray[regNum].regData7[0]); // заполняем поле флагов
            // заполняем поле данных
            regDataArray[regNum].value.UpperByte = quint8(regDataArray[regNum].regData7[2]);
            regDataArray[regNum].value.LowerByte = quint8(regDataArray[regNum].regData7[1]);
            qint16 valueReg16 = regDataArray[regNum].value.Reg16;

            // формируем строку дисплея регистра
            regDataArray[regNum].displayString = regName + " data: " + QString::number(valueReg16, 10);

            if(regDataArray[regNum].flagReg & IREGF_SCALE_PRESENT){  // если есть шкала масштаба
                regDataArray[regNum].displayString.append(" __scale: " +
                                    QString::number(quint16(regDataArray[regNum].scale.Reg16), 10));
            }

            if(regDataArray[regNum].flagReg & IREGF_MAXVAL_PRESENT){ // если есть максимальное значение переменной
               regDataArray[regNum].displayString.append(" __maxValue: " +
                                    QString::number(quint16(regDataArray[regNum].maxValue.Reg16), 10));
            }

            if(regDataArray[regNum].flagReg & IREGF_MIN_PRESENT){   // если есть минимум
                regDataArray[regNum].min.UpperByte = quint8(regDataArray[regNum].regData7[4]);
                regDataArray[regNum].min.LowerByte = quint8(regDataArray[regNum].regData7[3]);
                qint16 minReg16 = regDataArray[regNum].min.Reg16;
                regDataArray[regNum].displayString.append(" __min: " +  QString::number(minReg16, 10));
            }

            if(regDataArray[regNum].flagReg & IREGF_MAX_PRESENT){   // если есть максимум
                regDataArray[regNum].max.UpperByte = quint8(regDataArray[regNum].regData7[6]);
                regDataArray[regNum].max.LowerByte = quint8(regDataArray[regNum].regData7[5]);
                qint16 maxReg16 = regDataArray[regNum].max.Reg16;
                regDataArray[regNum].displayString.append(" __max: " +  QString::number(maxReg16, 10));
            }
            break;
         }
         case REGISTER_SHOW_SCALES_ID: { // если пришел scales регистр
            regDataArray[regNum].regScales7 = arrayDataFromCAN.mid(7, 7); // заполняем поле масштабов
            regDataArray[regNum].flagReg = quint8(regDataArray[regNum].regScales7[0]);
            if(regDataArray[regNum].flagReg & IREGF_SCALE_PRESENT){ // если есть шкала
             regDataArray[regNum].scale.UpperByte = quint8(regDataArray[regNum].regScales7[2]);
             regDataArray[regNum].scale.LowerByte = quint8(regDataArray[regNum].regScales7[1]);
            }
            if(regDataArray[regNum].flagReg & IREGF_MAXVAL_PRESENT){ // если есть максимальное значение переменной
             regDataArray[regNum].maxValue.UpperByte = quint8(regDataArray[regNum].regScales7[4]);
             regDataArray[regNum].maxValue.LowerByte = quint8(regDataArray[regNum].regScales7[3]);
            }
            break;
         }
    }


    QString data= "handleRegData. ID=" + QString::number(idWhole, 16).rightJustified(4, '0') +
            " data=" + QString::fromUtf8(regDataArray[regNum].regData7.toHex(' ')) +
            " i= "  +QString::number(regNum, 10) + " :" + regName;

    //---- обновим данные и по приходу шкалы и по приходу новых данных регистра
    regDataArray[regNum].flagNewData = true; // флаг получения нового значения
};


