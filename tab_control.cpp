#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "invertor.h"

// управление инвертором

void MainWindow::on_pushButton_sendCommand_clicked()
{
    QString commandString = ui->lineEdit_testCommand->text();
//    writeSerialPort(dataWriteString);

    ui->textEdit_commandCRC->append(AddCRC((AD_COM_SET_HEAD + commandString), 2).toHex()); // провериь, что без последующего чтения идет запись и убрать!!!!
}

QString MainWindow::glueString(quint16 data, quint8 registerInv){ // !!!! без quint не оттрабатывает отрицательные значения !!!!!!
    QString writeHeader = QString::number(REGISTER_WRITE_ID, 16).rightJustified(4, '0')
            + QString::number(IREG_INV_LEN, 16).rightJustified(2, '0')
            + QString::number(registerInv, 16).rightJustified(2, '0')
            + QString::number(data, 16).rightJustified(4, '0');
   // qDebug() << "формируем команду управления: " <<  writeHeader;
    return writeHeader;
}



// формируем заголовок стандартной CAN посылки в адаптер
static quint8 numberMessage = 0;
QString MainWindow::glueAdapterHeader(){

    QString writeADHeader = AD_COM_SET_HEAD
            + QString::number(ADAPT_REG_SET_LEN, 16).rightJustified(2, '0')   // длина ПОСЫЛКИ
            + QString::number(ADAPT_TYPE_STD_CAN, 16).rightJustified(2, '0')  // признак стандартного CAN
            + QString::number(numberMessage, 16).rightJustified(2, '0');
  //  qDebug() << "формируем заголовок записи в адаптер: " <<  writeADHeader;

    numberMessage++;
    if (numberMessage > 250) numberMessage = 0;

    return writeADHeader;
}

QString MainWindow::scaledValue(qint16 value, qint16 scale, qint16 max)
{
    //------ расчет значение при наличии шкалы
    double scaledValueInt = 0;
    if((scale == 0) || (max == 0)){
        return "(" + QString::number(value, 10) + ")";
    }
    else{
        scaledValueInt = double(value) * double(scale) / double(max);
        int prec = 1;
        if (scaledValueInt >= 1000) prec = 0;
        if (scaledValueInt < 10) prec = 2;
        return QString::number(scaledValueInt, 'f',  prec); // вывод с запятой
    }
}

