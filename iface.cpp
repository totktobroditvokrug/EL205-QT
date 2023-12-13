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


//--------------- добавление в поле значения регистра из адаптера -------------
QString handleRegData(
        quint8 idBody,
        quint8 idHdr,
        QByteArray standartArrayDATA,
        registerFields *regDataArray,
        quint8 regNum,
        QString regName
){
    regDataArray[regNum].regData7 = standartArrayDATA.mid(1, 7);
    regDataArray[regNum].id.Body = idBody;
    regDataArray[regNum].id.Hdr = idHdr;
    QString data= "handleRegData. ID=" + QString::number(regDataArray[regNum].id.Whole, 16) +
            " data=" + QString::fromUtf8(regDataArray[regNum].regData7.toHex(' ')) +
            " i= "  +QString::number(regNum, 10) + " :" + regName;
    return (data);
};


