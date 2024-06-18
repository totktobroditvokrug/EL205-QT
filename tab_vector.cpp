#include "tab_vector.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qpaintwidget.h"


tab_vector::tab_vector()
{

}

void MainWindow::checkVector(){
    int Uconv = int(regDataArray[RegnumClass::IREG_UOUT].regValueScaled);

 //   ui->lineEdit_UconvMax->setText(QString::number(regDataArray[RegnumClass::IREG_UOUT].max.Reg16, 10));
    int Umax = ui->lineEdit_UconvMax->text().toInt();

    int iA = int(regDataArray[RegnumClass:: IREG_IA_RMS].regValueScaled);
    int iB = int(regDataArray[RegnumClass:: IREG_IB_RMS].regValueScaled);
    int iC = int(regDataArray[RegnumClass:: IREG_IC_RMS].regValueScaled);
    int iMax = ui->lineEdit_IconvMax->text().toInt();

    int phA = int(regDataArray[RegnumClass:: IREG_PHA].regValueScaled);
    int phB = int(regDataArray[RegnumClass:: IREG_PHB].regValueScaled);
    int phC = int(regDataArray[RegnumClass:: IREG_PHC].regValueScaled);

    QString uConv = QString::number(Uconv, 10);
    QString fConv = QString::number(int(regDataArray[RegnumClass::IREG_FREQ].regValueScaled), 10);

    QString iA_RMS = QString::number(iA, 10);
    QString iB_RMS = QString::number(iB, 10);
    QString iC_RMS = QString::number(iC, 10);

    QString ph_A = QString::number(phA, 10);
    QString ph_B = QString::number(phB, 10);
    QString ph_C = QString::number(phC, 10);

    ui->lineEdit_Uconv->setText(uConv);
    ui->lineEdit_Fconv->setText(fConv);

    ui->lineEdit_IconvA->setText(iA_RMS);
    ui->lineEdit_IconvB->setText(iB_RMS);
    ui->lineEdit_IconvC->setText(iC_RMS);

    ui->lineEdit_PhA->setText(ph_A);
    ui->lineEdit_PhB->setText(ph_B);
    ui->lineEdit_PhC->setText(ph_C);


//    emit sendUconv(Uconv, Umax); // эмиссия сигнала со значением напряжения
//    emit sendIconv(iA, iB, iC, iMax, phA, phB, phC);
    bool showUph = ui->checkBox_uPh->isChecked();
    bool showUline = ui->checkBox_uLine->isChecked();
    bool showI = ui->checkBox_i->isChecked();

    ui->widget_paintVector->setUconv(Uconv, Umax, showUph, showUline);
    ui->widget_paintVector->setIconv(iA, iB, iC, iMax, phA, phB, phC, showI);
    ui->widget_paintVector->repaint();

}
