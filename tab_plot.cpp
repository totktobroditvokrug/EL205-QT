#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tab_plot.h"

tab_plot::tab_plot()
{

}



void MainWindow::addGraph(){
    timerPlotter = new QTimer;
    connect(timerPlotter, SIGNAL(timeout()), this, SLOT(addPointToGraph()));

    ui->widget_plot->addGraph();
}

void MainWindow::addPointToGraph(){
    QStringList dataPlotList;
    dataPlotList.clear();
    // тестовая функция с выводом частоты двигателя
    quint32 time_stamp_32 = regDataArray[RegnumClass::IREG_FREQ].time_stamp_32;
    ui->widget_plot->yAxis->setRange(0, 100);
    if(startTimeStamp == 0) {
        startTimeStamp = time_stamp_32; // стартовый отрезок времени с каждым запуском программы
        qDebug() << "стартуем с отметки " << startTimeStamp;
    }
    else{
        ui->widget_plot->xAxis->setRange(startTimeStamp, startTimeStamp + PLOT_MAX_SIZE_ARR * 1000);
        // todo тестовый вывод графика частоты
        int lengthBuffer = PLOT_MAX_SIZE_ARR;
        if (!regDataArray[RegnumClass::IREG_FREQ].flagFullBuffer) lengthBuffer = regDataArray[RegnumClass::IREG_FREQ].counterRegPlot;
        dataPlotList.append("lengthBuffer = " + QString::number(lengthBuffer, 10));
        for(int i = 0; i < lengthBuffer; i++){
            //
            //  xPlot[i] = double(i);
            yPlot[i] = double(regDataArray[RegnumClass::IREG_FREQ].regValue[i]) * double(regDataArray[RegnumClass::IREG_FREQ].scale.Reg16) /
                    double(regDataArray[RegnumClass::IREG_FREQ].maxValue.Reg16);
            xPlot[i] = double(regDataArray[RegnumClass::IREG_FREQ].regTime[i]);
            dataPlotList.append(QString::number(i, 10).leftJustified(5, ' ') + ": Y=" + QString::number(int(yPlot[i]), 10) +
                                ": X=" + QString::number(int(xPlot[i]-double(startTimeStamp)), 10));
        }
        ui->textEdit_dataPlot->setText(dataPlotList.join('\n'));
        ui->widget_plot->graph(0)->setData(xPlot, yPlot, true);
        ui->widget_plot->replot();
    }

}

void MainWindow::on_pushButton_holdPlot_clicked()
{
    if(timerPlotter->isActive()){
        timerPlotter->stop();
        ui->pushButton_holdPlot->setText("START");
    }
    else{
        timerPlotter->start(ui->lineEdit_freqPlot->text().toInt());
        ui->pushButton_holdPlot->setText("HOLD");
    }
}

void MainWindow::on_pushButton_restartPlot_clicked()
{
    quint32 time_stamp_32 = regDataArray[RegnumClass::IREG_FREQ].time_stamp_32;
    if(time_stamp_32 == 0) {
        qDebug() << "time_stamp_32 не приходил ";
    }
    else{
        qDebug() << "переустановка начала координат " << time_stamp_32;
        startTimeStamp = time_stamp_32;
        ui->widget_plot->xAxis->setRange(time_stamp_32, time_stamp_32 + PLOT_MAX_SIZE_ARR * 1000);
    }
}
