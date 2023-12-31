#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::createRegistersTable()
{
    qDebug() << "создать таблицу регистров";
    QStringList headers;
    headers << "№"
            << "Name"
            << "min"
            << "max"
            << "scale"
            << "maxValue"
            << "value"
            << "scaledValue";
    // добавить архив для data

    ui->tableRegister->setColumnCount(8); // Указываем число колонок
    ui->tableRegister->setShowGrid(true); // Включаем сетку
    // Разрешаем выделение только одного элемента
    ui->tableRegister->setSelectionMode(QAbstractItemView::SingleSelection);
    // Разрешаем выделение построчно
    ui->tableRegister->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем заголовки колонок
    ui->tableRegister->setHorizontalHeaderLabels(headers);
    // Растягиваем последнюю колонку на всё доступное пространство
    ui->tableRegister->horizontalHeader()->setStretchLastSection(true);

}

void MainWindow::addRowRegistersTable(int index, QString regName)
{
    QTableWidgetItem *nameReg = new QTableWidgetItem(regName);
    int prevRowCount = ui->tableRegister->rowCount(); // определяем текущий размер таблицы
    ui->tableRegister->insertRow(prevRowCount);
    ui->tableRegister->setItem(prevRowCount, 1, nameReg);
    ui->tableRegister->setItem(prevRowCount, 0, new QTableWidgetItem(QString::number(index, 10)));
}

void MainWindow::deleteRowRegistersTable(int index)
{
    int regNum; // номер регистра в таблице
    for(int i = 0; i <= ui->tableRegister->rowCount(); i++){
        QTableWidgetItem *item = ui->tableRegister->item(i, 0); // проверка столбца с номерами регистров
        if (!!item) { // если ячейка не NULL
           regNum = item->text().toInt();
           if(regNum == index) ui->tableRegister->removeRow(i);
        }
    }
}

//------------------Вывод значений регистров в таблицу-----------------
void MainWindow::regDisplayTable()
{
    for(int i = 0; i <= ui->tableRegister->rowCount(); i++){
        QTableWidgetItem *currentRegNum = ui->tableRegister->item(i, 0); // номер регистра
        if (!!currentRegNum) { // если ячейка не NULL
            int regNum = currentRegNum->text().toInt();
            qint16 valueInt = regDataArray[regNum].value.Reg16;

            QString min = "-";
            QString max = "-";
            QString scaleValue = "-";
            QString maxValue = "-";
            QString value = QString::number(valueInt, 10); //
            QString scaledValue = "-";

            if(regDataArray[regNum].flagReg & IREGF_MIN_PRESENT){
               min = QString::number(regDataArray[regNum].min.Reg16, 10);
            }
            if(regDataArray[regNum].flagReg & IREGF_MAX_PRESENT){
               max = QString::number(regDataArray[regNum].max.Reg16, 10);
            }
            if(regDataArray[regNum].flagReg & IREGF_SCALE_PRESENT){
               scaleValue = QString::number(regDataArray[regNum].scale.Reg16, 10);
            }
            if(regDataArray[regNum].flagReg & IREGF_MAXVAL_PRESENT){
               maxValue = QString::number(regDataArray[regNum].maxValue.Reg16, 10);
            }

            //------ расчет значение при наличии шкалы
            if((regDataArray[regNum].flagReg & IREGF_MAXVAL_PRESENT) && (regDataArray[regNum].flagReg & IREGF_SCALE_PRESENT) ){
               qint32 scaledValueInt = valueInt * regDataArray[regNum].scale.Reg16 / regDataArray[regNum].maxValue.Reg16;
               scaledValue = QString::number(scaledValueInt, 10); // вывод с плавающей запятой!!!!!!!
            }

            QTableWidgetItem *currentRegMin = new QTableWidgetItem(min);
            QTableWidgetItem *currentRegMax = new QTableWidgetItem(max);
            QTableWidgetItem *currentRegScale = new QTableWidgetItem(scaleValue);
            QTableWidgetItem *currentRegMaxValue = new QTableWidgetItem(maxValue);
            QTableWidgetItem *currentRegData = new QTableWidgetItem(value);
            QTableWidgetItem *currentRegScaledValue = new QTableWidgetItem(scaledValue);

            ui->tableRegister->setItem(i, 2, currentRegMin);
            ui->tableRegister->setItem(i, 3, currentRegMax);
            ui->tableRegister->setItem(i, 4, currentRegScale);
            ui->tableRegister->setItem(i, 5, currentRegMaxValue);
            ui->tableRegister->setItem(i, 6, currentRegData);
            ui->tableRegister->setItem(i, 7, currentRegScaledValue);
        }
    }
}
