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
            << "data";
    // добавить архив для data

    ui->tableRegister->setColumnCount(6); // Указываем число колонок
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
    int currentRegNum; // номер регистра в таблице
    for(int i = 0; i <= ui->tableRegister->rowCount(); i++){
        QTableWidgetItem *item = ui->tableRegister->item(i, 0); // проверка столбца с номерами регистров
        if (!!item) { // если ячейка не NULL
           currentRegNum = item->text().toInt();
           if(currentRegNum == index) ui->tableRegister->removeRow(i);
        }
    }
}
