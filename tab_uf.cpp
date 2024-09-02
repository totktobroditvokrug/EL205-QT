#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tab_plot.h"

// контроль выбранной ячейки
static int selectedRowUF = 10;
static int selectedColumnUF = 10;
static int startPointTable = RegnumClass::IREG_UF_U0 - 1;  // мнимое начало таблицы, где подставим нулевую частоту


void MainWindow::addGraph_UF(){
    timerPlotterUF = new QTimer;
    connect(timerPlotterUF, SIGNAL(timeout()), this, SLOT(addPointUF()));

    ui->widget_plot_UF->addGraph();
    ui->widget_plot_UF->addGraph(ui->widget_plot_UF->xAxis, ui->widget_plot_UF->yAxis2);
    ui->widget_plot_UF->graph(0)->setPen(QPen(Qt::blue));
    ui->widget_plot_UF->yAxis->setTickLabelColor(Qt::blue);

    ui->widget_plot_UF->yAxis->setLabel("voltage");

    ui->widget_plot_UF->yAxis2->setVisible(true);
    ui->widget_plot_UF->yAxis2->setTickLabels(true);
    ui->widget_plot_UF->graph(1)->setPen(QPen(Qt::green));
    ui->widget_plot_UF->yAxis2->setTickLabelColor(Qt::green);
    ui->widget_plot_UF->yAxis2->setLabel("current");

    ui->widget_plot_UF->xAxis->setLabel("frequency");
    ui->widget_plot_UF->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->widget_plot_UF->graph(1)->setScatterStyle(QCPScatterStyle::ssCircle);
}

void MainWindow::addPointUF(){
    checkUF();

    if( ui->table_UF->signalsBlocked()) ui->table_UF->blockSignals(false);

    double maxFreq = 0;
    double maxVlt = 0;
    double maxCurr = 0;

    QVector<double> freq, vlt, curr;

    freq = QVector<double>(MAX_SIZE_UF);
    vlt = QVector<double>(MAX_SIZE_UF);
    curr = QVector<double>(MAX_SIZE_UF);

    for(int i = 0; i < MAX_SIZE_UF; i++){
        if (i == 0){
            freq[0] = 0; // первая точка графика всегда с нулевой частотой
        }
        else {
            freq[i] = regDataArray[startPointTable + i * 3].value.Reg16;
            if(freq[i] > maxFreq) maxFreq = freq[i]; // поиск максимума частоты
        }

        vlt[i] = regDataArray[startPointTable + i * 3 + 1].value.Reg16;
        curr[i] = regDataArray[startPointTable + i * 3 + 2].value.Reg16;
        if(vlt[i] > maxVlt) maxVlt = vlt[i]; // поиск максимума напряжения
        if(curr[i] > maxCurr) maxCurr = curr[i]; // поиск максимума тока

        if((i < ui->table_UF->rowCount()) & (i != selectedRowUF - 1)) {
            ui->table_UF->setItem(i + 1, 0, new QTableWidgetItem(QString::number(quint16(freq[i]), 10)));
            ui->table_UF->setItem(i + 1, 1, new QTableWidgetItem(QString::number(quint16(vlt[i]), 10)));
            ui->table_UF->setItem(i + 1, 2, new QTableWidgetItem(QString::number(quint16(curr[i]), 10)));
        }
    }

    ui->widget_plot_UF->graph(0)->setData(freq, vlt, false);
    ui->widget_plot_UF->graph(1)->setData(freq, curr, false);

    ui->widget_plot_UF->xAxis->setRange(0, maxFreq);

    ui->widget_plot_UF->yAxis->setRange(0, maxVlt + 10);
    ui->widget_plot_UF->yAxis2->setRange(0, maxCurr +20);

    ui->widget_plot_UF->replot();
}


void MainWindow::on_table_UF_cellDoubleClicked(int row, int column)
{
    selectedRowUF = row;
    selectedColumnUF = column;
//    qDebug() << "двойной щелчок на ячейке" << row << column << "value =" << ui->table_UF->item(row, column)->text();
}


void MainWindow::on_table_UF_cellChanged(int row, int column)
{
    if((row == selectedRowUF) && (column == selectedColumnUF)){
        if((!!(ui->table_UF->item(1, column))) && (!!(ui->table_UF->item(row, column)))){
            ui->table_UF->item(row, column)->setForeground(Qt::green);
            QString newValueString = ui->table_UF->item(row, column)->text();
            qint16 regDataPrimary = newValueString.toShort();
//            qDebug() << "выбранная ячейка HEX:" <<  QString::number(regDataPrimary, 16).rightJustified(4, '0') << " BIN: " << QString::number(regDataPrimary, 2).rightJustified(16, '0');
            qint16 regData = changeHiLowBytes(regDataPrimary);
//            qDebug() << "смена байт HEX:" <<  QString::number(regData, 16).rightJustified(4, '0') << " BIN: " << QString::number(regData, 2).rightJustified(16, '0');
            quint8 regNumber =  quint8(startPointTable + (row-1) * 3 + column);
//            qDebug() << "cellChanged ячейки №: "  << regNumber
//                     << "; new value=" << QString::number(changeHiLowBytes(regData), 10);

            QString commandString = AddCRC((glueAdapterHeader() + glueString(quint16(regData), regNumber)), 2).toHex();
//            ui->textEdit_commandCRC->append(commandString);
            writeSerialPort(commandString);
           selectedRowUF = 10;
           selectedColumnUF = 10;
           regDataArray[regNumber].flagNewData = false;
           ui->table_UF->item(row, column)->setText(newValueString);
            ui->table_UF->blockSignals(true); // заблокировать фоновые события
        }
    }
}

void MainWindow::checkUF(){
    if(!ui->lineEdit_f_nom->hasFocus())    ui->lineEdit_f_nom->setText(QString::number(regDataArray[RegnumClass::IREG_M_F_NOM].value.Reg16, 10));
    if(!ui->lineEdit_f_err->hasFocus())    ui->lineEdit_f_err->setText(QString::number(regDataArray[RegnumClass::IREG_FREQ_ERROR].value.Reg16, 10));
    if(!ui->lineEdit_u_nom->hasFocus())    ui->lineEdit_u_nom->setText(QString::number(regDataArray[RegnumClass::IREG_M_U_NOM].value.Reg16, 10));
    if(!ui->lineEdit_ku_start->hasFocus()) ui->lineEdit_ku_start->setText(QString::number(regDataArray[RegnumClass::IREG_M_KU_START].value.Reg16, 10));
    if(!ui->lineEdit_i_nom->hasFocus())    ui->lineEdit_i_nom->setText(QString::number(regDataArray[RegnumClass::IREG_M_I_NOM].value.Reg16, 10));
    if(!ui->lineEdit_ki_start->hasFocus()) ui->lineEdit_ki_start->setText(QString::number(regDataArray[RegnumClass::IREG_M_KI_START].value.Reg16, 10));
}

void MainWindow::on_lineEdit_f_nom_editingFinished()
{
    QString newData = ui->lineEdit_f_nom->text();
    QString commandString = AddCRC((glueAdapterHeader() +
                            glueString(changeHiLowBytes_uint(quint16(newData.toShort())), RegnumClass::IREG_M_F_NOM)), 2).toHex();

    writeSerialPort(commandString);
}

void MainWindow::on_lineEdit_f_err_editingFinished()
{
    QString newData = ui->lineEdit_f_err->text();
    QString commandString = AddCRC((glueAdapterHeader() +
                            glueString(changeHiLowBytes_uint(quint16(newData.toShort())), RegnumClass::IREG_FREQ_ERROR)), 2).toHex();

    writeSerialPort(commandString);
}

void MainWindow::on_lineEdit_u_nom_editingFinished()
{
    QString newData = ui->lineEdit_u_nom->text();
    QString commandString = AddCRC((glueAdapterHeader() +
                            glueString(changeHiLowBytes_uint(quint16(newData.toShort())), RegnumClass::IREG_M_U_NOM)), 2).toHex();

    writeSerialPort(commandString);
}

void MainWindow::on_lineEdit_ku_start_editingFinished()
{
    QString newData = ui->lineEdit_ku_start->text();
    QString commandString = AddCRC((glueAdapterHeader() +
                            glueString(changeHiLowBytes_uint(quint16(newData.toShort())), RegnumClass::IREG_M_KU_START)), 2).toHex();

    writeSerialPort(commandString);
}

void MainWindow::on_lineEdit_i_nom_editingFinished()
{
    QString newData = ui->lineEdit_i_nom->text();
    QString commandString = AddCRC((glueAdapterHeader() +
                            glueString(changeHiLowBytes_uint(quint16(newData.toShort())), RegnumClass::IREG_M_I_NOM)), 2).toHex();

    writeSerialPort(commandString);
}

void MainWindow::on_lineEdit_ki_start_editingFinished()
{
    QString newData = ui->lineEdit_ki_start->text();
    QString commandString = AddCRC((glueAdapterHeader() +
                            glueString(changeHiLowBytes_uint(quint16(newData.toShort())), RegnumClass::IREG_M_KI_START)), 2).toHex();

    writeSerialPort(commandString);
}



