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
    int regNum_plot1[2] = {regNum_1, regNum_2};

    int regNum_3 = ui->comboBox_plot3->currentIndex();
    int regNum_4 = ui->comboBox_plot4->currentIndex();
    int regNum_plot2[2] = {regNum_3, regNum_4};


    int yAxis_1 = ui->lineEdit_yAxis_1->text().toInt();
    int yAxis_2 = ui->lineEdit_yAxis_2->text().toInt();
    int yAxis_3 = ui->lineEdit_yAxis_3->text().toInt();
    int yAxis_4 = ui->lineEdit_yAxis_4->text().toInt();

    QStringList dataPlotList;
    dataPlotList.clear();

    // тестовая функция с выводом частоты двигателя
    int windowWide = ui->lineEdit_scalePlot->text().toInt(); // размер экрана плоттера
    int lengthBuffer = PLOT_MAX_SIZE_ARR; // размер буфера парсинга данных

    // первый график с двумя осями
    ui->widget_plot_1->yAxis->setRange(-10, yAxis_1 +10);
    ui->widget_plot_1->yAxis->setLabel(regNumList[regNum_plot1[0]]);

    ui->widget_plot_1->yAxis2->setRange(-10, yAxis_2 +10);
    ui->widget_plot_1->yAxis2->setLabel(regNumList[regNum_plot1[1]]);


    for(int i = 0; i < 2; i++){
        xPlot.clear();
        yPlot.clear();
        if (!regDataArray[regNum_plot1[i]].flagFullBuffer) lengthBuffer = regDataArray[regNum_plot1[i]].counterRegPlot;

        for(int j = 0; j < lengthBuffer; j++){ // заполняем массив на выдачу плоттера
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
    ui->widget_plot_2->yAxis->setRange(-10, yAxis_3 +10);
    ui->widget_plot_2->yAxis->setLabel(regNumList[regNum_plot2[0]]);

    ui->widget_plot_2->yAxis2->setRange(-10, yAxis_4 +10);
    ui->widget_plot_2->yAxis2->setLabel(regNumList[regNum_plot2[1]]);

    for(int i = 0; i < 2; i++){
        xPlot.clear();
        yPlot.clear();
        if (!regDataArray[regNum_plot2[i]].flagFullBuffer) lengthBuffer = regDataArray[regNum_plot2[i]].counterRegPlot;

        for(int j = 0; j < lengthBuffer; j++){ // заполняем массив на выдачу плоттера
            yPlot.push_back(double(regDataArray[regNum_plot2[i]].regValueScaledArr[j]));
            int deltaTime = int(regDataArray[regNum_plot2[i]].regTimeArr[j]) - (startTimeStamp + windowWide);
            if(deltaTime > 0) startTimeStamp = startTimeStamp + deltaTime;

            xPlot.push_back(double(regDataArray[regNum_plot2[i]].regTimeArr[j]));
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
