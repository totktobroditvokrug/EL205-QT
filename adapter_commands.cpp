#include "adapter_commands.h"
#include "iface.h"
#include <QDebug>
#include <QList>
#include <QStringList>

//-------------- Добавление контрольной суммы для адаптера EL205-1 -------------
QByteArray AddCRC(QString textCommand, int indexStartByte)
{
    // textCommand - текст команды без контрольной суммы
    // indexStartByte – индекс байта, с которого считается контрольная сумма

    char checkSum = 0;

    QByteArray addCheckSum = QByteArray::fromHex(textCommand.toUtf8() ); // переводим строку в массив
//    qDebug() << "приняли для подсчета массив:" << addCheckSum.toHex() << " из строки:" << textCommand;
    int SizeCMD = addCheckSum.size();
//    qDebug() << "size=" << SizeCMD << "addCheckSum= " << addCheckSum;
    int i = 0;
    for (i = indexStartByte; i < SizeCMD; i++) // Начинаем считать для всех байт со 2-го

        {
            checkSum ^= addCheckSum[i];  // контрольная сумма по исключающему ИЛИ
//            qDebug() << "index: " << i << "data[i]=" << addCheckSum[i] << " checkSum= " << checkSum;
        }
        addCheckSum.append(checkSum);
//        qDebug() << "добавили контрольную сумму: " << addCheckSum.toHex();
    return addCheckSum;
}

//-------------- подсчет контрольной суммы для адаптера EL205-1 -------------
quint8 calcCRC(QByteArray addCheckSum)
{
    // addCheckSum - проверяемый массив
    // indexStartByte – индекс байта, с которого считается контрольная сумма

    quint8 checkSum = 0;

    int SizeCMD = addCheckSum.size();
    for (int i = 0; i < SizeCMD; i++) // Начинаем считать для всех байт
        {
            checkSum ^= addCheckSum[i];  // контрольная сумма по исключающему ИЛИ
//            qDebug() << "index: " << i << "data[i]=" << addCheckSum[i] << " checkSum= " << checkSum;
        }
    return checkSum;
}

//-------------- Обработка CAN посылки для адаптера EL205-1 --------
QString handleCAN(formatCanMessage canMessage, QString prefix)
{
    QString str = (QString::number(canMessage.numberSerialMessage, 10) + prefix + " ID_hdr:" +
                   QString::number(canMessage.id_hdr, 16) +
                   " ID_body:" + QString::number(canMessage.id_body, 16) + " length: " +
                   QString::number(canMessage.dataLen, 10) +
                   " DATA:" + QString::fromUtf8(canMessage.data.toHex(' ')));
    return (str);
}


//-------------- Обработка дежурного ответа адаптера EL205-1 -----------------------------
QString handleAdapterAnswer(QByteArray answerArrayID, quint8 lengthDataAnswer, QByteArray answerArrayDATA)
{
    QString str = ("--ansAD-- ID:" + QString::fromUtf8(answerArrayID.toHex(' ')) +
                   " length: " + QString::number(lengthDataAnswer, 10) + " DATA:" +
                   QString::fromUtf8(answerArrayDATA.toHex(' ')));
    return (str);
}

//-------------- Парсинг потока из адаптера EL205-1 -----------------------------
QStringList handleUartParsing(
    QByteArray dataRead,
    bool checkStandart,
    bool checkExtended,
    bool checkAnswer,
    QVector<QString> regNumList,  // имена регистров, взятые из enum или из файла
    registerFields *regDataArray  // эти поля регистров надо заполнить (данные, масштабы)
) {

  //  qDebug() << "handleUartParsing ";
    formatCanMessage canMessage;
    QStringList parsingDataList;
    parsingDataList.clear();
    for (int i=0; i<=(dataRead.size()-2); i++)
    {
        if ((quint8(dataRead.at(i)) == AD_COM_ID_FIRST_BYTE)) // если вероятен первый пакет
        {
            //qDebug() << "пришел стартовый байт_";
            switch (quint8(dataRead.at(i+1))) { // проверяем второй байт после стартового
                case AD_COM_ID_CAN_1 :{  // это CAN сообщение
                    if ((i+AD_COM_LENGTH_CAN-1)>dataRead.size()) // проверка длины полей сообщения
                    {
                      // qDebug() << "Неполное CAN сообщение";
                      // ui->textEdit_dataRead->append("-- Неполное CAN сообщение --");
                       parsingDataList.append("-- Неполное CAN сообщение --");
                       return (parsingDataList); // добавить склейку неполных сообщений!!!
                    }
                    else // если длина сообщения достаточная, заполняем поля ID, DATA и проверяем формат
                    {
                        // добавить проверку контрольной суммы!!!
                        QByteArray arrayDataForCRC = dataRead.mid((i+2), 20);

                        //-------- 4 байта ID_CAN, флаги, количество байт данных, 8 байт данных (с 7 по 20 позиции)

                        QByteArray arrayDataFromCAN = dataRead.mid((i+7), 14);
                        QByteArray standartArrayID = dataRead.mid((i+7), 2); // ID сообщения

                        quint8 idBody = quint8(dataRead[i+7]);  // тело идентификатора
                        quint8 idHdr = quint8(dataRead[i+8]);   // заголовок идентификатора

                        quint8 lengthDataCAN = quint8(dataRead[i+12]);  // длина сообщения

                        QByteArray standartArrayDATA = dataRead.mid((i+13), 8);
                        quint8 numberSerialMessage = quint8(dataRead[i+21]);  // номер сообщения
                        quint8 CRC = quint8(dataRead[i+22]);  // CRC

                        canMessage.numberSerialMessage = numberSerialMessage;
                        canMessage.id_body = idBody;
                        canMessage.id_hdr = idHdr;
                        canMessage.data = standartArrayDATA;
                        canMessage.dataLen = lengthDataCAN;

                        QString checkCRC = "crc-FALSE";
                        if(calcCRC(arrayDataForCRC) == CRC) checkCRC= "crc-OK";

                        //----- проверка формата CAN (расширенный/стандартный)
                        if(!(quint8(dataRead.at(i+11)) & AD_COM_EXT_CAN_FLAG)) // если стандартное сообщение
                        {
                          // qDebug() << "Стандартное CAN сообщение №"; // QString::number(quint8(dataRead[i+21]), 10);
                         //  QString standartFrame = handleStandartCAN(standartArrayID, lengthDataCAN, standartArrayDATA);
                            QString standartFrame = handleCAN(canMessage, STD_PREFIX + checkCRC);

                            //------------ заполняем поля стандартного фрэйма ----------
                           handleAllStandartDataCan(arrayDataFromCAN, regDataArray, regNumList);

                            if(checkStandart) parsingDataList.append(standartFrame);  // выводим при чекбоксе
                        }
                        else
                        {
                            //qDebug() << "Расширенное CAN сообщение №";
                            QString extendedFrame = handleCAN(canMessage, EXT_PREFIX + checkCRC);
                            if(checkExtended) parsingDataList.append(extendedFrame);  // выводим при чекбоксе
                        }
                     }
                    i=i+dataRead[i+2]+2; // перевод счетчика байт на начало следующего пакета минус 1
                   // qDebug() << "следующие два байта" << QString::number(quint8(dataRead[i+1]), 16)<< QString::number(quint8(dataRead[i+2]), 16);
                } break;
                case AD_COM_ID_ANS_1 :{
                   // qDebug() << "switch: Дежурный ответ";
                    quint8 lengthDataAnswer = quint8(dataRead[i+2]);
                    if ((i+lengthDataAnswer)>dataRead.size() && (lengthDataAnswer>6)){ // неполная посылка с ответом
                      // qDebug() << "switch: неполный дежурный ответ";
                    }
                    else{ // размер буфера данных больше или равен длине посылки
                        QByteArray answerArrayID = dataRead.mid((i+3), 4);
                        QByteArray answerArrayDATA = dataRead.mid((i+7), lengthDataAnswer-6);
                        QString adapterAnswer = handleAdapterAnswer(answerArrayID, lengthDataAnswer, answerArrayDATA);
                       // qDebug() << "пишем в QStringList дежурный ответ: " << adapterAnswer;
                        if(checkAnswer) {
                             parsingDataList.append(adapterAnswer);

                        }
                        i = i + lengthDataAnswer+2;
                    }

                } break;
                case AD_COM_ID_ERR_1START_BYTE :{
                    // ui->textEdit_dataRead->append("ERR: пропущен первый стартовый байт при передаче");
                    parsingDataList.append("ERR: пропущен первый стартовый байт при передаче");
                    i+=1;
                } break;
                case AD_COM_ID_ERR_2START_BYTE :{
                    // ui->textEdit_dataRead->append("ERR: пропущен второй стартовый байт при передаче");
                    parsingDataList.append("ERR: пропущен второй стартовый байт при передаче");
                    i+=1;
                } break;
                case AD_COM_ID_ERR_CHECK_SUM :{
                    // ui->textEdit_dataRead->append("ERR: неправильная контрольная сумма при передаче");
                    parsingDataList.append("ERR: неправильная контрольная сумма при передаче");
                    i+=1;
                } break;
                case AD_COM_ID_OVER_LENGTH :{
                    // ui->textEdit_dataRead->append("ERR: длина посылки более 40 бит");
                    parsingDataList.append("ERR: длина посылки более 40 бит");
                    i+=1;
                } break;
                case AD_COM_ID_ERR_TRANS_CAN :{
                    // ui->textEdit_dataRead->append("ERR: ошибка при передаче CAN-сообщения");
                    parsingDataList.append("ERR: ошибка при передаче CAN-сообщения");
                    i+=1;
                } break;
                default:{
                     parsingDataList.append("Это был не стартовый бит, а случайно залетевшее число, похожее на него!!!");
                //    ui->textEdit_dataRead->append("Это был не стартовый бит, а случайно залетевшее число, похожее на него!!!");
                } break;
            }
        }
    }

    return (parsingDataList);
}

