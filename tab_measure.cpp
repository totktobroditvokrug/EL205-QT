#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "measure.h"

void MainWindow::on_pushButton_testMeasure_clicked(){
    measure::debug();
}

// Временное поле хэш-таблиц стандартного и расширенного CAN
void MainWindow::displayHashID()
{
    QStringList canFromHash;
    canFromHash.clear();
    ui->textEdit_hash->clear();
    ui->textEdit_hash->append("Standart CAN");
    canFromHash.append("Standart CAN");
    QHashIterator<quint16, QByteArray> iteratorStantart(canByIdStandart);
    for (auto i = canByIdStandart.cbegin(), end = canByIdStandart.cend(); i != end; ++i){
        ui->textEdit_hash->append(QString::number(i.key(), 16).rightJustified(4, '0') + " : " + QString::fromUtf8(i.value().toHex(' ')));
        canFromHash.append(QString::number(i.key(), 16).rightJustified(4, '0') + " : " + QString::fromUtf8(i.value().toHex(' ')));
    }
    ui->textEdit_hash->append("Extended CAN");
    canFromHash.append("Extended CAN");
    QHashIterator<quint32, QByteArray> iteratorExtended(canByIdExtended);
    for (auto i = canByIdExtended.cbegin(), end = canByIdExtended.cend(); i != end; ++i){
        ui->textEdit_hash->append(QString::number(i.key(), 16).rightJustified(8, '0') + " : " + QString::fromUtf8(i.value().toHex(' ')));
        canFromHash.append(QString::number(i.key(), 16).rightJustified(8, '0') + " : " + QString::fromUtf8(i.value().toHex(' ')));
    }
    if(ui->radioButton_fromHashTable->isChecked()) ui->textEdit_dataRead->setText(canFromHash.join('\n'));
}

