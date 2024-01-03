#include "mainwindow.h"
#include "ui_mainwindow.h"


// генерация списка регистров из перечисления в проекте инвертора
void MainWindow::on_pushButton_genRegFromEnum_clicked()
{
    ui->listWidget_regNum->clear();
    regNumList = RegnumClass::regnumArray();

    for (int i = 0; i < IREG_INV_ALL_END_REGISTERS; i++) {
        // regnumList[i] = "test: " + QString::number(i, 10);
       QListWidgetItem *item = new QListWidgetItem;
      // item->setData(i);
       item->setText(QString::number(i, 10) + ": " + regNumList.at(i));
       item->setCheckState(Qt::Unchecked);
       ui->listWidget_regNum->addItem(item);
     //   qDebug() << regnumList.at(i);
    }
}

void MainWindow::on_pushButton_saveRegToFile_clicked()
{
    QFileDialog dialogSave;
    QString pathSave = dialogSave.getSaveFileName();
    qDebug() << "записываем файл с картой регистров: " << pathSave;

    QFile file(pathSave);
     // Открываем файл, создаем, если его не существует
    if((file.open(QIODevice::ReadWrite | QIODevice::Text))){
        QTextStream stream(&file);
        stream << ui->textEdit_selectedRegNum->toPlainText();
        file.close();
    }
    else ui->statusbar->showMessage("Ошибка: error opening output file");
}

void MainWindow::on_pushButton_readRegFromFile_clicked()
{
    qDebug() << "открываем файл с картой регистров";
    QFileDialog dialogOpen;
    QString fileName = dialogOpen.getOpenFileName(nullptr, "Выберите файл", "/", "Текстовый файл (*.txt)");
    qDebug() << "Выбранный файл: " << fileName;
    QFile file(fileName);
    if(file.open(QIODevice::ReadWrite | QIODevice::Text)){
        QTextStream stream(&file);
//        s << (RegnumClass::regnumList()).join("\n");
        QString readStr = stream.readAll();

        ui->textEdit_selectedRegNum->clear();
        ui->textEdit_selectedRegNum->setText(readStr);

        // заполняем данными из файла виджет с checkButton
        ui->listWidget_regNum->clear();
        QStringList stringArray = readStr.split("\n");
        int length = stringArray.size();
        for (int i = 0; i < length; i++) {
           QListWidgetItem *item = new QListWidgetItem;
           item->setText(stringArray.at(i));
           item->setCheckState(Qt::Unchecked);
           ui->listWidget_regNum->addItem(item);
           file.close();
        }
    }
    else ui->statusbar->showMessage("Ошибка: error opening output file");
}

//------- выбор регистров для вывода на экран
void MainWindow::on_listWidget_regNum_itemClicked(QListWidgetItem *item)
{
    QStringList separateNum = item->text().split(":", QString::SkipEmptyParts);
    quint8 index = quint8(separateNum[0].toInt()); // номер до :
    QString regName = separateNum[1].simplified();              // строка после :
    // qDebug() << "separateNum: " << separateNum[0] << "int= " << QString::number(index, 10);
    if(item->checkState() == Qt::Checked){
        item->setForeground(Qt::red);
        regDataArray[index].displayed = true;
        ui->textEdit_selectedRegNum->append(item->text());

       // qDebug() << "table test: " << item->text();
        addRowRegistersTable(index, regName);
    }
    else{
       item->setForeground(Qt::black);
       regDataArray[index].displayed = false;
       deleteRowRegistersTable(index);
    }
}

void MainWindow::on_pushButton_loadToDisplay_clicked()
{


}
