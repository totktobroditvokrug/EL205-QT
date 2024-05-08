#include "mainwindow.h"
#include "ui_mainwindow.h"


// Выбор создания псевдонимов regisres/samples
void MainWindow::on_radioButton_registers_clicked(bool checked) // выбор регистров
{
    if(checked){
        ui->label_selectAlias->setStyleSheet(StyleHelper::getRegistersStyle());
        ui->radioButton_samples->setStyleSheet(StyleHelper::getRadioButtonResetTextStyle());
        ui->radioButton_registers->setStyleSheet(StyleHelper::getRadioButtonRegistersStyle());
        ui->label_selectAlias->setText("Selected registers");
        ui->listWidget_sampleNum->hide();
        ui->listWidget_regNum->show();
        ui->textEdit_selectedSampleNum->hide();
        ui->textEdit_selectedRegNum->show();
    }
}


void MainWindow::on_radioButton_samples_clicked(bool checked)  // выбор измерений
{
    if(checked){
        ui->label_selectAlias->setStyleSheet(StyleHelper::getSamplesStyle());
        ui->radioButton_registers->setStyleSheet(StyleHelper::getRadioButtonResetTextStyle());
        ui->radioButton_samples->setStyleSheet(StyleHelper::getRadioButtonSamplesStyle());
        ui->label_selectAlias->setText("Selected samples");
        ui->listWidget_regNum->hide();
        ui->listWidget_sampleNum->show();
        ui->textEdit_selectedSampleNum->show();
        ui->textEdit_selectedRegNum->hide();
    }
}


// выбор домашней директории с файлами инвертора
void MainWindow::on_pushButton_workDir_clicked()
{
    QString currentWorkDir = ui->lineEdit_workDir->text();
    workDirPath = QFileDialog::getExistingDirectory(nullptr, "Directory Dialog", currentWorkDir);
    if(!workDirPath.isEmpty()) ui->lineEdit_workDir->setText(workDirPath);
    else workDirPath = currentWorkDir;
}


// генерация списка регистров из перечисления в проекте инвертора

void MainWindow::on_pushButton_genFromEnum_clicked()
{
    if(ui->radioButton_registers->isChecked()) genRegFromEnum();
    else genSampleFromEnum();
}

void MainWindow::genRegFromEnum()
{
    QString message = "Генерация списка всех регистров ПЧ";
    regNumList = RegnumClass::regnumArray();
    ui->listWidget_regNum->clear();
    int sizeArray = regNumList.size();
    for (int i = 0; i < sizeArray; i++) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(QString::number(i, 10) + ": " + regNumList.at(i));
        item->setCheckState(Qt::Unchecked);
        ui->listWidget_regNum->addItem(item);
    }
    ui->statusbar->showMessage(message);
}

void MainWindow::genSampleFromEnum()
{
    QString message = "Генерация списка измерений ПЧ";
    sampleNumList  = SampleCanIdClass::fccanidArray();
    ui->listWidget_sampleNum->clear();
    int sizeArray = sampleNumList.size();
    for (int i = SampleCanIdClass::CAN_START_SAMPLE_ID; i < sizeArray; i++) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(QString::number(i, 10) + ": " + sampleNumList.at(i));
        item->setCheckState(Qt::Unchecked);
        ui->listWidget_sampleNum->addItem(item);
    }
    ui->statusbar->showMessage(message);
}


void MainWindow::on_pushButton_saveRegToFile_clicked()
{
    QFileDialog dialogSave;
    QString pathSave = dialogSave.getSaveFileName(nullptr, "Выберите файл", workDirPath, "Текстовый файл (*.txt)");
    QString message;

    QFile file(pathSave);
     // Открываем файл, создаем, если его не существует
    if((file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))){
        QTextStream stream(&file);
        if(ui->radioButton_registers->isChecked()){
            stream << ui->textEdit_selectedRegNum->toPlainText();
            message = "Карта регистров записана в файл ";
        }
        else{
           stream << ui->textEdit_selectedSampleNum->toPlainText();
           message = "Карта измерений записана в файл ";
        }
        file.close();
    }
    else message = "Ошибка записи файла ";

    ui->statusbar->showMessage(message + pathSave);
}


// прочитать из файла список измерений или регистров
void MainWindow::on_pushButton_readRegFromFile_clicked()
{
    QString message;
    QFileDialog dialogOpen;
    QString fileName = dialogOpen.getOpenFileName(nullptr, "Выберите файл", workDirPath, "Текстовый файл (*.txt)");
    QFile file(fileName);
    if(file.open(QIODevice::ReadWrite | QIODevice::Text)){
        QTextStream stream(&file);
        QString readStr = stream.readAll();
        QStringList stringArray = readStr.split("\n");
        int length = stringArray.size();

        if(ui->radioButton_registers->isChecked()) ui->listWidget_regNum->clear();
        else ui->listWidget_sampleNum->clear();

        for (int i = 0; i < length; i++) {
           QListWidgetItem *item = new QListWidgetItem;

           QStringList separateNum = stringArray.at(i).split(":", QString::SkipEmptyParts); // разделяем номер и имя
           if (separateNum.size() != 2){
              message = "Неверный формат списка ";
           }
           else {
               quint8 index = quint8(separateNum[0].toInt()); // номер до :
               QString name = separateNum[1].simplified();              // строка после :
               item->setText(stringArray.at(i));
               item->setCheckState(Qt::Unchecked);

               if(ui->radioButton_registers->isChecked()){ // заполняем данными из файла виджет с checkButton
                   // инициализируем имена регистров !!! Непонятно, зачем. todo (если только пользователь хочет свои имена...)
                   regNumList[index] = name;
                   ui->listWidget_regNum->addItem(item);
                   message = "Прочитаны регистры из файла ";
               }
                else{       // заполняем данными из файла виджет с checkButton
                   sampleNumList[index] = name;
                   ui->listWidget_sampleNum->addItem(item);
                   message = "Прочитан список измерений из файла ";
               }
           }
        }
        file.close();
    }
    else message = "Ошибка чтения файла ";

    ui->statusbar->showMessage(message + fileName);
}


//-------- добавить выбранный регистр в таблицу и виджет просмотра
void MainWindow::addItemFromlistwidget(QListWidgetItem *item, quint8 index, QString regName){

    item->setForeground(Qt::red);
    regDataArray[index].displayed = true;
    ui->textEdit_selectedRegNum->append(item->text());
    addRowRegistersTable(index, regName); // добавляем выбранный регистр в таблицу
}

//-------- удалить выбранный регистр из таблицы и виджета просмотра
void MainWindow::deleteItemFromlistwidget(QListWidgetItem *item, quint8 index){
    item->setForeground(Qt::black);
    regDataArray[index].displayed = false;
    deleteRowRegistersTable(index);
    ui->textEdit_selectedRegNum->clear();
    int countRegnum = ui->listWidget_regNum->count();
    for(int i = 0; i < countRegnum; i++){
        QListWidgetItem *itemRegNum = ui->listWidget_regNum->item(i);
        if(itemRegNum->checkState() == Qt::Checked) ui->textEdit_selectedRegNum->append(itemRegNum->text());
    }
}

//------- ручной выбор регистров для вывода на экран
void MainWindow::on_listWidget_regNum_itemClicked(QListWidgetItem *item)
{
    QStringList separateNum = item->text().split(":", QString::SkipEmptyParts); // разделяем номер и имя
    quint8 index = quint8(separateNum[0].toInt()); // номер до :
    QString regName = separateNum[1].simplified();              // строка после :

    if(item->checkState() == Qt::Checked) {
        addItemFromlistwidget(item, index, regName);
    }
    else {
        deleteItemFromlistwidget(item, index);
    }
}


//------- ручной выбор списка измерений для вывода на экран
void MainWindow::on_listWidget_sampleNum_itemClicked(QListWidgetItem *item)
{
    QStringList separateNum = item->text().split(":", QString::SkipEmptyParts); // разделяем номер и имя
    int index = separateNum[0].toInt(); // номер до :
    QString sampleName = separateNum[1].simplified();              // строка после :

    if(item->checkState() == Qt::Checked) {
        addSampleFromlistwidget(item, index, sampleName);
    }
    else {
        deleteSampleFromlistwidget(item, index);
    }
}

//-------- добавить выбранный элемент измерений  в таблицу и виджет просмотра
void MainWindow::addSampleFromlistwidget(QListWidgetItem *item, int index, QString sampleName){
    item->setForeground(Qt::red);
    sampleDataArray[index].displayed = true;
    ui->textEdit_selectedSampleNum->append(item->text());
    addRowSamplesTable(index, sampleName); // добавляем выбранный регистр в таблицу
}

//-------- удалить выбранный элемент измерений  из таблицы и виджета просмотра
void MainWindow::deleteSampleFromlistwidget(QListWidgetItem *item, int index){
    item->setForeground(Qt::black);
    sampleDataArray[index].displayed = false;
    deleteRowSamplesTable(index);

    // чудовищная по скоростиисполнения реализация!!!!!! переделать todo
    ui->textEdit_selectedSampleNum->clear();
    int countRegnum = ui->listWidget_sampleNum->count();
    for(int i = 0; i < countRegnum; i++){
        QListWidgetItem *itemSampleNum = ui->listWidget_sampleNum->item(i);
        if(itemSampleNum->checkState() == Qt::Checked) ui->textEdit_selectedSampleNum->append(itemSampleNum->text());
    }
}


void MainWindow::on_pushButton_selectAll_clicked()
{
    int countNum;
    if(ui->radioButton_registers->isChecked()) countNum = ui->listWidget_regNum->count();
    else countNum = ui->listWidget_sampleNum->count();

    for(int i = 0; i < countNum; i++){
        QListWidgetItem *currentItem = nullptr;

        if(ui->radioButton_registers->isChecked()) currentItem = ui->listWidget_regNum->item(i);
        else currentItem = ui->listWidget_sampleNum->item(i);

        QStringList separateNum = currentItem->text().split(":", QString::SkipEmptyParts); // разделяем номер и имя
        int index = separateNum[0].toInt(); // номер до :
        QString name = separateNum[1].simplified(); // строка после :

        if(ui->radioButton_registers->isChecked()){
            ui->listWidget_regNum->item(i)->setCheckState(Qt::Checked);
            addItemFromlistwidget(currentItem, quint8(index), name);
        }
        else{
          ui->listWidget_sampleNum->item(i)->setCheckState(Qt::Checked);
          addSampleFromlistwidget(currentItem, index, name);
        }
    }
}

void MainWindow::on_pushButton_resetAll_clicked()
{
    int countNum;
    if(ui->radioButton_registers->isChecked()) countNum = ui->listWidget_regNum->count();
    else countNum = ui->listWidget_sampleNum->count();
    for(int i = 0; i < countNum; i++){
        QListWidgetItem *item = nullptr;
        if(ui->radioButton_registers->isChecked()) item = ui->listWidget_regNum->item(i);
        else item = ui->listWidget_sampleNum->item(i);

        QStringList separateNum = item->text().split(":", QString::SkipEmptyParts); // разделяем номер и имя
        quint8 index = quint8(separateNum[0].toInt()); // номер до :
        item->setCheckState(Qt::Unchecked);
        item->setForeground(Qt::black);
        if(ui->radioButton_registers->isChecked()) regDataArray[index].displayed = false;
        else sampleDataArray[index].displayed = false;
    }
    if(ui->radioButton_registers->isChecked()){
        createRegistersTable();
        ui->textEdit_selectedRegNum->clear();
    }
    else{
        createSamplesTable();
        ui->textEdit_selectedSampleNum->clear();
    }
}
