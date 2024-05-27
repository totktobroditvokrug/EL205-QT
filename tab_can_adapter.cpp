#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stylehelper.h"

void MainWindow::on_pushButton_searchListPort_clicked() // список доступных портов
{
    QString description;
    QString manufacturer;
    QString serialNumber;
    // Получить доступные последовательные порты
    QList<QSerialPortInfo> serialPortInfos = QSerialPortInfo::availablePorts();

         // Выводим количество последовательных портов, которые может использовать текущая система
    // qDebug() << "Total numbers of ports: " << serialPortInfos.count();

    ui->listWidget_portInfo->clear();
    ui->comboBox_serialPort->clear();
    // Добавить все доступные последовательные устройства вComboBoxв
    for (const QSerialPortInfo &serialPortInfo : serialPortInfos)
    {
       QStringList list;

       description = serialPortInfo.description();
       manufacturer = serialPortInfo.manufacturer();
       serialNumber = serialPortInfo.serialNumber();

       list << serialPortInfo.portName()
            << (!description.isEmpty() ? description : "None")
            << (!manufacturer.isEmpty() ? manufacturer : "None")
            << (!serialNumber.isEmpty() ? serialNumber : "None")
            << serialPortInfo.systemLocation()
            << (serialPortInfo.vendorIdentifier() ? QString::number(serialPortInfo.vendorIdentifier(), 16) : "None")
            << (serialPortInfo.productIdentifier() ? QString::number(serialPortInfo.productIdentifier(), 16) : "None");

       ui->comboBox_serialPort->addItem(list[0]);
       ui->listWidget_portInfo->addItem(list[0] + ": " + list[1]+ ": " + list[2]+ ": " + list[3]);
//       qDebug() << "Description: " << list;
    }
}

void MainWindow::initTabCan(){
//    qDebug() << "инициализация комбо-боксов и кнопок;";
    // Сбросить биты контроля состояния порта
    ui->label_nameOk->setText("-");
    ui->label_baudOk->setText("-");
    ui->label_dataOk->setText("-");
    ui->label_flowOk->setText("-");
    ui->label_parityOk->setText("-");
    ui->label_stopOk->setText("-");
    ui->label_flowOk->setText("-");


    // Устанавливаем скорость передачи
    ui->comboBox_baudRate->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->comboBox_baudRate->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->comboBox_baudRate->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->comboBox_baudRate->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->comboBox_baudRate->addItem(tr("Custom"));
    ui->comboBox_baudRate->setCurrentIndex(3);

        // Устанавливаем биты данных
    ui->comboBox_dataBits->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->comboBox_dataBits->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->comboBox_dataBits->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->comboBox_dataBits->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->comboBox_dataBits->setCurrentIndex(3);

        // Установить бит четности
    ui->comboBox_parity->addItem("None", 0);
    ui->comboBox_parity->addItem("Even", 1);
    ui->comboBox_parity->addItem("Odd", 2);
    ui->comboBox_parity->addItem("Mark", 3);
    ui->comboBox_parity->addItem("Space", 4);

        // Установить стоп-бит
    ui->comboBox_stopBit->addItem(QStringLiteral("1"), QSerialPort::OneStop);
    ui->comboBox_stopBit->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

        // Добавить управление потоком
    ui->comboBox_flowBit->addItem(tr("None"), 0);
    ui->comboBox_flowBit->addItem(tr("RTS/CTS"), 1);
    ui->comboBox_flowBit->addItem(tr("XON/XOFF"), 2);

    ui->comboBox_canFreq->addItem(tr("125 kb/s"), 0);
    ui->comboBox_canFreq->addItem(tr("250 kb/s"), 1);
    ui->comboBox_canFreq->addItem(tr("500 kb/s"), 2);
    ui->comboBox_canFreq->setCurrentIndex(1);

    ui->comboBox_readAllCan->addItem(tr("All"), 0);
    ui->comboBox_readAllCan->addItem(tr("Mask"), 1);

    ui->pushButton_disconnect->setEnabled(false);
//    ui->pushButton_sendMessage->setEnabled(false);

    ui->pushButton_readOnce->setEnabled(false);
    ui->pushButton_stopRead->setEnabled(false);
    ui->pushButton_startRead->setEnabled(false);

    ui->comboBox_canFreq->setEnabled(false);
    ui->comboBox_readAllCan->setEnabled(false);

    ui->pushButton_setRegistersFromFile->setEnabled(false);

    ui->pushButton_startInv->setStyleSheet(StyleHelper::getDisableButtonStyle());
    ui->pushButton_stopInv->setStyleSheet(StyleHelper::getDisableButtonStyle());
    ui->pushButton_alarmInv->setStyleSheet(StyleHelper::getDisableButtonStyle());
    ui->pushButton_startInv_panel->setStyleSheet(StyleHelper::getDisableButtonStyle());
    ui->pushButton_stopInv_panel->setStyleSheet(StyleHelper::getDisableButtonStyle());
    ui->pushButton_alarmInv_panel->setStyleSheet(StyleHelper::getDisableButtonStyle());

    ui->textEdit_invertorStatus->setEnabled(false);

    // отключить активность кнопок управления инвертором
    ui->pushButton_startInv->setEnabled(false);
    ui->pushButton_stopInv->setEnabled(false);
    ui->pushButton_alarmInv->setEnabled(false);
    ui->pushButton_startInv_panel->setEnabled(false);
    ui->pushButton_stopInv_panel->setEnabled(false);
    ui->pushButton_alarmInv_panel->setEnabled(false);

    // отключит активность полей вывода регистров. По ним вторично активизируется слайдер
    ui->lineEdit_currentFreq->setEnabled(false);
    ui->lineEdit_registerValue_1->setEnabled(false);
    ui->lineEdit_registerValue_2->setEnabled(false);
    ui->lineEdit_registerValue_3->setEnabled(false);

    ui->radioButton_fromHashTable->setChecked(true);
}


void MainWindow::on_pushButton_connect_clicked()
{
//    qDebug() << "попытка соединиться с портом. Предварительный сброс;";

    serial->close(); // если порт был открыт, закрываем его

    ui->pushButton_connect->setEnabled(false);

    ui->comboBox_baudRate->setEnabled(false);
    ui->comboBox_dataBits->setEnabled(false);
    ui->comboBox_flowBit->setEnabled(false);
    ui->comboBox_parity->setEnabled(false);
    ui->comboBox_serialPort->setEnabled(false);

    ui->pushButton_setConfigAdapter->setEnabled(false);
    ui->comboBox_stopBit->setEnabled(false);

        // Установить имя последовательного порта
    serial->setPortName(ui->comboBox_serialPort->currentText());
    ui->label_nameOk->setText("ok");

        // Устанавливаем скорость передачи
    if (serial->setBaudRate(ui->comboBox_baudRate->currentText().toInt())){
        ui->label_baudOk->setText("ok");
      //  qDebug() << "setBaudRate" << ui->comboBox_baudRate->currentText().toInt();
    }
    else ui->label_baudOk->setText("err");


        // Устанавливаем биты данных
    switch (ui->comboBox_dataBits->currentText().toUInt()) {
    case 5: if (serial->setDataBits(QSerialPort::Data5)) {
            ui->label_dataOk->setText("ok");
        }
        else ui->label_dataOk->setText("err");
        break;
    case 6: if (serial->setDataBits(QSerialPort::Data6)){
            ui->label_dataOk->setText("err");
        }
        else ui->label_dataOk->setText("ok");
        break;
    case 7: if (serial->setDataBits(QSerialPort::Data7)){
            ui->label_dataOk->setText("ok");
        }
        else ui->label_dataOk->setText("err");
        break;
    case 8: if (serial->setDataBits(QSerialPort::Data8)){
            ui->label_dataOk->setText("ok");
        }
        else ui->label_dataOk->setText("err");
        break;
    default: ui->label_dataOk->setText("err"); break;
    }

        // Установить бит четности
    switch(ui->comboBox_parity->currentIndex()) {
    case 0:
        if (serial->setParity(QSerialPort::NoParity)){
            ui->label_parityOk->setText("ok");
        }
        else ui->label_parityOk->setText("err");
        break;
    case 1:
        if (serial->setParity(QSerialPort::EvenParity)){
            ui->label_parityOk->setText("ok");
        }
        else ui->label_parityOk->setText("err");
        break;
    case 2:
        if (serial->setParity(QSerialPort::OddParity)){
            ui->label_parityOk->setText("ok");
        }
        else ui->label_parityOk->setText("err");
        break;
    case 3:
        if (serial->setParity(QSerialPort::MarkParity)){
            ui->label_parityOk->setText("ok");
        }
        else ui->label_parityOk->setText("err");
        break;
    case 4:
        if (serial->setParity(QSerialPort::SpaceParity)){
            ui->label_parityOk->setText("ok");
        }
        else ui->label_parityOk->setText("err");
        break;
    default: ui->label_parityOk->setText("err"); break;
    }

        // Установить стоп-бит
    switch (ui->comboBox_stopBit->currentText().toUInt()) {
    case 1: if (serial->setStopBits(QSerialPort::OneStop)){
            ui->label_stopOk->setText("ok");
        }
        else ui->label_stopOk->setText("err");
        break;
    case 2:  if (serial->setStopBits(QSerialPort::TwoStop)){
            ui->label_stopOk->setText("ok");
        }
        else ui->label_stopOk->setText("err");
        break;
    default: ui->label_stopOk->setText("err"); break;
    }

    // Установить управление потоком
    switch (ui->comboBox_flowBit->currentIndex()) {
    case 0:
        if (serial->setFlowControl(QSerialPort::NoFlowControl)){
            ui->label_flowOk->setText("ok");
        }
        else ui->label_flowOk->setText("err");
        break;
    case 1:
        if (serial->setFlowControl(QSerialPort::HardwareControl)){
            ui->label_flowOk->setText("ok");
        }
        else ui->label_flowOk->setText("err");
        break;
    case 2:
        if (serial->setFlowControl(QSerialPort::SoftwareControl)){
            ui->label_flowOk->setText("ok");
        }
        else ui->label_flowOk->setText("err");
        break;
    default: ui->label_flowOk->setText("err"); break;
    }

    // пробуем подключится
    if (!serial->open(QIODevice::ReadWrite)) {
        // если подключится не получится, то покажем сообщение с ошибкой
        qDebug()<<"Port opening error"<<serial->errorString();
        ui->statusbar->showMessage("Ошибка: " + serial->errorString());
        ui->pushButton_disconnect->setEnabled(true);
        return;
    }
//    qDebug()<<"Serial port is OPEN: ";
    ui->statusbar->showMessage("Serial port is OPEN");

    ui->pushButton_disconnect->setEnabled(true);
//    ui->pushButton_sendMessage->setEnabled(true);
    ui->pushButton_readOnce->setEnabled(true);
    ui->pushButton_stopRead->setEnabled(true);
    ui->pushButton_startRead->setEnabled(true);

    ui->comboBox_canFreq->setEnabled(true);
    ui->comboBox_readAllCan->setEnabled(true);
    ui->pushButton_setConfigAdapter->setEnabled(true);

    init_setConfigAdapter();  // инициализация адаптера по частоте CAN и фильтру сообщений
}


void MainWindow::on_pushButton_disconnect_clicked()
{
    ui->pushButton_disconnect->setEnabled(false);
    serial->close();
    qDebug()<<"Serial port is CLOSE: ";
    ui->statusbar->showMessage("Serial port is CLOSE");
//    ui->txtOutput->clear();

    ui->comboBox_baudRate->setEnabled(true);
    ui->comboBox_dataBits->setEnabled(true);
    ui->comboBox_flowBit->setEnabled(true);
    ui->comboBox_parity->setEnabled(true);
    ui->comboBox_serialPort->setEnabled(true);
    ui->comboBox_stopBit->setEnabled(true);

    ui->label_flowOk->setText("-");
    ui->label_stopOk->setText("-");
    ui->label_parityOk->setText("-");
    ui->label_dataOk->setText("-");
    ui->label_baudOk->setText("-");

//   ui->pushButton_sendMessage->setEnabled(false);
    ui->pushButton_readOnce->setEnabled(false);
    ui->pushButton_stopRead->setEnabled(false);
    ui->pushButton_startRead->setEnabled(false);
    ui->pushButton_connect->setEnabled(true);
    ui->pushButton_setRegistersFromFile->setEnabled(false);

    ui->pushButton_setConfigAdapter->setEnabled(false);
    ui->comboBox_canFreq->setEnabled(false);
    ui->comboBox_readAllCan->setEnabled(false);

    ui->label_nameOk->setText("-");
    ui->label_baudOk->setText("-");
    ui->label_dataOk->setText("-");
    ui->label_flowOk->setText("-");
    ui->label_parityOk->setText("-");
    ui->label_stopOk->setText("-");
    ui->label_flowOk->setText("-");
}


//------------------ Запись в serialport ------------------
void MainWindow::writeSerialPort(QString dataWriteString)
{
    QByteArray dataWriteIn = QByteArray::fromHex(dataWriteString.toUtf8()); // данные из строки textEdit_sendMessage
  //  qDebug() << "функция записи: " << dataWriteString;
    serial->write(dataWriteIn);
    serial->waitForBytesWritten();
}

//------------------ Чтение из serialport -----------------
QByteArray MainWindow::readSerialPort()
{
 //   qDebug() << "запуск чтения порта";
    QByteArray dataRead;
    while (serial->waitForReadyRead(30)) {
        dataRead = serial->readAll();
    }

    QByteArray text = dataRead.toHex();
 //   qDebug() << "приняли";
    return text;
}

//-------------- настройка режима работы адаптера ----------
void MainWindow::on_comboBox_canFreq_currentIndexChanged(int index) // выбор частоты CAN шины
{
    // ввести проверку на состояние порта!!!!!
 //   qDebug() << "выбор частоты CAN шины; ";
    switch (index) {
        case 0: {
         //  ui->lineEdit_canFreq->setText(AddCRC(AD_COM_SET_FREQ_CAN_125, 2).toHex());
           writeSerialPort(AddCRC(AD_COM_SET_FREQ_CAN_125, 2).toHex());
        } break;
        case 1: {
         //  ui->lineEdit_canFreq->setText(AddCRC(AD_COM_SET_FREQ_CAN_250, 2).toHex());
           writeSerialPort(AddCRC(AD_COM_SET_FREQ_CAN_250, 2).toHex());
        } break;
        case 2: {
         //  ui->lineEdit_canFreq->setText(AddCRC(AD_COM_SET_FREQ_CAN_500, 2).toHex());
           writeSerialPort(AddCRC(AD_COM_SET_FREQ_CAN_500, 2).toHex());
        } break;
       // default: ui->lineEdit_canFreq->setText("не установлена частота CAN");
    }
}


void MainWindow::on_comboBox_readAllCan_currentIndexChanged(int index) // выбор фильтра CAN шины
{
 //    qDebug() << "выбор фильтра CAN шины; ";
    switch (index) {
        case 0: {
         //  ui->lineEdit_readAllCan->setText(AddCRC(AD_COM_SET_READ_ALL_CAN, 2).toHex());
           writeSerialPort(AddCRC(AD_COM_SET_READ_ALL_CAN, 2).toHex());
        } break;
        case 1: {
         //  ui->lineEdit_readAllCan->setText(AddCRC(AD_COM_SET_READ_ALL_CAN, 2).toHex());
           writeSerialPort(AddCRC(AD_COM_SET_READ_ALL_CAN, 2).toHex());
        } break;
       // default: ui->lineEdit_readAllCan->setText("не выставлен режим фильтра");
    }

}

void MainWindow::on_pushButton_setConfigAdapter_clicked() // конфигурация адаптера по комбобоксам
{
    init_setConfigAdapter();
}

void MainWindow::init_setConfigAdapter()
{
    QString setFreqCan = AD_COM_SET_FREQ_CAN_250; // по умолчанию 250
    int index = ui->comboBox_canFreq->currentIndex();
 //   qDebug() << "принудительная инициализация адаптера. index=" << index;
    switch (index) {
        case 0: {
           setFreqCan = AD_COM_SET_FREQ_CAN_125;
        } break;
        case 1: {
           setFreqCan = AD_COM_SET_FREQ_CAN_250;
        } break;
        case 2: {
          setFreqCan = AD_COM_SET_FREQ_CAN_500;
        } break;
       // default: ui->lineEdit_canFreq->setText("не установлена частота CAN");
    }
    QString dataWriteString = AddCRC(setFreqCan, 2).toHex() + AddCRC(AD_COM_SET_READ_ALL_CAN, 2).toHex();
    writeSerialPort(dataWriteString);
}

