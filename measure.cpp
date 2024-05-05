#include "measure.h"

#include <QTextStream>
#include <QMessageBox>

#include <QFileDialog>
#include <iostream>

measure::measure()
{

}

void measure::debug(){
    qDebug() << "--- запуск построения списка измерений инвертора ---";
    readMeasureListFromFile();
}

void measure::readMeasureListFromFile()
{
  //  qDebug() << "открываем файл с картой регистров";
    QString message = "список из файла ";
    QFileDialog dialogOpen;
    QString fileName = dialogOpen.getOpenFileName(nullptr, "Выберите файл", "/", "Текстовый файл (*.txt)");
//    qDebug() << "Выбранный файл: " << fileName;
    QFile file(fileName);
    if(file.open(QIODevice::ReadWrite | QIODevice::Text)){
        QTextStream stream(&file);
        QString readStr = stream.readAll();

        QStringList stringArray = readStr.split("\n");
        int length = stringArray.size();
//        for (int i = 0; i < length; i++) {

//           QStringList separateNum = stringArray.at(i).split(":", QString::SkipEmptyParts); // разделяем номер и имя
//           if (separateNum.size() != 2){
//              message = "Неверный формат списка регистров ";
//           }
//           else {
//               quint8 index = quint8(separateNum[0].toInt()); // номер до :
//               QString regName = separateNum[1].simplified();              // строка после :

//           }
//        }
        qDebug() << "общее количество строк файла - " << length;
        file.close();
    }
    else message = "Ошибка чтения файла ";

    qDebug() << (message + fileName);
}

