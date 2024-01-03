#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_pushButton_searchListPort_clicked() // список доступных портов
{
    QString description;
    QString manufacturer;
    QString serialNumber;
    // Получить доступные последовательные порты
    QList<QSerialPortInfo> serialPortInfos = QSerialPortInfo::availablePorts();

         // Выводим количество последовательных портов, которые может использовать текущая система
    qDebug() << "Total numbers of ports: " << serialPortInfos.count();

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
       qDebug() << "Description: " << list;
    }
}

void MainWindow::initTabCan(){
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


    ui->statusbar->showMessage("well");

    ui->comboBox_canFreq->addItem(tr("125 kb/s"), 0);
    ui->comboBox_canFreq->addItem(tr("250 kb/s"), 1);
    ui->comboBox_canFreq->addItem(tr("500 kb/s"), 2);

    ui->comboBox_readAllCan->addItem(tr("All"), 0);
    ui->comboBox_readAllCan->addItem(tr("Mask"), 1);

    ui->pushButton_disconnect->setEnabled(false);
    ui->pushButton_sendMessage->setEnabled(false);
    ui->pushButton_readOnce->setEnabled(false);
    ui->pushButton_stopRead->setEnabled(false);
    ui->pushButton_startRead->setEnabled(false);
    ui->comboBox_canFreq->setEnabled(false);
}


void MainWindow::on_pushButton_connect_clicked()
{
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
        qDebug() << "setBaudRate" << ui->comboBox_baudRate->currentText().toInt();
    }
    else qDebug() << "Error setBaudRate";


        // Устанавливаем биты данных
    switch (ui->comboBox_dataBits->currentText().toUInt()) {
        case 5: if (serial->setDataBits(QSerialPort::Data5)) {
           ui->label_dataOk->setText("ok");
           qDebug() << "setDataBits: " << (ui->comboBox_dataBits->currentText().toInt());
        } break;
    case 6: if (serial->setDataBits(QSerialPort::Data6)){
            ui->label_dataOk->setText("ok");
            qDebug() << "setDataBits: " << (ui->comboBox_dataBits->currentText().toInt());
        } break;
    case 7: if (serial->setDataBits(QSerialPort::Data7)){
            ui->label_dataOk->setText("ok");
            qDebug() << "setDataBits: " << (ui->comboBox_dataBits->currentText().toInt());
        } break;
    case 8: if (serial->setDataBits(QSerialPort::Data8)){
            ui->label_dataOk->setText("OK");
            qDebug() << "setDataBits: " << (ui->comboBox_dataBits->currentText().toInt());
        } break;
        default: qDebug() << "Error setDataBits"; break;
    }


        // Установить бит четности
    qDebug() << "setParity: " << ui->comboBox_parity->currentIndex();
    switch(ui->comboBox_parity->currentIndex()) {
        case 0:
            if (serial->setParity(QSerialPort::NoParity)){
               ui->label_parityOk->setText("OK");
            }
             break;
        case 1:
            if (serial->setParity(QSerialPort::EvenParity)){
                ui->label_parityOk->setText("ok");
            } break;
        case 2:
            if (serial->setParity(QSerialPort::OddParity)){
                ui->label_parityOk->setText("ok");
            } break;
        case 3:
            if (serial->setParity(QSerialPort::MarkParity)){
                ui->label_parityOk->setText("ok");
            } break;
        case 4:
            if (serial->setParity(QSerialPort::SpaceParity)){
                ui->label_parityOk->setText("ok");
            } break;
        default: qDebug() << "Error parity"; break;
    }


        // Установить стоп-бит
    qDebug() << "setStopBits: " << (ui->comboBox_stopBit->currentText().toInt());
    switch (ui->comboBox_stopBit->currentText().toUInt()) {
        case 1: if (serial->setStopBits(QSerialPort::OneStop)){
            ui->label_stopOk->setText("ok");
        } break;
    case 2:  if (serial->setStopBits(QSerialPort::TwoStop)){
        ui->label_stopOk->setText("ok");
        } break;
        default: qDebug() << "Error stop bit"; break;
    }


    // Установить управление потоком

    qDebug() << "setFlowControl: " << ui->comboBox_flowBit->currentIndex();

    switch (ui->comboBox_flowBit->currentIndex()) {
        case 0:
            if (serial->setFlowControl(QSerialPort::NoFlowControl)){
                ui->label_flowOk->setText("OK");
            } break;
        case 1:
            if (serial->setFlowControl(QSerialPort::HardwareControl)){
                ui->label_flowOk->setText("OK");
            } break;
        case 2:
            if (serial->setFlowControl(QSerialPort::SoftwareControl)){
                ui->label_flowOk->setText("OK");
            } break;
        default: qDebug() << "Error setFlowControl"; break;
    }

    // пробуем подключится
    if (!serial->open(QIODevice::ReadWrite)) {
        // если подключится не получится, то покажем сообщение с ошибкой
        qDebug()<<"Port opening error"<<serial->errorString();
        ui->statusbar->showMessage("Ошибка: " + serial->errorString());
        ui->pushButton_disconnect->setEnabled(true);
        return;
    }
    qDebug()<<"Serial port is OPEN: ";
    ui->statusbar->showMessage("Serial port is OPEN");

    ui->pushButton_disconnect->setEnabled(true);
    ui->pushButton_sendMessage->setEnabled(true);
    ui->pushButton_readOnce->setEnabled(true);
    ui->pushButton_stopRead->setEnabled(true);
    ui->pushButton_startRead->setEnabled(true);

    ui->comboBox_canFreq->setEnabled(true);
    ui->pushButton_setConfigAdapter->setEnabled(true);
}

void MainWindow::on_pushButton_disconnect_clicked()
{
    ui->pushButton_disconnect->setEnabled(false);
    serial->close();
    qDebug()<<"Serial port is CLOSE: ";
    ui->statusbar->showMessage("Serial port is CLOSE");
    ui->txtOutput->clear();

    ui->comboBox_baudRate->setEnabled(true);
    ui->comboBox_dataBits->setEnabled(true);
    ui->comboBox_flowBit->setEnabled(true);
    ui->comboBox_parity->setEnabled(true);
    ui->comboBox_serialPort->setEnabled(true);
    ui->comboBox_stopBit->setEnabled(true);

    ui->pushButton_sendMessage->setEnabled(false);
    ui->pushButton_readOnce->setEnabled(false);
    ui->pushButton_stopRead->setEnabled(false);
    ui->pushButton_startRead->setEnabled(false);
    ui->pushButton_connect->setEnabled(true);

    ui->pushButton_setConfigAdapter->setEnabled(false);
    ui->comboBox_canFreq->setEnabled(false);

    ui->label_nameOk->setText("-");
    ui->label_baudOk->setText("-");
    ui->label_dataOk->setText("-");
    ui->label_flowOk->setText("-");
    ui->label_parityOk->setText("-");
    ui->label_stopOk->setText("-");
    ui->label_flowOk->setText("-");
}

void MainWindow::on_comboBox_canFreq_currentIndexChanged(int index) // выбор частоты CAN шины
{
    switch (index) {
        case 0: {
           ui->lineEdit_canFreq->setText(AddCRC(AD_COM_SET_FREQ_CAN_125, 2).toHex());
        } break;
        case 1: {
           ui->lineEdit_canFreq->setText(AddCRC(AD_COM_SET_FREQ_CAN_250, 2).toHex());
        } break;
        case 2: {
           ui->lineEdit_canFreq->setText(AddCRC(AD_COM_SET_FREQ_CAN_500, 2).toHex());
        } break;
        default: ui->lineEdit_canFreq->setText("не установлена частота CAN");
    }
}

void MainWindow::on_pushButton_setConfigAdapter_clicked() // конфигурация адаптера по комбобоксам
{
    QString dataWriteString = ui->lineEdit_canFreq->text() + ui->lineEdit_readAllCan->text();
    writeSerialPort(dataWriteString);
}

void MainWindow::on_comboBox_readAllCan_currentIndexChanged(int index) // выбор фильтра CAN шины
{
    switch (index) {
        case 0: {
           ui->lineEdit_readAllCan->setText(AddCRC(AD_COM_SET_READ_ALL_CAN, 2).toHex());
        } break;
        case 1: {
           ui->lineEdit_readAllCan->setText(AddCRC(AD_COM_SET_READ_ALL_CAN, 2).toHex());
        } break;
        default: ui->lineEdit_readAllCan->setText("не выставлен режим фильтра");
    }
}

//------------------ Запись в serialport ------------------
void MainWindow::writeSerialPort(QString dataWriteString)
{
    QByteArray dataWriteIn = QByteArray::fromHex(dataWriteString.toUtf8()); // данные из строки textEdit_sendMessage
//    qDebug() << "функция записи: " << dataWriteString;
    serial->write(dataWriteIn);
    serial->waitForBytesWritten();
}

//------------------ Чтение из serialport -----------------
QByteArray MainWindow::readSerialPort()
{
    QByteArray dataRead;
    while (serial->waitForReadyRead(30)) {
        dataRead = serial->readAll();
    }

    QByteArray text = dataRead.toHex();
//    qDebug() << "приняли: " << dataRead.toHex('/');
    return text;
}

void MainWindow::on_pushButton_sendMessage_clicked() // записать произвольную команду в сериалпорт
{
    QString dataWriteString = ui->textEdit_sendMessage->toPlainText();
    writeSerialPort(dataWriteString);
    ui->txtOutput->append(readSerialPort()); // провериь, что без последующего чтения идет запись и убрать!!!!
}
