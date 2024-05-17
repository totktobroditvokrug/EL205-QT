#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tab_plot.h"

tab_plot::tab_plot()
{

}



void MainWindow::addGraph(){
    QStringList registersList = RegnumClass::regnumList();
    timerPlotter = new QTimer;
    connect(timerPlotter, SIGNAL(timeout()), this, SLOT(addPointToGraph()));

    ui->widget_plot->addGraph();


    ui->comboBox_plot1->addItems(registersList);
    ui->comboBox_plot2->addItems(registersList);
    ui->comboBox_plot3->addItems(registersList);
    ui->comboBox_plot4->addItems(registersList);

    ui->comboBox_plot1->setCurrentIndex(RegnumClass::IREG_UOUT);
    ui->comboBox_plot2->setCurrentIndex(RegnumClass::IREG_IOUT);
    ui->comboBox_plot3->setCurrentIndex(RegnumClass::IREG_FREQ_REF);
    ui->comboBox_plot4->setCurrentIndex(RegnumClass::IREG_FREQ);

    ui->lineEdit_yAxis_1->setText("500");
    ui->lineEdit_yAxis_2->setText("500");
    ui->lineEdit_yAxis_3->setText("200");
    ui->lineEdit_yAxis_4->setText("200");
}

void MainWindow::addPointToGraph(){
    // по выбранным в комбобоксе регистрам смотрим их значения
    int regNum_1 = ui->comboBox_plot1->currentIndex();
    int regNum_2 = ui->comboBox_plot2->currentIndex();
    int regNum_3 = ui->comboBox_plot3->currentIndex();
    int regNum_4 = ui->comboBox_plot4->currentIndex();

    int yAxis_1 = ui->lineEdit_yAxis_1->text().toInt();
    int yAxis_2 = ui->lineEdit_yAxis_2->text().toInt();
    int yAxis_3 = ui->lineEdit_yAxis_3->text().toInt();
    int yAxis_4 = ui->lineEdit_yAxis_4->text().toInt();

    QStringList dataPlotList;
    int windowWide = ui->lineEdit_scalePlot->text().toInt(); // размер экрана плоттера
    dataPlotList.clear();
    // тестовая функция с выводом частоты двигателя
    quint32 time_stamp_32 = regDataArray[regNum_4].time_stamp_32;
    ui->widget_plot->yAxis->setRange(-10, yAxis_4 +10);
    ui->widget_plot->yAxis->setLabel(regNumList[regNum_4]);
    if(startTimeStamp == 0) {
        startTimeStamp = int(time_stamp_32); // стартовый отрезок времени с каждым запуском программы
        qDebug() << "стартуем с отметки " << startTimeStamp;
    }
    else{

        // todo тестовый вывод графика частоты

            xPlot.clear();
            yPlot.clear();

        int lengthBuffer = PLOT_MAX_SIZE_ARR;
        if (!regDataArray[regNum_4].flagFullBuffer) lengthBuffer = regDataArray[regNum_4].counterRegPlot;
        dataPlotList.append("lengthBuffer = " + QString::number(lengthBuffer, 10)); // количество данных в буфере

        for(int i = 0; i < lengthBuffer; i++){ // заполняем массив на выдачу плоттера
            yPlot.push_back(double(regDataArray[regNum_4].regValueScaledArr[i]));
            int deltaTime = int(regDataArray[regNum_4].regTimeArr[i]) - (startTimeStamp + windowWide);
            if(deltaTime > 0) startTimeStamp = startTimeStamp + deltaTime;

            xPlot.push_back(double(regDataArray[regNum_4].regTimeArr[i]));

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
