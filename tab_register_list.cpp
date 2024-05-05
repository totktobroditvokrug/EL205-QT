#include "mainwindow.h"
#include "ui_mainwindow.h"


// Выбор создания псевдонимов regisres/samles

void MainWindow::on_radioButton_registers_clicked(bool checked)
{
    if(checked){
        ui->label_selectAlias->setStyleSheet(StyleHelper::getRegistersStyle());
        ui->radioButton_samples->setStyleSheet(StyleHelper::getRadioButtonResetTextStyle());
        ui->radioButton_registers->setStyleSheet(StyleHelper::getRadioButtonRegistersStyle());
        ui->label_selectAlias->setText("Selected registers");
    }
}

void MainWindow::on_radioButton_samples_clicked(bool checked)
{
    if(checked){
        ui->label_selectAlias->setStyleSheet(StyleHelper::getSamplesStyle());
        ui->radioButton_registers->setStyleSheet(StyleHelper::getRadioButtonResetTextStyle());
        ui->radioButton_samples->setStyleSheet(StyleHelper::getRadioButtonSamplesStyle());
        ui->label_selectAlias->setText("Selected samples");
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
    ui->listWidget_regNum->clear();
    //regNumList = RegnumClass::regnumArray();
    regNumList = FcCanIdClass::fccanidArray();

    for (int i = 0; i < IREG_INV_ALL_END_REGISTERS; i++) {
       QListWidgetItem *item = new QListWidgetItem;
       item->setText(QString::number(i, 10) + ": " + regNumList.at(i));
       item->setCheckState(Qt::Unchecked);
       ui->listWidget_regNum->addItem(item);
    }
    ui->statusbar->showMessage("Генерация списка всех регистров ПЧ");
}

void MainWindow::on_pushButton_saveRegToFile_clicked()
{
    QFileDialog dialogSave;
    QString pathSave = dialogSave.getSaveFileName(nullptr, "Выберите файл", workDirPath, "Текстовый файл (*.txt)");
 //   qDebug() << "записываем файл с картой регистров: " << pathSave;

    QFile file(pathSave);
     // Открываем файл, создаем, если его не существует
    if((file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))){
        QTextStream stream(&file);
        stream << ui->textEdit_selectedRegNum->toPlainText();
        file.close();
        ui->statusbar->showMessage("Карта регистров записана в файл " + pathSave);
    }
    else ui->statusbar->showMessage("Ошибка записи файла " + pathSave);
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
