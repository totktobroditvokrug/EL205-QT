#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "stylehelper.h"

//------------------ Чтение потока из serialport -----------------
void MainWindow::readStream()
{
    QByteArray dataRead;    // принятая куча из COM- порта
    bool checkStandart = ui->checkBox_canStandart->checkState();
    bool checkExtended = ui->checkBox_canExtended->checkState();
    bool checkAnswer = ui->checkBox_adapterAnswer->checkState();

    // qDebug() << "запуск чтения порта";

    dataRead.clear();
    while (serial->waitForReadyRead(30))
    {
        qint64 bytesFromAdapter = serial->bytesAvailable();
        if (bytesFromAdapter > 8) // если пришло достаточное количество байт, то читаем
        {
            ui->lineEdit_availableByte->setText(QString::number(bytesFromAdapter, 10));
            // qDebug() << "прочитано " << QString::number(serial->bytesAvailable(), 10) << " байт";
            dataRead = serial->readAll();
            // принятые данные распишутся по структурам и возвращаютсяв виде форматированных строк ответа
            QStringList parsingDataList = handleUartParsing(dataRead,
                                                            checkStandart,
                                                            checkExtended,
                                                            checkAnswer,
                                                            regNumList,
                                                            sampleNumList,
                                                            regDataArray,
                                                            sampleDataArray,
                                                            &canByIdStandart,
                                                            &canByIdExtended);
            if (parsingDataList.size() > 0){ // если ответ не нулевой, выводим его в текстовое поле регулируемой длины
                if(ui->radioButton_byChekBox->isChecked()) ui->textEdit_dataRead->append(parsingDataList.join("\n"));
            }

            regDisplayTable();
            displayHashID();

            if(bytesFromAdapter > 60) { // переделать под настоящие ответы по CAN
               ui->pushButton_setRegistersFromFile->setEnabled(true);
               emptyBufferCounter = 0;
            }
            return;  // обработали валидное количество байт. Выходим из функции запроса
        }
    }

    ui->pushButton_setRegistersFromFile->setEnabled(false);
    if(emptyBufferCounter < 15) {
        init_setConfigAdapter(); // если не было ничего прочитано, повторно конфигурируем адаптер
        emptyBufferCounter++;
    }
    else {
        if(ui->checkBox_lossConnection->checkState()){
            //  qDebug() << "Вывод предупреждения про частоту кэн";
              QMessageBox::warning(this, "Внимание","Проверьте подключение к CAN шине и скорость передачи данных.");

        }
        emptyBufferCounter = 0;
    }

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

    ui->pushButton_startInv->setEnabled(true);
    ui->pushButton_stopInv->setEnabled(true);
    ui->pushButton_alarmInv->setEnabled(true);

    ui->textEdit_invertorStatus->setEnabled(true);

    ui->lineEdit_currentFreq->setEnabled(true);
    ui->lineEdit_registerValue_1->setEnabled(true);
    ui->lineEdit_registerValue_2->setEnabled(true);
    ui->lineEdit_registerValue_3->setEnabled(true);

   ui->statusbar->showMessage("Запущено чтение CAN");
}

void MainWindow::on_pushButton_readOnce_clicked()
{
    readStream();
}

void MainWindow::on_pushButton_stopRead_clicked()
{
    timer->stop();
    emptyBufferCounter = 0; // запускать таймер проверки связи с нуля
    // разрешить менять настройки CAN
    ui->comboBox_canFreq->setEnabled(true);
    ui->comboBox_readAllCan->setEnabled(true);
    ui->pushButton_setConfigAdapter->setEnabled(true);
    // разрешенные кнопки
    ui->pushButton_stopRead->setEnabled(false);
    ui->pushButton_startRead->setEnabled(true);
    ui->pushButton_disconnect->setEnabled(true);
    ui->pushButton_setRegistersFromFile->setEnabled(false);

    ui->pushButton_startInv->setStyleSheet(StyleHelper::getDisableButtonStyle());
    ui->pushButton_stopInv->setStyleSheet(StyleHelper::getDisableButtonStyle());
    ui->pushButton_alarmInv->setStyleSheet(StyleHelper::getDisableButtonStyle());

    ui->pushButton_startInv->setEnabled(false);
    ui->pushButton_stopInv->setEnabled(false);
    ui->pushButton_alarmInv->setEnabled(false);

    ui->textEdit_invertorStatus->setEnabled(false);

    ui->horizontalSlider->setEnabled(false);

    ui->lineEdit_currentFreq->setEnabled(false);
    ui->lineEdit_registerValue_1->setEnabled(false);
    ui->lineEdit_registerValue_2->setEnabled(false);
    ui->lineEdit_registerValue_3->setEnabled(false);

    ui->statusbar->showMessage("Остановка чтения CAN");
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
