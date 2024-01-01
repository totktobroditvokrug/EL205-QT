#include "mainwindow.h"
#include "ui_mainwindow.h"

//------------------ Чтение потока из serialport -----------------
void MainWindow::readStream()
{
    QByteArray dataRead;    // принятая куча из COM- порта
    bool checkStandart = ui->checkBox_canStandart->checkState();
    bool checkExtended = ui->checkBox_canExtended->checkState();
    bool checkAnswer = ui->checkBox_adapterAnswer->checkState();

    dataRead.clear();
    while (serial->waitForReadyRead(30))
    {
        if ((serial->bytesAvailable())>8) // если пришло достаточное количество байт, то читаем
        {
          //  qDebug() << "bytesAvailable " << QString::number(serial->bytesAvailable(), 10);
            ui->lineEdit_availableByte->setText(QString::number(serial->bytesAvailable(), 10));
            dataRead = serial->readAll();
            ui->textEdit_dataRead->setText(handleUartParsing(dataRead, checkStandart, checkExtended, checkAnswer, regNumList, regDataArray).join("\n"));
            return;  // обработали валидное количество байт. Выходим из функции запроса
        }
    }
       // qDebug() << "не  вышли по return, неполное сообщение";
}

void MainWindow::on_pushButton_startRead_clicked() // запуск цикличного чтения потока данных
{
    timer->start((ui->lineEdit_freqSampl->text().toInt())); // чтение данных

    timerRegDisplay->start((ui->lineEdit_freqSampl->text().toInt())); // вывод на экран регистров

    ui->pushButton_stopRead->setEnabled(true);
    ui->pushButton_startRead->setEnabled(false);
}

void MainWindow::on_pushButton_readOnce_clicked()
{
    readStream();
}

void MainWindow::on_pushButton_stopRead_clicked()
{
    timer->stop();
    ui->pushButton_stopRead->setEnabled(false);
    ui->pushButton_startRead->setEnabled(true);

}

void MainWindow::on_pushButton_clear_clicked() // очистить поле вывода потока
{
    ui->textEdit_dataRead->clear();
}

void MainWindow::on_lineEdit_volumeTextRead_editingFinished()
{
     // qDebug() << "Число строк:" << ui->lineEdit_volumeTextRead->text().toInt();
      ui->textEdit_dataRead->document()->setMaximumBlockCount(ui->lineEdit_volumeTextRead->text().toInt());
}

void MainWindow::on_lineEdit_freqSampl_editingFinished()
{
    timer->setInterval((ui->lineEdit_freqSampl->text().toInt()));
}
