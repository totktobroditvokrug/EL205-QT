#include "mainwindow.h"
#include "ui_mainwindow.h"


// Выбор создания псевдонимов regisres/samles

void MainWindow::on_radioButton_registers_clicked(bool checked) // выбор регистров
{
    if(checked){
        ui->label_selectAlias->setStyleSheet(StyleHelper::getRegistersStyle());
        ui->radioButton_samples->setStyleSheet(StyleHelper::getRadioButtonResetTextStyle());
        ui->radioButton_registers->setStyleSheet(StyleHelper::getRadioButtonRegistersStyle());
        ui->label_selectAlias->setText("Selected registers");
        ui->listWidget_sampleNum->hide();
        ui->listWidget_regNum->show();
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
void MainWindow::on_pushButton_genRegFromEnum_clicked()
{
    QVector<QString> numList; // формирование списка
    QString message;
    if(ui->radioButton_registers->isChecked()){
      numList  = RegnumClass::regnumArray();
      ui->listWidget_regNum->clear();
      message = "Генерация списка всех регистров ПЧ";
    }
    else{
        numList  = FcCanIdClass::fccanidArray();
        ui->listWidget_sampleNum->clear();
        message = "Генерация списка измерений ПЧ";
    }

    int sizeArray = numList.size();
    for (int i = 0; i < sizeArray; i++) {
       QListWidgetItem *item = new QListWidgetItem;
       item->setText(QString::number(i, 10) + ": " + numList.at(i));
       item->setCheckState(Qt::Unchecked);
       if(ui->radioButton_registers->isChecked()) ui->listWidget_regNum->addItem(item);
       else ui->listWidget_sampleNum->addItem(item);
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

void MainWindow::on_pushButton_readRegFromFile_clicked()
{
  //  qDebug() << "открываем файл с картой регистров";
    QString message = "Прочитаны регистры из файла ";
    QFileDialog dialogOpen;
    QString fileName = dialogOpen.getOpenFileName(nullptr, "Выберите файл", workDirPath, "Текстовый файл (*.txt)");
//    qDebug() << "Выбранный файл: " << fileName;
    QFile file(fileName);
    if(file.open(QIODevice::ReadWrite | QIODevice::Text)){
        QTextStream stream(&file);
        QString readStr = stream.readAll();

        // заполняем данными из файла виджет с checkButton
        ui->listWidget_regNum->clear();
        QStringList stringArray = readStr.split("\n");
        int length = stringArray.size();
        for (int i = 0; i < length; i++) {
           QListWidgetItem *item = new QListWidgetItem;

           QStringList separateNum = stringArray.at(i).split(":", QString::SkipEmptyParts); // разделяем номер и имя
           if (separateNum.size() != 2){
              message = "Неверный формат списка регистров ";
           }
           else {
               quint8 index = quint8(separateNum[0].toInt()); // номер до :
               QString regName = separateNum[1].simplified();              // строка после :

               regNumList[index] = regName; // инициализируем имена регистров

               item->setText(stringArray.at(i));
               item->setCheckState(Qt::Unchecked);
               ui->listWidget_regNum->addItem(item);
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

    // чудовищная по скоростиисполнения реализация!!!!!! переделать
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


void MainWindow::on_pushButton_selectAll_clicked()
{

    int countRegnum = ui->listWidget_regNum->count();
  //  qDebug() << "Select All. Count=" << QString::number(countRegnum,10);
    for(int i = 0; i < countRegnum; i++){
        QListWidgetItem *item = ui->listWidget_regNum->item(i);
        QStringList separateNum = item->text().split(":", QString::SkipEmptyParts); // разделяем номер и имя
        quint8 index = quint8(separateNum[0].toInt()); // номер до :
        QString regName = separateNum[1].simplified(); // строка после :
        ui->listWidget_regNum->item(i)->setCheckState(Qt::Checked);
        addItemFromlistwidget(item, index, regName);
    }
}

void MainWindow::on_pushButton_resetAll_clicked()
{
    int countRegnum = ui->listWidget_regNum->count();
  //  qDebug() << "Reset All. Count=" << QString::number(countRegnum,10);
    for(int i = 0; i < countRegnum; i++){
        QListWidgetItem *item = ui->listWidget_regNum->item(i);
        QStringList separateNum = item->text().split(":", QString::SkipEmptyParts); // разделяем номер и имя
        quint8 index = quint8(separateNum[0].toInt()); // номер до :
        QString regName = separateNum[1].simplified(); // строка после :
        item->setCheckState(Qt::Unchecked);
        deleteItemFromlistwidget(item, index);
    }
}
