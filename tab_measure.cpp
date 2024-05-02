#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "measure.h"

void MainWindow::on_pushButton_testMeasure_clicked(){
    measure::debug();
}

// Временное поле хэш-таблиц стандартного и расширенного CAN
void MainWindow::displayHashID()
{
    // qDebug() << "displayHashID";
    ui->textEdit_hash->clear();
    ui->textEdit_hash->append("Standart CAN");
    QHashIterator<quint16, QByteArray> iteratorStantart(canByIdStandart);
    for (auto i = canByIdStandart.cbegin(), end = canByIdStandart.cend(); i != end; ++i){
  //     qDebug() << "ID_std: " + QString::number(i.key(), 16);
        ui->textEdit_hash->append(QString::number(i.key(), 16).rightJustified(4, '0') + " : " + QString::fromUtf8(i.value().toHex(' ')));
    }
    ui->textEdit_hash->append("Extended CAN");
    QHashIterator<quint32, QByteArray> iteratorExtended(canByIdExtended);
    for (auto i = canByIdExtended.cbegin(), end = canByIdExtended.cend(); i != end; ++i){
   //     qDebug() << "ID_ext: " + QString::number(i.key(), 16);
        ui->textEdit_hash->append(QString::number(i.key(), 16).rightJustified(8, '0') + " : " + QString::fromUtf8(i.value().toHex(' ')));
    }
}

