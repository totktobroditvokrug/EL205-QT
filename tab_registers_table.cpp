#include "mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::createRegistersTable()
{
    qDebug() << "создать таблицу регистров";
    ui->tableRegister->clear();
    ui->tableRegister->setRowCount(0); // очистить предыдущую таблицу
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
    ui->tableRegister->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->tableRegister->blockSignals(true);
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
static int selectedRow = 300;
static int selectedColumn = 300;

void MainWindow::on_tableRegister_cellDoubleClicked(int row, int column)
{
    selectedRow = row;
    selectedColumn = column;
   // qDebug() << "выбранная ячейка: " << selectedRow << " : " << selectedColumn;
}

void MainWindow::on_tableRegister_cellChanged(int row, int column)
{

    if((row == selectedRow) && (column == selectedColumn)){
        if((!!(ui->tableRegister->item(1, column))) && (!!(ui->tableRegister->item(row, column)))){
            ui->tableRegister->item(row, column)->setForeground(Qt::green);
            QString newValueString = ui->tableRegister->item(row, column)->text();
            qint16 regDataPrimary = newValueString.toShort();
//            qDebug() << "выбранная ячейка HEX:" <<  QString::number(regDataPrimary, 16).rightJustified(4, '0') << " BIN: " << QString::number(regDataPrimary, 2).rightJustified(16, '0');
            qint16 regData = changeHiLowBytes(regDataPrimary);
//            qDebug() << "смена байт HEX:" <<  QString::number(regData, 16).rightJustified(4, '0') << " BIN: " << QString::number(regData, 2).rightJustified(16, '0');
            quint8 regNumber = quint8(ui->tableRegister->item(row, 0)->text().toUInt());
//            qDebug() << "cellChanged ячейки №: "  << regNumber
//                     << "; new value=" << QString::number(changeHiLowBytes(regData), 10);

            QString commandString = AddCRC((glueAdapterHeader() + glueString(quint16(regData), regNumber)), 2).toHex();
            ui->textEdit_commandCRC->append(commandString);
            writeSerialPort(commandString);
           selectedRow = 300;
           selectedColumn = 300;
           regDataArray[regNumber].flagNewData = false;
           ui->tableRegister->item(row, column)->setText(newValueString);
            ui->tableRegister->blockSignals(true); // заблокировать фоновые события
        }

    }

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
    if( ui->tableRegister->signalsBlocked()) ui->tableRegister->blockSignals(false);
    for(int i = 0; i <= ui->tableRegister->rowCount(); i++){

        QTableWidgetItem *currentRegNum = ui->tableRegister->item(i, 0); // номер регистра из нулевого столбца

        if (!!currentRegNum) { // если ячейка не NULL
            int regNum = currentRegNum->text().toInt();

            if((regNum <= 0) & (regNum >= 255)) qDebug() << "невалидный номер регистра. regNum=" << regNum;

            if(!!regDataArray[regNum].flagNewData){ // если получили новое значение
              //  qDebug() << "у регистра №" << regNum << " обновилось значение";

              //  regDataArray[regNum].flagNewData = false; // сброс флага лишает возможности дублировать регистры



                //---- заполняем данные только если был флаг прихода нового значения регистра
                qint16 valueInt = regDataArray[regNum].value.Reg16;

                QString min = "-";
                QString max = "-";
                QString scaleValue = "-";
                QString maxValue = "-";
                QString value = QString::number(valueInt, 10); //
                QString scaledValue = "-";

//                if(regNum == 128){
//                    qDebug() << "пришел регистр 128:" <;

//                }


                    if(regDataArray[regNum].flagReg){
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
                    qint32 scaledValueInt = 0;
                    if((regDataArray[regNum].flagReg & IREGF_MAXVAL_PRESENT) && (regDataArray[regNum].flagReg & IREGF_SCALE_PRESENT) ){
                       if((regDataArray[regNum].scale.Reg16 == 0) || (regDataArray[regNum].maxValue.Reg16 == 0)){
                         //  qDebug() << "деление на ноль";
                           scaledValue = "Error";
                       }
                       else{
                           scaledValueInt = valueInt * regDataArray[regNum].scale.Reg16 / regDataArray[regNum].maxValue.Reg16;
                           scaledValue = QString::number(scaledValueInt, 10); // вывод с плавающей запятой!!!!!!!
                       }

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
        //                qDebug() << regNum << " :регистр можно редактировать";
                        // если  ячейка редактируется, не меняем ее

                        if((i == selectedRow) && (selectedColumn == 6)){
        //                    if(ui->tableRegister->item(i, 6)->isSelected()){
        //                        qDebug() << "ячейка currentRegData isSelected()";
        //                    }
        //                    ui->tableRegister->item(i, 6)->setText(ui->tableRegister->item(i, 6)->text());

                       //    qDebug() << "ячейку currentRegData можно редактировать";
                         //  connect(ui->tableRegister, SIGNAL(cellChanged(i, 6)), SLOT(tableRegister_cellChanged(i, 6)));
                        }
                        else {
                            ui->tableRegister->item(i, 6)->setText(value);
                           // qDebug() << "ячейку currentRegData перезаписали";
                        }

                        if((i == selectedRow) &&  (selectedColumn == 7)){

                          //  qDebug() << "ячейку currentRegScaledValue можно редактировать";
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


// --------------------   чтение и запись таблицы  -----------------------

void MainWindow::createTableFromFile()
{
    qDebug() << "таблица из файла";
    ui->tableFromFile->clear();
    ui->tableFromFile->setRowCount(0); // очистить предыдущую таблицу
    QStringList headers;
    headers << "№"
            << "Name"
            << "min"
            << "max"
            << "scale"
            << "maxValue"
            << "value"
            << "current"
            << "scaledValue";
    // добавить архив для data

    ui->tableFromFile->setColumnCount(9); // Указываем число колонок
    ui->tableFromFile->setShowGrid(true); // Включаем сетку
    // Разрешаем выделение только одного элемента
//    ui->tableRegister->setSelectionMode(QAbstractItemView::SingleSelection);
    // Разрешаем выделение построчно
 //   ui->tableRegister->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем заголовки колонок
    ui->tableFromFile->setHorizontalHeaderLabels(headers);
//    ui->tableRegister->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    ui->tableRegister->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    // Растягиваем последнюю колонку на всё доступное пространство
 //   ui->tableRegister->horizontalHeader()->setStretchLastSection(true);
    ui->tableFromFile->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->tableFromFile->blockSignals(true);
}

void MainWindow::addRowTableFromFile(QString lineFromFile)
{
    QStringList splitIntoColumns = lineFromFile.split(";"); // разделяем по столбцам

    if (splitIntoColumns.size() < 8) return; // выходим, если запись неполная

    int prevRowCount = ui->tableFromFile->rowCount(); // определяем текущий размер таблицы
//     qDebug() << prevRowCount << " - " << splitIntoColumns;
    ui->tableFromFile->insertRow(prevRowCount);

    QString regNumber = splitIntoColumns[0];
    QString regName = splitIntoColumns[1];
    QString min = splitIntoColumns[2];
    QString max = splitIntoColumns[3];
    QString scaleValue = splitIntoColumns[4];
    QString maxValue = splitIntoColumns[5];
    QString value = splitIntoColumns[6];
    QString currentValue = "---";
    QString scaledValue = splitIntoColumns[7];


    QTableWidgetItem *regNum = new QTableWidgetItem(regNumber);
    QTableWidgetItem *nameReg = new QTableWidgetItem(regName);
    QTableWidgetItem *currentRegMin = new QTableWidgetItem(min);
    QTableWidgetItem *currentRegMax = new QTableWidgetItem(max);
    QTableWidgetItem *currentRegScale = new QTableWidgetItem(scaleValue);
    QTableWidgetItem *currentRegMaxValue = new QTableWidgetItem(maxValue);
    QTableWidgetItem *currentRegData = new QTableWidgetItem(value);
    QTableWidgetItem *currentRegValue = new QTableWidgetItem(currentValue);
    QTableWidgetItem *currentRegScaledValue = new QTableWidgetItem(scaledValue);


    ui->tableFromFile->setItem(prevRowCount, 0, regNum);
    ui->tableFromFile->setItem(prevRowCount, 1, nameReg);
    ui->tableFromFile->setItem(prevRowCount, 2, currentRegMin);
    ui->tableFromFile->setItem(prevRowCount, 3, currentRegMax);
    ui->tableFromFile->setItem(prevRowCount, 4, currentRegScale);
    ui->tableFromFile->setItem(prevRowCount, 5, currentRegMaxValue);
    ui->tableFromFile->setItem(prevRowCount, 6, currentRegData);
    ui->tableFromFile->setItem(prevRowCount, 7, currentRegValue);
    ui->tableFromFile->setItem(prevRowCount, 8, currentRegScaledValue);

    ui->tableFromFile->item(prevRowCount, 1)->setBackground(Qt::lightGray);
    ui->tableFromFile->item(prevRowCount, 1)->setForeground(Qt::black);
    ui->tableFromFile->setColumnWidth(1, 220);


    ui->tableFromFile->item(prevRowCount, 0)->setBackground(Qt::lightGray);
    ui->tableFromFile->item(prevRowCount, 0)->setForeground(Qt::black);
    ui->tableFromFile->setColumnWidth(0, 70);

    ui->tableFromFile->item(prevRowCount, 7)->setBackground(Qt::cyan);
    ui->tableFromFile->item(prevRowCount, 7)->setForeground(Qt::black);

    // запрет редактирования и выбора ячеек по умолчанию
    ui->tableFromFile->item(prevRowCount, 0)->setFlags(Qt::NoItemFlags);
    ui->tableFromFile->item(prevRowCount, 1)->setFlags(Qt::NoItemFlags);
    ui->tableFromFile->item(prevRowCount, 2)->setFlags(Qt::NoItemFlags);
    ui->tableFromFile->item(prevRowCount, 3)->setFlags(Qt::NoItemFlags);
    ui->tableFromFile->item(prevRowCount, 4)->setFlags(Qt::NoItemFlags);
    ui->tableFromFile->item(prevRowCount, 5)->setFlags(Qt::NoItemFlags);
    ui->tableFromFile->item(prevRowCount, 6)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
    ui->tableFromFile->item(prevRowCount, 7)->setFlags(Qt::NoItemFlags);
    ui->tableFromFile->item(prevRowCount, 8)->setFlags(Qt::NoItemFlags);
//    ui->tableRegister->item(prevRowCount, 6)->setFlags(Qt::NoItemFlags);
//    ui->tableRegister->item(prevRowCount, 7)->setFlags(Qt::NoItemFlags);
}

void MainWindow::on_pushButton_saveTable_clicked()
{    QFileDialog dialogSave;
     QString pathSave = dialogSave.getSaveFileName(nullptr, "Save file", "/", "table (*.csv)");
//     qDebug() << "записываем файл с таблицей регистров: " << pathSave;

     QFile file(pathSave);
      // Открываем файл, создаем, если его не существует
     if((file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))){
         QTextStream stream(&file);
         //stream << " check write value registers";
         QString headers = "";
         QString textData = "";
         int rows = ui->tableRegister->rowCount();
         int columns = ui->tableRegister->columnCount();

         for (int j = 0; j < columns; j++) {
             headers += (ui->tableRegister->horizontalHeaderItem(j)->text() + ";");
         }
         headers.chop(1);           // убрать последнюю ;
         headers += "\n";

         for (int i = 0; i < rows; i++) {
             for (int j = 0; j < columns; j++) {

                     textData += ui->tableRegister->item(i, j)->text();
                     textData += ";";      // для .csv формата в экселе нужен разделитель ;
             }
             textData.chop(1);             // убрать последнюю ;
             textData += "\n";
         }
         stream << headers << textData;
         file.close();
         ui->statusbar->showMessage("Файл настроек сохранен в " + pathSave);
     }
     else ui->statusbar->showMessage("Ошибка сохранения файла настроек " + pathSave);
}

void MainWindow::on_pushButton_loadTable_clicked()
{
//    qDebug() << "открываем файл со значениями регистров";
    QFileDialog dialogOpen;
    QString fileName = dialogOpen.getOpenFileName(nullptr, "Выберите файл", "/", "table (*.csv)");
//    qDebug() << "Выбранный файл: " << fileName;
    QFile file(fileName);
    if(file.open(QIODevice::ReadWrite | QIODevice::Text)){
//        QTableWidget tabFromFile = ui->tableFromFile;
        createTableFromFile();
        QTextStream stream(&file);
        QString readStr = stream.readAll();
        QStringList splitIntoLines = readStr.split("\n", QString::SkipEmptyParts); // разделяем на строки таблиц
        int rows = splitIntoLines.size();
        if (rows > 1) {
            for (int i = 1; i < rows; i++){ // Считываем без заголовка
                        addRowTableFromFile(splitIntoLines[i]);
                    }
        }
        file.close();
        ui->statusbar->showMessage("Чтение файла настроек " + fileName);
        ui->lineEdit_registersFile->setText(fileName);
    }
    else ui->statusbar->showMessage("Ошибка чтения файла настроек " + fileName);
}

//-------------- работа с загруженной из файла таблицей регистров --------------


void MainWindow::checkRangeValue()
{
    int lengthTable = ui->tableFromFile->rowCount(); // определяем текущий размер таблицы
    for(int i = 0; i < lengthTable; i++){
        if((!!(ui->tableFromFile->item(i, 0))) && (!!(ui->tableFromFile->item(i, 1))) && (!!(ui->tableFromFile->item(i, 2)))
           && (!!(ui->tableFromFile->item(i, 3)))&& (!!(ui->tableFromFile->item(i, 6)))){
            int min = ui->tableFromFile->item(i, 2)->text().toInt();
            int max = ui->tableFromFile->item(i, 3)->text().toInt();
            int value = ui->tableFromFile->item(i, 6)->text().toInt();
            checkValueRegister(i, value);
         //   qDebug() << "проверка диапазона: " << min << " < " << value << " < " << max;
            if((value >= min) && (value <= max)){
                 ui->tableFromFile->item(i, 6)->setForeground(Qt::darkGreen);
                 ui->tableFromFile->item(i, 1)->setBackground(Qt::green);
                 ui->tableFromFile->item(i, 0)->setBackground(Qt::green);
            }
            else{
                ui->tableFromFile->item(i, 6)->setForeground(Qt::red);
                ui->tableFromFile->item(i, 1)->setBackground(Qt::red);
                ui->tableFromFile->item(i, 0)->setBackground(Qt::red);
            }
        }

    }
}

void MainWindow::checkValueRegister(int i, int value)   // запретить без подключения к CAN
{
    QTableWidgetItem *currentRegNum = ui->tableFromFile->item(i, 0); // номер регистра из нулевого столбца

    if (!!currentRegNum) { // если ячейка не NULL
        int regNum = currentRegNum->text().toInt();

        if((regNum <= 0) & (regNum >= 255)) qDebug() << "невалидный номер регистра. regNum=" << regNum;
        else{
            qint16 valueInt = regDataArray[regNum].value.Reg16;
            QString currentValue = QString::number(valueInt, 10); //
 //           QTableWidgetItem *currentValue = ui->tableFromFile->item(i, 7); // ячейка текущего значения
            ui->tableFromFile->item(i, 7)->setText(currentValue);
            if(value != int(valueInt)){
                ui->tableFromFile->item(i, 7)->setBackground(Qt::red);
            }
            else ui->tableFromFile->item(i, 7)->setBackground(Qt::green);
        }
    }
}

// Проверка значений в таблице на диапазон и сравнение с текущими данными из ПЧ
void MainWindow::on_pushButton_checkRegistersFromFile_clicked()
{
   qDebug() << "проверка таблицы регистров";
   checkRangeValue();
}

