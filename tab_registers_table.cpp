#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "invertor.h"
#include "iface.h"
#include "stylehelper.h"


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
  //  qDebug() << "создать таблицу регистров";
    ui->tableRegister->clear();
    ui->tableRegister->setRowCount(0); // очистить предыдущую таблицу
    QStringList headers;
    headers << "№"
            << "Register"
            << "min"
            << "max"
            << "scale"
            << "maxValue"
            << "value"
            << "scaledValue";
    // добавить архив для data

    ui->tableRegister->setColumnCount(8); // Указываем число колонок
    ui->tableRegister->setShowGrid(true); // Включаем сетку
    // Разрешаем выделение построчно
 //   ui->tableRegister->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем заголовки колонок
    ui->tableRegister->setHorizontalHeaderLabels(headers);
//    ui->tableRegister->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableRegister->verticalHeader()->setVisible(false);
    // Растягиваем последнюю колонку на всё доступное пространство
 //   ui->tableRegister->horizontalHeader()->setStretchLastSection(true);
    ui->tableRegister->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->tableRegister->blockSignals(true);
}

void MainWindow::addRowRegistersTable(int regNum, QString regName)
{
    if(regName.contains("Reserved", Qt::CaseSensitive)) return;
    QTableWidgetItem *nameReg = new QTableWidgetItem(regName);
    int prevRowCount = ui->tableRegister->rowCount(); // определяем текущий размер таблицы
    ui->tableRegister->insertRow(prevRowCount);
    ui->tableRegister->setItem(prevRowCount, 1, nameReg);
    ui->tableRegister->item(prevRowCount, 1)->setBackground(Qt::lightGray);
    ui->tableRegister->item(prevRowCount, 1)->setForeground(Qt::black);
    ui->tableRegister->setColumnWidth(1, 220);

    ui->tableRegister->setItem(prevRowCount, 0, new QTableWidgetItem(QString::number(regNum, 10)));
    ui->tableRegister->item(prevRowCount, 0)->setBackground(Qt::lightGray);
    ui->tableRegister->item(prevRowCount, 0)->setForeground(Qt::black);
    ui->tableRegister->setColumnWidth(0, 70);

    ui->tableRegister->setColumnWidth(7, 160);

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
    ui->tableRegister->item(prevRowCount, 6)->setData(Qt::TextAlignmentRole,Qt::AlignCenter);
    ui->tableRegister->item(prevRowCount, 7)->setData(Qt::TextAlignmentRole,Qt::AlignCenter);
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
//            ui->textEdit_commandCRC->append(commandString);
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
    checkInvertorStatus();
    getFreqInv();
    setRegistersCombobox();

    if( ui->tableRegister->signalsBlocked()) ui->tableRegister->blockSignals(false);
    for(int i = 0; i <= ui->tableRegister->rowCount(); i++){

        QTableWidgetItem *currentRegNum = ui->tableRegister->item(i, 0); // номер регистра из нулевого столбца

        if (!!currentRegNum) { // если ячейка не NULL
            int regNum = currentRegNum->text().toInt();

            if((regNum <= 0) & (regNum >= 255)) qDebug() << "невалидный номер регистра. regNum=" << regNum;

            if(!!regDataArray[regNum].flagNewData){ // если получили новое значение

                //  regDataArray[regNum].flagNewData = false; // сброс флага лишает возможности дублировать регистры


                //---- заполняем данные только если был флаг прихода нового значения регистра
                qint16 valueInt = regDataArray[regNum].value.Reg16;

                QString min = "-";
                QString max = "-";
                QString scaleValue = "-";
                QString maxValue = "-";
                QString value = QString::number(valueInt, 10); //
                QString scaledValue = "-";

                // проверка на наличие соответствующего регистру шкалы и диапазонов
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
                    scaledValue = MainWindow::scaledValue(valueInt, regDataArray[regNum].scale.Reg16, regDataArray[regNum].maxValue.Reg16);
                }

             //   если это регистры статуса, выдаем в scaledValue двоичными тетрадами
                if(regNumList[regNum].contains("STATUS", Qt::CaseSensitive)){
                     scaledValue = QString::number(valueInt, 2).rightJustified(16, '0');
                     scaledValue.insert(4, " ").insert(9, " ").insert(14, " ");
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
   // qDebug() << "таблица из файла";
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
    // Устанавливаем заголовки колонок
    ui->tableFromFile->setHorizontalHeaderLabels(headers);
    ui->tableFromFile->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->tableFromFile->blockSignals(true);
}


void MainWindow::addRowTableFromFile(QString lineFromFile)
{
    QStringList splitIntoColumns = lineFromFile.split(";"); // разделяем по столбцам

    if (splitIntoColumns.size() < 8){
       ui->statusbar->showMessage("Неверный формат строки");
       return; // выходим, если запись неполная
    }

    int prevRowCount = ui->tableFromFile->rowCount(); // определяем текущий размер таблицы
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
}


void MainWindow::on_pushButton_saveTable_clicked()
{    QFileDialog dialogSave;
     QString pathSave = dialogSave.getSaveFileName(nullptr, "Save file", workDirPath, "table (*.csv)");

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
    QFileDialog dialogOpen;
    QString fileName = dialogOpen.getOpenFileName(nullptr, "Выберите файл", workDirPath, "table (*.csv)");
    QFile file(fileName);
    if(file.open(QIODevice::ReadWrite | QIODevice::Text)){
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

void MainWindow::checkValueRegister(int i, int value)   // запретить без подключения к CAN
{
    QTableWidgetItem *currentRegNum = ui->tableFromFile->item(i, 0); // номер регистра из нулевого столбца

    if (!!currentRegNum) { // если ячейка не NULL
        int regNum = currentRegNum->text().toInt();

        if((regNum <= 0) & (regNum >= 255)) qDebug() << "невалидный номер регистра. regNum=" << regNum;
        else{
            qint16 valueInt = regDataArray[regNum].value.Reg16;
            QString currentValue = QString::number(valueInt, 10); //
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
    int lengthTable = ui->tableFromFile->rowCount(); // определяем текущий размер таблицы
    for(int i = 0; i < lengthTable; i++){
        if((!!(ui->tableFromFile->item(i, 0))) && (!!(ui->tableFromFile->item(i, 1))) && (!!(ui->tableFromFile->item(i, 2)))
           && (!!(ui->tableFromFile->item(i, 3)))&& (!!(ui->tableFromFile->item(i, 6)))){
            int min = ui->tableFromFile->item(i, 2)->text().toInt();
            int max = ui->tableFromFile->item(i, 3)->text().toInt();
            int value = ui->tableFromFile->item(i, 6)->text().toInt();
            checkValueRegister(i, value);
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
    ui->statusbar->showMessage("Сравнение карты регистров с ПЧ");
}


// Запись значений регистров из файла в ПЧ
void MainWindow::on_pushButton_setRegistersFromFile_clicked()
{
    QString commandString = "";
    for(int i = 0; i< ui->tableFromFile->rowCount(); i++){
        if(!!(ui->tableFromFile->item(i, 6))){
            ui->tableFromFile->item(i, 6)->setForeground(Qt::black);
            ui->tableFromFile->item(i, 6)->setBackground(Qt::yellow);
            ui->tableFromFile->item(i, 7)->setBackground(Qt::yellow);
            QString newValueString = ui->tableFromFile->item(i, 6)->text();
            qint16 regDataPrimary = newValueString.toShort();
            qint16 regData = changeHiLowBytes(regDataPrimary);
            quint8 regNumber = quint8(ui->tableFromFile->item(i, 0)->text().toUInt());
            commandString += AddCRC((glueAdapterHeader() + glueString(quint16(regData), regNumber)), 2).toHex();
        }
    }
    QTimer::singleShot(5000, this, SLOT(on_pushButton_checkRegistersFromFile_clicked())); // запустить с задержкой повторную проверку на соответствие регистров
    writeSerialPort(commandString);
    ui->statusbar->showMessage("Ожидание записи регистров в ПЧ");
}


//---------------------------------- Быстрое меню и кнопки ------------------------

//---------------- Запись регистра через слайдер
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    int regNum;
    switch (selectedComboBox) {
    case 1:{
        regNum = ui->comboBox_register_1->currentIndex();
        setSelectedRegisterSlider(regNum);
        ui->lineEdit_registerValue_1->setText(getRegisterInv(regNum, qint16(value)));
    }
    break;
    case 2: {
        regNum = ui->comboBox_register_2->currentIndex();
        setSelectedRegisterSlider(regNum);
        ui->lineEdit_registerValue_2->setText(getRegisterInv(regNum, qint16(value)));
    }
    break;
    case 3: {
        regNum = ui->comboBox_register_3->currentIndex();
        setSelectedRegisterSlider(regNum);
        ui->lineEdit_registerValue_3->setText(getRegisterInv(regNum, qint16(value)));
    }
    break;
    }
}


// по сигналу отпускания слайдера делаем запись его значения в регистр
void MainWindow::on_horizontalSlider_sliderReleased()
{
    int regNum = 0;
    switch (selectedComboBox) {
    case 1:{
        regNum = ui->comboBox_register_1->currentIndex();
    }
    break;
    case 2: {
        regNum = ui->comboBox_register_2->currentIndex();
    }
    break;
    case 3: {
        regNum = ui->comboBox_register_3->currentIndex();
    }
    break;
    }
    qint16 regDataPrimary = qint16(ui->horizontalSlider->value());
    qint16 regData = changeHiLowBytes(regDataPrimary);

    QString commandString = AddCRC((glueAdapterHeader() + glueString(quint16(regData), quint8(regNum))), 2).toHex();

    writeSerialPort(commandString);
}

void MainWindow::on_lineEdit_registerValue_1_selectionChanged() //  было выбрано поле №1
{
    int regNum = ui->comboBox_register_1->currentIndex();
    selectedComboBox = 1;

    ui->horizontalSlider->setValue(regDataArray[regNum].value.Reg16);
    //------ определить, можно ли редактировать регистр
    if(!(regDataArray[regNum].flagReg & IREGF_READONLY)){
        ui->horizontalSlider->setEnabled(true);
    }
    else{
         ui->horizontalSlider->setEnabled(false);
    }
}

void MainWindow::on_lineEdit_registerValue_2_selectionChanged() //  было выбрано поле №2
{
    int regNum = ui->comboBox_register_2->currentIndex();
    selectedComboBox = 2;

    ui->horizontalSlider->setValue(regDataArray[regNum].value.Reg16);
    //------ определить, можно ли редактировать регистр
    if(!(regDataArray[regNum].flagReg & IREGF_READONLY)){
        ui->horizontalSlider->setEnabled(true);
    }
    else{
         ui->horizontalSlider->setEnabled(false);
    }
}


void MainWindow::on_lineEdit_registerValue_3_selectionChanged()  // было выбрано поле №3
{
    int regNum = ui->comboBox_register_3->currentIndex();
    selectedComboBox = 3;

    ui->horizontalSlider->setValue(regDataArray[regNum].value.Reg16);
    //------ определить, можно ли редактировать регистр
    if(!(regDataArray[regNum].flagReg & IREGF_READONLY)){
        ui->horizontalSlider->setEnabled(true);
    }
    else{
         ui->horizontalSlider->setEnabled(false);
    }
}


void MainWindow::initComboBoxRegister()
{
    selectedComboBox = 0; // текущий комбобокс- частота инвертора
    QStringList registersList = RegnumClass::regnumList();

    ui->comboBox_register_1->addItems(registersList);
    ui->comboBox_register_2->addItems(registersList);
    ui->comboBox_register_3->addItems(registersList);


    ui->comboBox_register_1->setCurrentIndex(RegnumClass::IREG_UOUT);
    ui->comboBox_register_2->setCurrentIndex(RegnumClass::IREG_IOUT);
    ui->comboBox_register_3->setCurrentIndex(RegnumClass::IREG_FREQ_REF);

    ui->horizontalSlider->setEnabled(false);
    ui->lineEdit_registerValue_1->setEnabled(false);
    ui->lineEdit_registerValue_2->setEnabled(false);
    ui->lineEdit_registerValue_3->setEnabled(false);

    on_pushButton_showPanel_clicked(); // по умолчанию показать панель управления
    ui->pushButton_showPanel->setIcon(QIcon());
}

 //------ расчет значение частоты
void MainWindow::getFreqInv(){     
      ui->lineEdit_currentFreq->setText(getRegisterInv(RegnumClass::IREG_FREQ, regDataArray[RegnumClass::IREG_FREQ].value.Reg16));
}


//------ расчет значения регистра с учетом шкалы
QString MainWindow::getRegisterInv(int regNum, qint16 valueInt){
    if((regDataArray[regNum].flagReg & IREGF_MAXVAL_PRESENT) && (regDataArray[regNum].flagReg & IREGF_SCALE_PRESENT) ){
        return MainWindow::scaledValue(valueInt, regDataArray[regNum].scale.Reg16, regDataArray[regNum].maxValue.Reg16);
     }
     return QString::number(valueInt, 10); // без учета шкалы
}


//------ установить значения трех выбранных регистров быстрого доступа с учетом шкалы
void MainWindow::setRegistersCombobox(){
    // по выбранным в комбобоксе регистрам смотрим их значения
    int regNum_1 = ui->comboBox_register_1->currentIndex();
    int regNum_2 = ui->comboBox_register_2->currentIndex();
    int regNum_3 = ui->comboBox_register_3->currentIndex();
    qint16 valueInt_1 = regDataArray[regNum_1].value.Reg16;
    qint16 valueInt_2 = regDataArray[regNum_2].value.Reg16;
    qint16 valueInt_3 = regDataArray[regNum_3].value.Reg16;

    //------ определить, можно ли редактировать регистр
    if(!(regDataArray[regNum_1].flagReg & IREGF_READONLY)){
        if(selectedComboBox == 1) ui->lineEdit_registerValue_1->setStyleSheet(StyleHelper::getSlidersSelectedStyle());
        else ui->lineEdit_registerValue_1->setStyleSheet(StyleHelper::getReadWriteStyle());
    }
    else{
         ui->lineEdit_registerValue_1->setStyleSheet(StyleHelper::getReadOnlyStyle());
    }

    if(!(regDataArray[regNum_2].flagReg & IREGF_READONLY)){
        if(selectedComboBox == 2) ui->lineEdit_registerValue_2->setStyleSheet(StyleHelper::getSlidersSelectedStyle());
        else ui->lineEdit_registerValue_2->setStyleSheet(StyleHelper::getReadWriteStyle());
    }
    else{
         ui->lineEdit_registerValue_2->setStyleSheet(StyleHelper::getReadOnlyStyle());
    }

    if(!(regDataArray[regNum_3].flagReg & IREGF_READONLY)){
        if(selectedComboBox == 3) ui->lineEdit_registerValue_3->setStyleSheet(StyleHelper::getSlidersSelectedStyle());
        else ui->lineEdit_registerValue_3->setStyleSheet(StyleHelper::getReadWriteStyle());
    }
    else{
         ui->lineEdit_registerValue_3->setStyleSheet(StyleHelper::getReadOnlyStyle());
    }

    // если работает слайдер, не пишем текущее значение трех регистров быстрого доступа
    if(!(ui->horizontalSlider->underMouse())){
        ui->lineEdit_registerValue_1->setText(getRegisterInv(regNum_1, valueInt_1));
        ui->lineEdit_registerValue_2->setText(getRegisterInv(regNum_2, valueInt_2));
        ui->lineEdit_registerValue_3->setText(getRegisterInv(regNum_3, valueInt_3));
    }

    ui->pushButton_showPanel->setText("     F = " + getRegisterInv(RegnumClass::IREG_FREQ, regDataArray[RegnumClass::IREG_FREQ].value.Reg16) +
                                      "      " + ui->comboBox_register_1->currentText() + " = " + getRegisterInv(regNum_1, valueInt_1) +
                                      "      " + ui->comboBox_register_2->currentText() + " = " + getRegisterInv(regNum_2, valueInt_2) +
                                      "      " + ui->comboBox_register_3->currentText() + " = " + getRegisterInv(regNum_3, valueInt_3));
}


//------ установить границы слайдера
void MainWindow::setSelectedRegisterSlider(int regNum){
    ui->horizontalSlider->setMinimum(-32767);
    ui->horizontalSlider->setMaximum(32767);

    if(regDataArray[regNum].flagReg & IREGF_MIN_PRESENT){
        ui->horizontalSlider->setMinimum(regDataArray[regNum].min.Reg16);
    }

    if(regDataArray[regNum].flagReg & IREGF_MAX_PRESENT){
        ui->horizontalSlider->setMaximum(regDataArray[regNum].max.Reg16);
    }
//    if(regDataArray[regNum].flagReg & IREGF_MAXVAL_PRESENT){
//       ui->horizontalSlider->setMaximum(regDataArray[regNum].maxValue.Reg16);
//    }
}


//-------------------- панель управления ----------------------

//---------- кнопки управления

void MainWindow::on_pushButton_showPanel_clicked()
{
    ui->groupBox_controlPanel->setVisible(true);
    ui->pushButton_showPanel->setVisible(false);
}

void MainWindow::on_pushButton_hidePanel_clicked()
{
    ui->groupBox_controlPanel->setVisible(false);
    ui->pushButton_showPanel->setVisible(true);
}

void MainWindow::on_pushButton_startInv_clicked()
{
    QString commandString = AddCRC((glueAdapterHeader() + glueString(INV_CTRL_START, IREG_INV_CONTROL)), 2).toHex();
    ui->textEdit_commandCRC->append(commandString);
    writeSerialPort(commandString);
}

void MainWindow::on_pushButton_stopInv_clicked()
{
    QString commandString = AddCRC((glueAdapterHeader() + glueString(INV_CTRL_STOP, IREG_INV_CONTROL)), 2).toHex();
    ui->textEdit_commandCRC->append(commandString);
    writeSerialPort(commandString);
}

void MainWindow::on_pushButton_alarmInv_clicked()
{
    QString commandString = AddCRC((glueAdapterHeader() + glueString(INV_CTRL_ALARM, IREG_INV_CONTROL)), 2).toHex();
    ui->textEdit_commandCRC->append(commandString);
    writeSerialPort(commandString);
}

//---------- проверка статуса работы инвертора
void MainWindow::checkInvertorStatus()
{
    QString currentStatus = "";
    qint16 invStatus = regDataArray[RegnumClass::IREG_INV_STATUS].value.Reg16;
    if (invStatus & INV_STS_STARTED){

      if (invStatus & INV_STS_TO_STOP_MODE) {
          ui->pushButton_startInv->setStyleSheet(StyleHelper::getWaitButtonStyle());
          ui->pushButton_showPanel->setIcon(QIcon(":/images/wait_small.png"));
      }
      else{
         ui->pushButton_startInv->setStyleSheet(StyleHelper::getStartedButtonStyle());
         ui->pushButton_showPanel->setIcon(QIcon(":/images/start_small.png"));
      }
      currentStatus += ("Система запущена \n");
    }
    else{
      ui->pushButton_startInv->setStyleSheet(StyleHelper::getStartButtonStyle());
      ui->pushButton_showPanel->setIcon(QIcon(":/images/stop_small.png"));
      currentStatus += ("Система остановлена \n");
    }
    if (invStatus & INV_STS_WAIT_RECT_START) currentStatus += ("Ожидает запуска выпрямителя \n");
    if (invStatus & INV_STS_STOPPED_REGISTERS) currentStatus += ("Остановлен по изменению важного регистра \n");
    if (invStatus & INV_STS_STOPPED_EXTERNAL){
        ui->pushButton_stopInv->setStyleSheet(StyleHelper::getStopedButtonStyle());
        currentStatus += ("Остановлен по команде извне (CAN, Modbus) \n");
    }
    else{
        ui->pushButton_stopInv->setStyleSheet(StyleHelper::getStopButtonStyle());
    }
    if (invStatus & INV_STS_WAIT_RECT_STOP) currentStatus += ("Ожидает останова выпрямителя \n");
    if (invStatus & INV_STS_FAULT_STOPPED) currentStatus += ("Остановлен по причине FAULT \n");
    if (invStatus & INV_STS_I_LIMIT) currentStatus += ("Токоограничение \n");
    if (invStatus & INV_STS_ULOW) currentStatus += ("Недостаточно напряжения \n");
    if (invStatus & INV_STS_STOPPED_ALARM){
        ui->pushButton_alarmInv->setStyleSheet(StyleHelper::getStopedButtonStyle());
        currentStatus += ("Аварийный останов \n");
    }
    else{
      ui->pushButton_alarmInv->setStyleSheet(StyleHelper::getStopButtonStyle());
    }
    if (invStatus & INV_STS_UD_LOW_FAULT) currentStatus += ("Остановлен по снижению напряжения на шине \n");
    if (invStatus & INV_STS_TO_STOP_MODE) currentStatus += ("Режим плавной остановки инвертора \n");
    if (invStatus & INV_STS_URECT_SHORTCIRCUIT) currentStatus += ("Остановлен по КЗ от выпрямителя \n");

    ui->textEdit_invertorStatus->setText(currentStatus);
}

//----------- для дальнейшего отображения статуса инвертора!!!!
/* система запущена */
//#define INV_STS_STARTED			(1 << 0)

/* ожидает запуска выпрямителя */
//#define INV_STS_WAIT_RECT_START		(1 << 1)

/* Инвертор остановлен по изменению какого-либо важного регистра */
//#define INV_STS_STOPPED_REGISTERS	(1 << 2)

/* Инвертор остановлен по команде извне (CAN, Modbus) */
//#define INV_STS_STOPPED_EXTERNAL	(1 << 3)

/* ожидает останова выпрямителя */
//#define INV_STS_WAIT_RECT_STOP		(1 << 4)

/* Остановлен по причине FAULT */
//#define INV_STS_FAULT_STOPPED		(1 << 5)

/* Фактическое направление вращения */
//#define INV_STS_RIGHT_DIR			(1 << 6)

/* Токоограничение */
//#define INV_STS_I_LIMIT				(1 << 7)

/* Недостаточно напряжения */
//#define INV_STS_ULOW				(1 << 8)

/* остановлен аварийной кнопкой */
//#define INV_STS_STOPPED_ALARM		(1 << 9)

/* остановлен по снижению напряжения на шине */
//#define INV_STS_UD_LOW_FAULT		(1 << 10)

/* остановлен по снижению напряжения на шине */
//#define INV_STS_UD_HIGH_FAULT		(1 << 11)

/* напряжение не в норме так или эдак */
//#define INV_STS_UD_FAULT \
//	(INV_STS_UD_LOW_FAULT | INV_STS_UD_HIGH_FAULT)

/* режим плавной остановки инвертора */
//#define INV_STS_TO_STOP_MODE		(1 << 12)

/* Активный режим (R, D - "1"; P, N - "0") */
//#define INV_STS_RUN					(1 << 13)

/* остановлен по КЗ зареганному от выпрямителя */
//#define INV_STS_URECT_SHORTCIRCUIT	(1 << 14)
