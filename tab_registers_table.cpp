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
//    ui->tableRegister->setSelectionMode(QAbstractItemView::SingleSelection);
    // Разрешаем выделение построчно
 //   ui->tableRegister->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем заголовки колонок
    ui->tableRegister->setHorizontalHeaderLabels(headers);
//    ui->tableRegister->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    ui->tableRegister->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    // Растягиваем последнюю колонку на всё доступное пространство
 //   ui->tableRegister->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::addRowRegistersTable(int regNum, QString regName)
{
    QTableWidgetItem *nameReg = new QTableWidgetItem(regName);
    int prevRowCount = ui->tableRegister->rowCount(); // определяем текущий размер таблицы
    ui->tableRegister->insertRow(prevRowCount);
    ui->tableRegister->setItem(prevRowCount, 1, nameReg);
    ui->tableRegister->item(prevRowCount, 1)->setBackground(Qt::lightGray);
    ui->tableRegister->setColumnWidth(1, 220);

    ui->tableRegister->setItem(prevRowCount, 0, new QTableWidgetItem(QString::number(regNum, 10)));
    ui->tableRegister->item(prevRowCount, 0)->setBackground(Qt::lightGray);
    ui->tableRegister->setColumnWidth(0, 70);

    QString min = "-";
    QString max = "-";
    QString scaleValue = "-";
    QString maxValue = "-";
    QString value = "-";
    QString scaledValue = "-";

    QTableWidgetItem *currentRegMin = new QTableWidgetItem(min);
    QTableWidgetItem *currentRegMax = new QTableWidgetItem(max);
    QTableWidgetItem *currentRegScale = new QTableWidgetItem(scaleValue);
    QTableWidgetItem *currentRegMaxValue = new QTableWidgetItem(maxValue);
    QTableWidgetItem *currentRegData = new QTableWidgetItem(value);
    QTableWidgetItem *currentRegScaledValue = new QTableWidgetItem(scaledValue);

    ui->tableRegister->setItem(prevRowCount, 2, currentRegMin);
    ui->tableRegister->setItem(prevRowCount, 3, currentRegMax);
    ui->tableRegister->setItem(prevRowCount, 4, currentRegScale);
    ui->tableRegister->setItem(prevRowCount, 5, currentRegMaxValue);
    ui->tableRegister->setItem(prevRowCount, 6, currentRegData);
    ui->tableRegister->setItem(prevRowCount, 7, currentRegScaledValue);

    // запрет редактирования и выбора ячеек по умолчанию
    ui->tableRegister->item(prevRowCount, 0)->setFlags(Qt::NoItemFlags);
    ui->tableRegister->item(prevRowCount, 1)->setFlags(Qt::NoItemFlags);
    ui->tableRegister->item(prevRowCount, 2)->setFlags(Qt::NoItemFlags);
    ui->tableRegister->item(prevRowCount, 3)->setFlags(Qt::NoItemFlags);
    ui->tableRegister->item(prevRowCount, 4)->setFlags(Qt::NoItemFlags);
    ui->tableRegister->item(prevRowCount, 5)->setFlags(Qt::NoItemFlags);
    ui->tableRegister->item(prevRowCount, 6)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
    ui->tableRegister->item(prevRowCount, 7)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
//    ui->tableRegister->item(prevRowCount, 6)->setFlags(Qt::NoItemFlags);
//    ui->tableRegister->item(prevRowCount, 7)->setFlags(Qt::NoItemFlags);
}


// контроль выбранной ячейки
static int selectedRow = 0;
static int selectedColumn = 0;

void MainWindow::on_tableRegister_cellClicked(int row, int column)
{
    selectedRow = row;
    selectedColumn = column;
    qDebug() << "выбранная ячейка: " << selectedRow << " : " << selectedColumn;
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


// void QTableWidgetItem::setFlags(Qt::ItemFlags flags)
//------------------Вывод значений регистров в таблицу-----------------
void MainWindow::regDisplayTable()
{
    for(int i = 0; i <= ui->tableRegister->rowCount(); i++){


        QTableWidgetItem *currentRegNum = ui->tableRegister->item(i, 0); // номер регистра из нулевого столбца

        if (!!currentRegNum) { // если ячейка не NULL
            int regNum = currentRegNum->text().toInt();
            qint16 valueInt = regDataArray[regNum].value.Reg16;
            if(regDataArray[regNum].flagNewData){ // если получили новое значение
                qDebug() << "у регистра №" << regNum << " обновилось значение";
                regDataArray[regNum].flagNewData = false; // сброс флага

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

                ui->tableRegister->item(i, 2)->setText(min);
                ui->tableRegister->item(i, 3)->setText(max);
                ui->tableRegister->item(i, 4)->setText(scaleValue);
                ui->tableRegister->item(i, 5)->setText(maxValue);

                //------ определить, можно ли редактировать регистр
                if(!(regDataArray[regNum].flagReg & IREGF_READONLY)){
                    ui->tableRegister->item(i, 6)->setForeground(Qt::blue);
                    ui->tableRegister->item(i, 7)->setForeground(Qt::blue);
    //                ui->tableRegister->item(i, 6)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
    //                ui->tableRegister->item(i, 7)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
                   // qDebug() << "регистр можно редактировать";
                    // если  ячейка редактируется, не меняем ее

                    if((i == selectedRow) && (selectedColumn == 6)){
    //                    if(ui->tableRegister->item(i, 6)->isSelected()){
    //                        qDebug() << "ячейка currentRegData isSelected()";
    //                    }
    //                    ui->tableRegister->item(i, 6)->setText(ui->tableRegister->item(i, 6)->text());

                       qDebug() << "ячейку currentRegData можно редактировать";
                    }
                    else {
                        ui->tableRegister->item(i, 6)->setText(value);
                       // qDebug() << "ячейку currentRegData перезаписали";
                    }

                    if((i == selectedRow) &&  (selectedColumn == 7)){

                        qDebug() << "ячейку currentRegScaledValue можно редактировать";
                    }
                    else ui->tableRegister->item(i, 7)->setText(scaledValue);
                }
                else{
                    ui->tableRegister->item(i, 6)->setFlags(Qt::NoItemFlags);
                    ui->tableRegister->item(i, 7)->setFlags(Qt::NoItemFlags);
                    ui->tableRegister->item(i, 6)->setForeground(Qt::red);
                    ui->tableRegister->item(i, 7)->setForeground(Qt::red);
                    ui->tableRegister->item(i, 6)->setText(value);
                    ui->tableRegister->item(i, 7)->setText(scaledValue);
                }
            }
        }
    }
}
