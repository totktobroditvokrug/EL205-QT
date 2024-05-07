#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "measure.h"

// Поле хэш-таблиц стандартного и расширенного CAN
void MainWindow::displayHashID()
{
    QStringList canFromHash;
    canFromHash.clear();
    canFromHash.append("-- Standart CAN --");
    QHashIterator<quint16, QByteArray> iteratorStantart(canByIdStandart);
    for (auto i = canByIdStandart.cbegin(), end = canByIdStandart.cend(); i != end; ++i){
        canFromHash.append(QString::number(i.key(), 16).rightJustified(4, '0') + " : " + QString::fromUtf8(i.value().toHex(' ')));
    }
    canFromHash.append(" ");
    canFromHash.append("-- Extended CAN --");
    QHashIterator<quint32, QByteArray> iteratorExtended(canByIdExtended);
    for (auto i = canByIdExtended.cbegin(), end = canByIdExtended.cend(); i != end; ++i){
        canFromHash.append(QString::number(i.key(), 16).rightJustified(8, '0') + " : " + QString::fromUtf8(i.value().toHex(' ')));
    }
    if(ui->radioButton_fromHashTable->isChecked()) ui->textEdit_dataRead->setText(canFromHash.join('\n'));
}

void MainWindow::createSamplesTable()
{

    ui->tableSamples->clear();
    ui->tableSamples->setRowCount(0); // очистить предыдущую таблицу
    QStringList headers;
    headers << "№"
            << "Sample"
            << "value"
            << "maxValue"
            << "scale"
            << "zero"
            << "scaledValue";
    // добавить архив для data

    ui->tableSamples->setColumnCount(7); // Указываем число колонок
    ui->tableSamples->setShowGrid(true); // Включаем сетку
    // Разрешаем выделение только одного элемента
//    ui->tableRegister->setSelectionMode(QAbstractItemView::SingleSelection);
    // Разрешаем выделение построчно
 //   ui->tableRegister->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем заголовки колонок
    ui->tableSamples->setHorizontalHeaderLabels(headers);
//    ui->tableRegister->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableSamples->verticalHeader()->setVisible(false);
    // Растягиваем последнюю колонку на всё доступное пространство
 //   ui->tableRegister->horizontalHeader()->setStretchLastSection(true);
    ui->tableSamples->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->tableSamples->blockSignals(true);
}

void MainWindow::addRowSamplesTable(int sampleNum, QString sampleName)
{
    QTableWidgetItem *nameSample = new QTableWidgetItem(sampleName);
    int prevRowCount = ui->tableSamples->rowCount(); // определяем текущий размер таблицы
    ui->tableSamples->insertRow(prevRowCount);
    ui->tableSamples->setItem(prevRowCount, 1, nameSample);
    ui->tableSamples->item(prevRowCount, 1)->setBackground(Qt::lightGray);
    ui->tableSamples->setColumnWidth(1, 220);

    ui->tableSamples->setItem(prevRowCount, 0, new QTableWidgetItem(QString::number(sampleNum, 10)));
    ui->tableSamples->item(prevRowCount, 0)->setBackground(Qt::lightGray);
    ui->tableSamples->setColumnWidth(0, 70);

    QString value = "-";
    QString maxValue = "-";
    QString scaleValue = "-";
    QString zero = "-";
    QString scaledValue = "-";

    QTableWidgetItem *currentSampleZero = new QTableWidgetItem(zero);
    QTableWidgetItem *currentSampleScale = new QTableWidgetItem(scaleValue);
    QTableWidgetItem *currentSampleMaxValue = new QTableWidgetItem(maxValue);
    QTableWidgetItem *currentSampleData = new QTableWidgetItem(value);
    QTableWidgetItem *currentSampleScaledValue = new QTableWidgetItem(scaledValue);

    ui->tableSamples->setItem(prevRowCount, 2, currentSampleData);
    ui->tableSamples->setItem(prevRowCount, 3, currentSampleMaxValue);
    ui->tableSamples->setItem(prevRowCount, 4, currentSampleScale);
    ui->tableSamples->setItem(prevRowCount, 5, currentSampleZero);
    ui->tableSamples->setItem(prevRowCount, 6, currentSampleScaledValue);

    // запрет редактирования и выбора ячеек по умолчанию
    ui->tableSamples->item(prevRowCount, 0)->setFlags(Qt::NoItemFlags);
    ui->tableSamples->item(prevRowCount, 1)->setFlags(Qt::NoItemFlags);
    ui->tableSamples->item(prevRowCount, 2)->setFlags(Qt::NoItemFlags);
    ui->tableSamples->item(prevRowCount, 3)->setFlags(Qt::NoItemFlags);
    ui->tableSamples->item(prevRowCount, 4)->setFlags(Qt::NoItemFlags);
    ui->tableSamples->item(prevRowCount, 5)->setFlags(Qt::NoItemFlags);
    ui->tableSamples->item(prevRowCount, 6)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
//    ui->tableRegister->item(prevRowCount, 6)->setFlags(Qt::NoItemFlags);
}

void MainWindow::deleteRowSamplesTable(int index)
{
    int sampleNum; // номер регистра в таблице
    for(int i = 0; i <= ui->tableSamples->rowCount(); i++){
        QTableWidgetItem *item = ui->tableSamples->item(i, 0); // проверка столбца с номерами регистров
        if (!!item) { // если ячейка не NULL
           sampleNum = item->text().toInt();
           if(sampleNum == index) ui->tableSamples->removeRow(i);
        }
    }
}

//------------------Вывод значений регистров в таблицу-----------------
void MainWindow::sampleDisplayTable()
{
//    qDebug() << "Вывод измерений";
    for(int i = 0; i <= ui->tableSamples->rowCount(); i++){
//qDebug() << sampleDataArray[i].displayString;

        QTableWidgetItem *currentSampleNum = ui->tableSamples->item(i, 0); // номер измерения из нулевого столбца
//        qDebug() << currentSampleNum->text();

        if (!!currentSampleNum) { // если ячейка не NULL
            int sampleNum = currentSampleNum->text().toInt();

            if((sampleNum <= 0) & (sampleNum > FcCanIdClass::CAN_END_SAMPLE_ID)) qDebug() << "невалидный номер регистра. regNum=" << sampleNum;

            if(true){ // если получили новое значение
                //  qDebug() << "у регистра №" << regNum << " обновилось значение";

                //  regDataArray[regNum].flagNewData = false; // сброс флага лишает возможности дублировать регистры



                //---- заполняем данные только если был флаг прихода нового значения регистра
                qint16 valueInt = sampleDataArray[sampleNum].value.Reg16;

                QString zero = QString::number(sampleDataArray[sampleNum].zero.Reg16, 10);
                QString scaleValue = QString::number(sampleDataArray[sampleNum].scale.Reg16, 10);
                QString maxValue = QString::number(sampleDataArray[sampleNum].maxValue.Reg16, 10);
                QString value = QString::number(valueInt, 10); //
                QString scaledValue = "-";

                //------ расчет значение при наличии шкалы
                qint32 scaledValueInt = 0;
                if((sampleDataArray[sampleNum].scale.Reg16 == 0) || (sampleDataArray[sampleNum].maxValue.Reg16 == 0)){
                    //  qDebug() << "деление на ноль";
                    scaledValue = "Error";
                }
                else{
                    scaledValueInt = valueInt * sampleDataArray[sampleNum].scale.Reg16 / sampleDataArray[sampleNum].maxValue.Reg16;
                    scaledValue = QString::number(scaledValueInt, 10); // вывод с плавающей запятой!!!!!!!
                }
                ui->tableSamples->item(i, 2)->setText(value);
                ui->tableSamples->item(i, 3)->setText(maxValue);
                ui->tableSamples->item(i, 4)->setText(scaleValue);
                ui->tableSamples->item(i, 5)->setText(zero);
                ui->tableSamples->item(i, 6)->setText(scaledValue);
            }
        }
    }


}


