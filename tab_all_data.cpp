#include "mainwindow.h"
#include "ui_mainwindow.h"

//------------------ Чтение потока из serialport -----------------
void MainWindow::readStream()
{
    // QStringList adapterAnswerList; // ошибки адаптера и дежурные ответы
    QByteArray dataRead;    // принятая куча из COM- порта
    bool checkStandart = ui->checkBox_canStandart->checkState();
    bool checkExtended = ui->checkBox_canExtended->checkState();
    bool checkAnswer = ui->checkBox_adapterAnswer->checkState();

    // qDebug() << "запуск чтения порта";

    dataRead.clear();
    while (serial->waitForReadyRead(30))
    {
        if ((serial->bytesAvailable())>8) // если пришло достаточное количество байт, то читаем
        {
            ui->lineEdit_availableByte->setText(QString::number(serial->bytesAvailable(), 10));
            // qDebug() << "прочитано " << QString::number(serial->bytesAvailable(), 10) << " байт";
            dataRead = serial->readAll();
            ui->textEdit_dataRead->setText(handleUartParsing(dataRead,
                                                             checkStandart,
                                                             checkExtended,
                                                             checkAnswer,
                                                             regNumList,
                                                             regDataArray,
                                                             &adapterAnswerList).join("\n"));


            // regDisplay(); // как только прошел парсинг- выдаем данные
            regDisplayTable();
            // qDebug() << adapterAnswerList.join("\n");
            ui->textEdit_adapterAnswer->setText(adapterAnswerList.join("\n"));

            return;  // обработали валидное количество байт. Выходим из функции запроса
        }
    }
    init_setConfigAdapter(); // если не было ничего прочитано, повторно конфигурируем адаптер
       // qDebug() << "не  вышли по return, неполное сообщение";

}

void MainWindow::on_pushButton_startRead_clicked() // запуск цикличного чтения потока данных
{
    // запретить менять настройки CAN
    ui->comboBox_canFreq->setEnabled(false);
    ui->comboBox_readAllCan->setEnabled(false);
    ui->pushButton_setConfigAdapter->setEnabled(false);
    // разрешенные кнопки
    ui->pushButton_stopRead->setEnabled(true);
    ui->pushButton_startRead->setEnabled(false);
    ui->pushButton_disconnect->setEnabled(false);

    timer->start((ui->lineEdit_freqSampl->text().toInt())); // чтение данных

   // timerRegDisplay->start((ui->lineEdit_freqSampl->text().toInt())); // вывод на экран регистров ВРЕМЕННО!!!


}

void MainWindow::on_pushButton_readOnce_clicked()
{
    readStream();
}

void MainWindow::on_pushButton_stopRead_clicked()
{
    timer->stop();
    // разрешить менять настройки CAN
    ui->comboBox_canFreq->setEnabled(true);
    ui->comboBox_readAllCan->setEnabled(true);
    ui->pushButton_setConfigAdapter->setEnabled(true);
    // разрешенные кнопки
    ui->pushButton_stopRead->setEnabled(false);
    ui->pushButton_startRead->setEnabled(true);
    ui->pushButton_disconnect->setEnabled(true);

}

void MainWindow::on_pushButton_clear_clicked() // очистить поле вывода потока
{
    ui->textEdit_dataRead->clear();
}

//------- изменить максимальный размер блока для вывода выбранных посылок
void MainWindow::on_lineEdit_volumeTextRead_editingFinished()
{
      ui->textEdit_dataRead->document()->setMaximumBlockCount(ui->lineEdit_volumeTextRead->text().toInt());
}

//------- изменить частоту опроса CAN-адаптера
void MainWindow::on_lineEdit_freqSampl_editingFinished()
{
    timer->setInterval((ui->lineEdit_freqSampl->text().toInt()));
}
