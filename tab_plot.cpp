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
    ui->widget_plot->addGraph();
    ui->widget_plot->addGraph();
    ui->widget_plot->addGraph();

    ui->widget_plot->graph(0)->setPen(QPen(Qt::blue));
    ui->widget_plot->graph(1)->setPen(QPen(Qt::green));
    ui->widget_plot->graph(2)->setPen(QPen(Qt::red));
    ui->widget_plot->graph(3)->setPen(QPen(Qt::black));


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

    ui->lineEdit_yAxis_1->setStyleSheet("color: blue");
    ui->lineEdit_yAxis_2->setStyleSheet("color: green");
    ui->lineEdit_yAxis_3->setStyleSheet("color: red");
    ui->lineEdit_yAxis_4->setStyleSheet("color: black");
}

void MainWindow::addPointToGraph(){
    quint32 time_stamp_32 = regDataArray[RegnumClass::IREG_FREQ].time_stamp_32;
    if(startTimeStamp == 0) {
        startTimeStamp = int(time_stamp_32); // стартовый отрезок времени с каждым запуском программы
        qDebug() << "стартуем с отметки " << startTimeStamp;
        return;
    }
    // по выбранным в комбобоксе регистрам смотрим их значения

    int regNum_1 = ui->comboBox_plot1->currentIndex();
    int regNum_2 = ui->comboBox_plot2->currentIndex();
    int regNum_3 = ui->comboBox_plot3->currentIndex();
    int regNum_4 = ui->comboBox_plot4->currentIndex();
    int regNum[4] = {regNum_1, regNum_2, regNum_3, regNum_4};

    int yAxis_1 = ui->lineEdit_yAxis_1->text().toInt();
    int yAxis_2 = ui->lineEdit_yAxis_2->text().toInt();
    int yAxis_3 = ui->lineEdit_yAxis_3->text().toInt();
    int yAxis_4 = ui->lineEdit_yAxis_4->text().toInt();
    int yAxis[4] = {yAxis_1, yAxis_2, yAxis_3, yAxis_4};

    QStringList dataPlotList;
    dataPlotList.clear();

    // тестовая функция с выводом частоты двигателя
    int windowWide = ui->lineEdit_scalePlot->text().toInt(); // размер экрана плоттера

    for(int i = 0; i < 4; i++){
        ui->widget_plot->yAxis->setRange(-10, yAxis[i] +10);
        ui->widget_plot->yAxis->setLabel(regNumList[regNum[i]]);

        xPlot.clear();
        yPlot.clear();

        int lengthBuffer = PLOT_MAX_SIZE_ARR;
        if (!regDataArray[regNum[i]].flagFullBuffer) lengthBuffer = regDataArray[regNum[i]].counterRegPlot;
    //    dataPlotList.append("lengthBuffer = " + QString::number(lengthBuffer, 10)); // количество данных в буфере

        for(int j = 0; j < lengthBuffer; j++){ // заполняем массив на выдачу плоттера
            yPlot.push_back(double(regDataArray[regNum[i]].regValueScaledArr[j]));
            int deltaTime = int(regDataArray[regNum[i]].regTimeArr[j]) - (startTimeStamp + windowWide);
            if(deltaTime > 0) startTimeStamp = startTimeStamp + deltaTime;

            xPlot.push_back(double(regDataArray[regNum[i]].regTimeArr[j]));

//            dataPlotList.append(QString::number(i, 10).leftJustified(5, ' ') + ": Y=" + QString::number(int(yPlot[i]), 10) +
//                                ": X=" + QString::number(int(xPlot[j]-double(startTimeStamp)), 10));
        }
        ui->widget_plot->xAxis->setRange(startTimeStamp, startTimeStamp + windowWide);

    //    ui->textEdit_dataPlot->setText(dataPlotList.join('\n')); // дежурный список. Убрать!!!

        ui->widget_plot->graph(i)->setData(xPlot, yPlot, false);
    }

    ui->widget_plot->replot();
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
