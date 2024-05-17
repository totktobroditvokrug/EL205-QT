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
    int windowWide = ui->lineEdit_scalePlot->text().toInt(); // размер экрана плоттера
    dataPlotList.clear();
    // тестовая функция с выводом частоты двигателя
    quint32 time_stamp_32 = regDataArray[RegnumClass::IREG_FREQ].time_stamp_32;
    ui->widget_plot->yAxis->setRange(0, 100);
    ui->widget_plot->yAxis->setLabel(regNumList[RegnumClass::IREG_FREQ]);
    if(startTimeStamp == 0) {
        startTimeStamp = int(time_stamp_32); // стартовый отрезок времени с каждым запуском программы
        qDebug() << "стартуем с отметки " << startTimeStamp;
    }
    else{

        // todo тестовый вывод графика частоты

            xPlot.clear();
            yPlot.clear();

        int lengthBuffer = PLOT_MAX_SIZE_ARR;
        if (!regDataArray[RegnumClass::IREG_FREQ].flagFullBuffer) lengthBuffer = regDataArray[RegnumClass::IREG_FREQ].counterRegPlot;
        dataPlotList.append("lengthBuffer = " + QString::number(lengthBuffer, 10)); // количество данных в буфере

        for(int i = 0; i < lengthBuffer; i++){ // заполняем массив на выдачу плоттера
            yPlot.push_back(double(regDataArray[RegnumClass::IREG_FREQ].regValueScaledArr[i]));
            int deltaTime = int(regDataArray[RegnumClass::IREG_FREQ].regTimeArr[i]) - (startTimeStamp + windowWide);
            if(deltaTime > 0) startTimeStamp = startTimeStamp + deltaTime;

            xPlot.push_back(double(regDataArray[RegnumClass::IREG_FREQ].regTimeArr[i]));

            dataPlotList.append(QString::number(i, 10).leftJustified(5, ' ') + ": Y=" + QString::number(int(yPlot[i]), 10) +
                                ": X=" + QString::number(int(xPlot[i]-double(startTimeStamp)), 10));
        }
        ui->widget_plot->xAxis->setRange(startTimeStamp, startTimeStamp + windowWide);

        ui->textEdit_dataPlot->setText(dataPlotList.join('\n')); // дежурный список. Убрать!!!

        ui->widget_plot->graph(0)->setData(xPlot, yPlot, false);
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
