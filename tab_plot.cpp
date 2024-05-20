#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tab_plot.h"

tab_plot::tab_plot()
{

}



void MainWindow::addGraph(){
    QStringList registersList = RegnumClass::regnumList();
    QStringList samplesList = SampleCanIdClass::fccanidList().mid(SampleCanIdClass::CAN_START_SAMPLE_ID);
    timerPlotter = new QTimer;
    connect(timerPlotter, SIGNAL(timeout()), this, SLOT(addPointToGraph()));

    ui->widget_plot_1->addGraph();
    ui->widget_plot_1->addGraph(ui->widget_plot_1->xAxis, ui->widget_plot_1->yAxis2);
    ui->widget_plot_1->graph(0)->setPen(QPen(Qt::blue));
    ui->widget_plot_1->graph(1)->setPen(QPen(Qt::green));
    ui->widget_plot_1->yAxis->setLabel("label1");

    ui->widget_plot_1->yAxis2->setVisible(true);
    ui->widget_plot_1->yAxis2->setTickLabels(true);
    ui->widget_plot_1->yAxis2->setLabel("label2");

    ui->widget_plot_2->addGraph();
    ui->widget_plot_2->addGraph(ui->widget_plot_2->xAxis, ui->widget_plot_2->yAxis2);
    ui->widget_plot_2->graph(0)->setPen(QPen(Qt::red));
    ui->widget_plot_2->graph(1)->setPen(QPen(Qt::black));
    ui->widget_plot_2->yAxis->setLabel("label1");

    ui->widget_plot_2->yAxis2->setVisible(true);
    ui->widget_plot_2->yAxis2->setTickLabels(true);
    ui->widget_plot_2->yAxis2->setLabel("label2");

    ui->comboBox_plot1->addItems(registersList);
    ui->comboBox_plot2->addItems(registersList);
    ui->comboBox_plot3->addItems(samplesList);
    ui->comboBox_plot4->addItems(samplesList);

    ui->comboBox_plot1->setCurrentIndex(RegnumClass::IREG_UOUT);
    ui->comboBox_plot2->setCurrentIndex(RegnumClass::IREG_IOUT);
    ui->comboBox_plot3->setCurrentIndex(SampleCanIdClass::CAN_DR0_PH0_T1 - SampleCanIdClass::CAN_START_SAMPLE_ID);
    ui->comboBox_plot4->setCurrentIndex(SampleCanIdClass::CAN_INV_IA - SampleCanIdClass::CAN_START_SAMPLE_ID);

    ui->lineEdit_yAxis_1->setText("500");
    ui->lineEdit_yAxis_2->setText("500");

    ui->lineEdit_yAxis_3->setText("150");
    ui->lineEdit_yAxis_4->setText("500");

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
    int regNum_plot1[2] = {regNum_1, regNum_2};

    int yAxis_1 = ui->lineEdit_yAxis_1->text().toInt();
    int yAxis_2 = ui->lineEdit_yAxis_2->text().toInt();

    int sampleNum_1 = ui->comboBox_plot3->currentIndex() + SampleCanIdClass::CAN_START_SAMPLE_ID;
    int sampleNum_2 = ui->comboBox_plot4->currentIndex() + SampleCanIdClass::CAN_START_SAMPLE_ID;
    int sampleNum_plot2[2] = {sampleNum_1, sampleNum_2};

    int yAxis_3 = ui->lineEdit_yAxis_3->text().toInt();
    int yAxis_4 = ui->lineEdit_yAxis_4->text().toInt();

    QStringList dataPlotList;
    dataPlotList.clear();

    // тестовая функция с выводом частоты двигателя
    int windowWide = ui->lineEdit_scalePlot->text().toInt(); // размер экрана плоттера


    // первый график с двумя осями
    ui->widget_plot_1->yAxis->setRange(0, yAxis_1);
    ui->widget_plot_1->yAxis->setLabel(regNumList[regNum_plot1[0]]);

    ui->widget_plot_1->yAxis2->setRange(0, yAxis_2);
    ui->widget_plot_1->yAxis2->setLabel(regNumList[regNum_plot1[1]]);

    for(int i = 0; i < 2; i++){
        int lengthBuffer_1 = PLOT_MAX_SIZE_ARR; // размер буфера парсинга данных
        xPlot.clear();
        yPlot.clear();
        if (!regDataArray[regNum_plot1[i]].flagFullBuffer) lengthBuffer_1 = regDataArray[regNum_plot1[i]].counterRegPlot;

        for(int j = 0; j < lengthBuffer_1; j++){ // заполняем массив на выдачу плоттера
            yPlot.push_back(double(regDataArray[regNum_plot1[i]].regValueScaledArr[j]));
            int deltaTime = int(regDataArray[regNum_plot1[i]].regTimeArr[j]) - (startTimeStamp + windowWide);
            if(deltaTime > 0) startTimeStamp = startTimeStamp + deltaTime;

            xPlot.push_back(double(regDataArray[regNum_plot1[i]].regTimeArr[j]));
        }
        ui->widget_plot_1->xAxis->setRange(startTimeStamp, startTimeStamp + windowWide);
        ui->widget_plot_1->graph(i)->setData(xPlot, yPlot, false);
    }
    ui->widget_plot_1->replot();

// второй график с двумя осями
    ui->widget_plot_2->yAxis->setRange(0, yAxis_3);
    ui->widget_plot_2->yAxis->setLabel(sampleNumList[sampleNum_plot2[0]]);

    ui->widget_plot_2->yAxis2->setRange(0, yAxis_4);
    ui->widget_plot_2->yAxis2->setLabel(sampleNumList[sampleNum_plot2[1]]);

    for(int i = 0; i < 2; i++){
        int lengthBuffer_2 = PLOT_MAX_SIZE_ARR; // размер буфера парсинга измерений
        xPlot.clear();
        yPlot.clear();
        if (!sampleDataArray[sampleNum_plot2[i]].flagFullBuffer) lengthBuffer_2 = sampleDataArray[sampleNum_plot2[i]].counterSamplePlot;

        for(int j = 0; j < lengthBuffer_2; j++){ // заполняем массив на выдачу плоттера
            yPlot.push_back(double(sampleDataArray[sampleNum_plot2[i]].sampleValueScaledArr[j]));
            int deltaTime = int(sampleDataArray[sampleNum_plot2[i]].sampleTimeArr[j]) - (startTimeStamp + windowWide);
            if(deltaTime > 0) startTimeStamp = startTimeStamp + deltaTime;

            xPlot.push_back(double(sampleDataArray[sampleNum_plot2[i]].sampleTimeArr[j]));
        }
        ui->widget_plot_2->xAxis->setRange(startTimeStamp, startTimeStamp + windowWide);
        ui->widget_plot_2->graph(i)->setData(xPlot, yPlot, false);
    }
    ui->widget_plot_2->replot();

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
