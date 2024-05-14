#include "mainwindow.h"
#include "ui_mainwindow.h"

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
            << "maxValue"
            << "scale"
            << "zero"
            << "value"
            << "scaledValue"
            << "time";
    // добавить архив для data

    ui->tableSamples->setColumnCount(8); // Указываем число колонок
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
    ui->tableSamples->setColumnWidth(1, 250);
    ui->tableSamples->setColumnWidth(0, 70);
}

void MainWindow::addRowSamplesTable(int sampleNum, QString sampleName)
{
    if(sampleName.contains("Reserved", Qt::CaseSensitive)) return;

    QTableWidgetItem *nameSample = new QTableWidgetItem(sampleName);
    int prevRowCount = ui->tableSamples->rowCount(); // определяем текущий размер таблицы
    ui->tableSamples->insertRow(prevRowCount);
    ui->tableSamples->setItem(prevRowCount, 1, nameSample);
    ui->tableSamples->item(prevRowCount, 1)->setBackground(Qt::lightGray);
    ui->tableSamples->item(prevRowCount, 1)->setForeground(Qt::black);


    ui->tableSamples->setItem(prevRowCount, 0, new QTableWidgetItem(QString::number(sampleNum, 10)));
    ui->tableSamples->item(prevRowCount, 0)->setBackground(Qt::lightGray);
    ui->tableSamples->item(prevRowCount, 0)->setForeground(Qt::black);


    QString maxValue = "-";
    QString scaleValue = "-";
    QString zero = "-";
    QString value = "-";
    QString scaledValue = "-";
    QString time = "----";

    QTableWidgetItem *currentSampleMaxValue = new QTableWidgetItem(maxValue);
    QTableWidgetItem *currentSampleScale = new QTableWidgetItem(scaleValue);
    QTableWidgetItem *currentSampleZero = new QTableWidgetItem(zero);
    QTableWidgetItem *currentSampleData = new QTableWidgetItem(value);
    QTableWidgetItem *currentSampleScaledValue = new QTableWidgetItem(scaledValue);
    QTableWidgetItem *timeStamp = new QTableWidgetItem(time);

    ui->tableSamples->setItem(prevRowCount, 2, currentSampleMaxValue );
    ui->tableSamples->setItem(prevRowCount, 3, currentSampleScale);
    ui->tableSamples->setItem(prevRowCount, 4, currentSampleZero);
    ui->tableSamples->setItem(prevRowCount, 5, currentSampleData);
    ui->tableSamples->setItem(prevRowCount, 6, currentSampleScaledValue);
    ui->tableSamples->setItem(prevRowCount, 7, timeStamp);

    // запрет редактирования и выбора ячеек по умолчанию
    ui->tableSamples->item(prevRowCount, 0)->setFlags(Qt::NoItemFlags);
    ui->tableSamples->item(prevRowCount, 1)->setFlags(Qt::NoItemFlags);
    ui->tableSamples->item(prevRowCount, 2)->setFlags(Qt::NoItemFlags);
    ui->tableSamples->item(prevRowCount, 3)->setFlags(Qt::NoItemFlags);
    ui->tableSamples->item(prevRowCount, 4)->setFlags(Qt::NoItemFlags);
    ui->tableSamples->item(prevRowCount, 5)->setFlags(Qt::NoItemFlags);
    ui->tableSamples->item(prevRowCount, 6)->setFlags(Qt::NoItemFlags);
    ui->tableSamples->item(prevRowCount, 5)->setData(Qt::TextAlignmentRole,Qt::AlignCenter);
    ui->tableSamples->item(prevRowCount, 6)->setData(Qt::TextAlignmentRole,Qt::AlignCenter);
    ui->tableSamples->item(prevRowCount, 5)->setForeground(Qt::darkRed);
    ui->tableSamples->item(prevRowCount, 6)->setForeground(Qt::darkBlue);
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
    for(int i = 0; i <= ui->tableSamples->rowCount(); i++){

        QTableWidgetItem *currentSampleNum = ui->tableSamples->item(i, 0); // номер измерения из нулевого столбца

        if (!!currentSampleNum) { // если ячейка не NULL
            int sampleNum = currentSampleNum->text().toInt();

            if((sampleNum <= 0) & (sampleNum > SampleCanIdClass::CAN_END_SAMPLE_ID)) qDebug() << "невалидный номер регистра. regNum=" << sampleNum;

            if(sampleDataArray[sampleNum].flagNewData){ // если получили новое значение
                sampleDataArray[sampleNum].flagNewData = false; // сброс флага, чтобы сэкономить время прорисовки таблицы

                //---- заполняем данные только если был флаг прихода нового значения регистра
                qint16 valueInt = sampleDataArray[sampleNum].value.Reg16;
                qint16 scaleInt = sampleDataArray[sampleNum].scale.Reg16;
                qint16 maxInt = sampleDataArray[sampleNum].maxValue.Reg16;

                QString zero = QString::number(sampleDataArray[sampleNum].zero.Reg16, 10);
                QString scaleValue = QString::number(scaleInt, 10);
                QString maxValue = QString::number(maxInt, 10);
                QString value = QString::number(valueInt, 10);
                QString scaledValue = MainWindow::scaledValue(valueInt, scaleInt, maxInt);
                QString time = QString::number(sampleDataArray[sampleNum].time_stamp_32, 10);

                ui->tableSamples->item(i, 2)->setText(maxValue );
                ui->tableSamples->item(i, 3)->setText(scaleValue);
                ui->tableSamples->item(i, 4)->setText(zero);
                ui->tableSamples->item(i, 5)->setText(value);
                ui->tableSamples->item(i, 6)->setText(scaledValue);
                ui->tableSamples->item(i, 7)->setText(time);
            }
        }
    }
}


