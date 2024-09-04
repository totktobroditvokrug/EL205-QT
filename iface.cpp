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
     for (int i = 0; i <= (length + addEmpty) && i< IREG_INV_ALL_END_REGISTERS; i++){
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
     for (int i = 0; i <= (length + addEmpty) && i< IREG_INV_ALL_END_REGISTERS; i++){
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

QStringList SampleCanIdClass::fccanidList(){
    QStringList fc_can_idList;
    fc_can_idList.clear();


     QMetaObject metaObject = SampleCanIdClass::staticMetaObject;

     QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("fccanid"));
     int length = metaEnum.keyCount(); // без пустых строк. Так нельзя, пропускаем последние строки
     int addEmpty = 0; // добавляем при пустых линиях enum
     for (int i = 0; i <= (length + addEmpty) && i< CAN_END_SAMPLE_ID; i++){
         QString value = metaEnum.valueToKey(i);
         if(metaEnum.valueToKey(i) == nullptr){
             value = "Reserved";
             addEmpty++;
         }

         fc_can_idList.append(QString::number(i, 10) + ": " + value);
         // qDebug() << value;
     }
     return (fc_can_idList);
}


QVector<QString> SampleCanIdClass::fccanidArray(){
    QVector<QString> fc_can_idArray(CAN_END_SAMPLE_ID);
//    regnumArray.clear();

     QMetaObject metaObject = SampleCanIdClass::staticMetaObject;
     QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("fccanid"));
     int length = metaEnum.keyCount(); // без пустых строк. Так нельзя, пропускаем последние строки
     int addEmpty = 0; // добавляем при пустых линиях enum
     for (int i = 0; i <= (length + addEmpty) && i< CAN_END_SAMPLE_ID; i++){
         QString value = metaEnum.valueToKey(i);
         if(metaEnum.valueToKey(i) == nullptr){
             value = "Reserved";
             addEmpty++;
         }

         fc_can_idArray[i] = value;
       //  qDebug() << i << " " << value;
     }
     return (fc_can_idArray);
}


//-----------------Все данные стандартного CAN из парсинга uart-----------
void handleAllStandartDataCan(
        quint32 time_stamp_32,   // метка времени EL205
        QByteArray arrayDataFromCAN,
        registerFields *regDataArray,
        QVector<QString> regNumList,     // таблица названия регистров 256 значений
        samplesFields *sampleDataArray,
        QVector<QString> sampleNumList
        ){
    standartID fieldStandartID;

    //__________ Общие поля стандартного CAN____________

    // склеиваем ID сообщения todo перенести до вызова функции
    fieldStandartID.id.Body = quint8(arrayDataFromCAN[0]);
    fieldStandartID.id.Hdr = quint8(arrayDataFromCAN[1]);
    quint16 idWhole = fieldStandartID.id.Whole;

    //----------- Заполняем структуру samples стандартного CAN ----------

    if((idWhole >= SampleCanIdClass::CAN_START_SAMPLE_ID) && (idWhole <= SampleCanIdClass::CAN_END_SAMPLE_ID)){
        sampleDataArray[idWhole].time_stamp_32 = time_stamp_32; // метка времени по номеру ID
        sampleDataArray[idWhole].value.LowerByte = quint8(arrayDataFromCAN[6]);
        sampleDataArray[idWhole].value.UpperByte = quint8(arrayDataFromCAN[7]);
        sampleDataArray[idWhole].maxValue.LowerByte = quint8(arrayDataFromCAN[8]);
        sampleDataArray[idWhole].maxValue.UpperByte = quint8(arrayDataFromCAN[9]);
        sampleDataArray[idWhole].scale.LowerByte = quint8(arrayDataFromCAN[10]);
        sampleDataArray[idWhole].scale.UpperByte = quint8(arrayDataFromCAN[11]);
        sampleDataArray[idWhole].zero.LowerByte = quint8(arrayDataFromCAN[12]);
        sampleDataArray[idWhole].zero.UpperByte = quint8(arrayDataFromCAN[13]);
        sampleDataArray[idWhole].displayString = sampleNumList[idWhole] + " : " + QString::number(sampleDataArray[idWhole].value.Reg16, 10) +
                " maxValue: " + QString::number(sampleDataArray[idWhole].maxValue.Reg16, 10) +
                " scale: " + QString::number(sampleDataArray[idWhole].scale.Reg16, 10);

        //---------------- формируем данные для плоттера ---------------
        qint16 valueSample16 = sampleDataArray[idWhole].value.Reg16; // значение измерения
        qint16 scaleSample16 = sampleDataArray[idWhole].scale.Reg16;
        qint16 maxSample16 = sampleDataArray[idWhole].maxValue.Reg16;
        if ((scaleSample16 == 0) || (maxSample16 == 0)) sampleDataArray[idWhole].sampleValueScaled = double(valueSample16); // NULL;
        else sampleDataArray[idWhole].sampleValueScaled = double(valueSample16) * double(scaleSample16) / double(maxSample16);

        int counterSample = sampleDataArray[idWhole].counterSamplePlot;  // при инициализации счетчик массива сэмплов =0
        if(counterSample < PLOT_MAX_SIZE_ARR){
            if(!sampleDataArray[idWhole].flagFullBuffer){
                sampleDataArray[idWhole].sampleTimeArr.resize(counterSample + 1);  // массив значений сэмплов по оси Х
                sampleDataArray[idWhole].sampleValueScaledArr.resize(counterSample + 1);  // массив значений сэмплов по оси У
            }
            sampleDataArray[idWhole].sampleTimeArr[counterSample] = double(time_stamp_32);
            sampleDataArray[idWhole].sampleValueScaledArr[counterSample] = sampleDataArray[idWhole].sampleValueScaled;
            sampleDataArray[idWhole].counterSamplePlot++;
            if(sampleDataArray[idWhole].counterSamplePlot >= PLOT_MAX_SIZE_ARR){
                sampleDataArray[idWhole].flagFullBuffer = true; // буфер заполнен полностью
                sampleDataArray[idWhole].counterSamplePlot = 0;    // включаем перезапись буфера
                sampleDataArray[idWhole].sampleTimeArr.resize(PLOT_MAX_SIZE_ARR);
                sampleDataArray[idWhole].sampleValueScaledArr.resize(PLOT_MAX_SIZE_ARR);
            }
        }

        sampleDataArray[idWhole].flagNewData = true;
        //  qDebug() << sampleDataArray[idWhole].displayString;
    }
    else{
        //----------- Заполняем структуру регистров ---------
        switch (idWhole) {  // проверяем пришедшие данные на принадлежность к SHOW, SCALES
        case REGISTER_SHOW_ID: { // если пришел show регистр
            QString regName; // имя регистра по regNumList
            quint8 regNum = quint8(arrayDataFromCAN[6]); // если пришел регистр, то в этом месте будет его номер
            regName = regNumList[regNum].leftJustified(35, '_'); // у него будет имя из загруженного regNumList
            regDataArray[regNum].regData7 = arrayDataFromCAN.mid(7, 7); // и поле данных !!!!! проверить
            regDataArray[regNum].flagReg = quint8(regDataArray[regNum].regData7[0]); // заполняем поле флагов
            // заполняем поле данных
            regDataArray[regNum].value.UpperByte = quint8(regDataArray[regNum].regData7[2]);
            regDataArray[regNum].value.LowerByte = quint8(regDataArray[regNum].regData7[1]);
            regDataArray[regNum].time_stamp_32 = time_stamp_32; // метка времени по номеру регистра

            qint16 valueReg16 = regDataArray[regNum].value.Reg16; // значение регистра

            // вычисляем отмасштабированное значение регистра
            if((regDataArray[regNum].flagReg & IREGF_MAXVAL_PRESENT) && (regDataArray[regNum].flagReg & IREGF_SCALE_PRESENT)){
                qint16 scaleReg16 = regDataArray[regNum].scale.Reg16;
                qint16 maxReg16 = regDataArray[regNum].maxValue.Reg16;
                if ((scaleReg16 == 0) || (maxReg16 == 0)) regDataArray[regNum].regValueScaled =  double(valueReg16); //NULL; //
                else regDataArray[regNum].regValueScaled = double(valueReg16) * double(scaleReg16) / double(maxReg16);
            }
            else regDataArray[regNum].regValueScaled = double(valueReg16);


            //------------------- формируем данные для плоттера ---------------
            int counterReg = regDataArray[regNum].counterRegPlot; // при инициализации =0
            if(counterReg < PLOT_MAX_SIZE_ARR){
                if(!regDataArray[regNum].flagFullBuffer){
                    regDataArray[regNum].regTimeArr.resize(counterReg + 1);  // массив значений регистров по оси Х
                    regDataArray[regNum].regValueScaledArr.resize(counterReg + 1);  // массив значений регистров по оси У
                }
                regDataArray[regNum].regTimeArr[counterReg] = double(time_stamp_32);  // метка времени из программы парсинга
                //   regDataArray[regNum].regValueArr[regDataArray[regNum].counterRegPlot] = valueReg16;
                regDataArray[regNum].regValueScaledArr[counterReg] = regDataArray[regNum].regValueScaled;
                //    qDebug() << regDataArray[regNum].regValue[regDataArray[regNum].counterRegPlot] << regDataArray[regNum].regTime[regDataArray[regNum].counterRegPlot];
                regDataArray[regNum].counterRegPlot++;
                if(regDataArray[regNum].counterRegPlot >= PLOT_MAX_SIZE_ARR){
                    regDataArray[regNum].flagFullBuffer = true; // буфер заполнен полностью
                    regDataArray[regNum].counterRegPlot = 0;    // включаем перезапись буфера
                    regDataArray[regNum].regTimeArr.resize(PLOT_MAX_SIZE_ARR);
                    regDataArray[regNum].regValueScaledArr.resize(PLOT_MAX_SIZE_ARR);
                }
            }


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
            //---- обновим данные и по приходу шкалы и по приходу новых данных регистра
            regDataArray[regNum].flagNewData = true; // флаг получения нового значения
            break;
        }
        case REGISTER_SHOW_SCALES_ID: { // если пришел scales регистр
            quint8 regNum = quint8(arrayDataFromCAN[6]); // если пришел регистр, то в этом месте будет его номер
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
            //---- обновим данные и по приходу шкалы и по приходу новых данных регистра
            regDataArray[regNum].flagNewData = true; // флаг получения нового значения
            break;
        }
        }
    }
};


// функция смены младший-старший байт

qint16 changeHiLowBytes(qint16 dataIn){
    struct twoBytes{
        union{          // значение регистра
           qint16 Reg16;
           struct {
               qint8 LowerByte;
               qint8 UpperByte;
           };
        } value;
    };
    twoBytes direct;
    twoBytes reverse;
    direct.value.Reg16 = dataIn;
    reverse.value.LowerByte = direct.value.UpperByte;
    reverse.value.UpperByte = direct.value.LowerByte;
    qint16 dataOut = reverse.value.Reg16;
    return dataOut;
}

quint16 changeHiLowBytes_uint(quint16 dataIn){
    struct twoBytes{
        union{          // значение регистра
           quint16 Reg16;
           struct {
               quint8 LowerByte;
               quint8 UpperByte;
           };
        } value;
    };
    twoBytes direct;
    twoBytes reverse;
    direct.value.Reg16 = dataIn;
    reverse.value.LowerByte = direct.value.UpperByte;
    reverse.value.UpperByte = direct.value.LowerByte;
    quint16 dataOut = reverse.value.Reg16;
    return dataOut;
}


