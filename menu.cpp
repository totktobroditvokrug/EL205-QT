#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionOpen_register_map_triggered()
{
    qDebug() << "открываем файл с картой регистров";
    QFileDialog dialogOpen;
    QString fileName = dialogOpen.getOpenFileName(nullptr, "Выберите файл", "/", "Текстовый файл (*.txt)");
    qDebug() << "Выбранный файл: " << fileName;
    QFile file(fileName);
    if(file.open(QIODevice::ReadWrite | QIODevice::Text)){
        QTextStream stream(&file);
//        s << (RegnumClass::regnumList()).join("\n");
        QString str;

        stream >> str;
        //ui->textEdit_listRegnum->clear();
        //ui->textEdit_listRegnum->setText(str);
    }
    else ui->statusbar->showMessage("Ошибка: error opening output file");
}

void MainWindow::on_actionCreate_register_map_triggered()
{

}
