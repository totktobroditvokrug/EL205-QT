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
    ui->widget_plot_1->yAxis->setTickLabelColor(Qt::blue);

    ui->widget_plot_1->yAxis->setLabel("label1");

    ui->widget_plot_1->yAxis2->setVisible(true);
    ui->widget_plot_1->yAxis2->setTickLabels(true);
    ui->widget_plot_1->graph(1)->setPen(QPen(Qt::green));
    ui->widget_plot_1->yAxis2->setTickLabelColor(Qt::green);
    ui->widget_plot_1->yAxis2->setLabel("label2");

    ui->widget_plot_2->addGraph();
    ui->widget_plot_2->addGraph(ui->widget_plot_2->xAxis, ui->widget_plot_2->yAxis2);
    ui->widget_plot_2->graph(0)->setPen(QPen(Qt::red));

    ui->widget_plot_2->yAxis->setTickLabelColor(Qt::red);
    ui->widget_plot_2->yAxis->setLabel("label1");

    ui->widget_plot_2->yAxis2->setVisible(true);
    ui->widget_plot_2->yAxis2->setTickLabels(true);
    ui->widget_plot_2->graph(1)->setPen(QPen(Qt::darkYellow));
    ui->widget_plot_2->yAxis2->setTickLabelColor(Qt::darkYellow);
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
    ui->lineEdit_yAxis_4->setStyleSheet("color: #808000");
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

    int windowWide = ui->lineEdit_scalePlot->text().toInt(); // размер экрана плоттера


    //----------------- первый график с двумя осями
    ui->widget_plot_1->yAxis->setRange(0, yAxis_1);
    ui->widget_plot_1->yAxis->setLabel(regNumList[regNum_plot1[0]]);

    ui->widget_plot_1->yAxis2->setRange(0, yAxis_2);
    ui->widget_plot_1->yAxis2->setLabel(regNumList[regNum_plot1[1]]);

    for(int i = 0; i < 2; i++){
        int regCounter = regDataArray[regNum_plot1[i]].counterRegPlot; // положение счетчика буфера парсинга данных
        if (regCounter > 0) regCounter--;
        int deltaTime = int(regDataArray[regNum_plot1[i]].regTimeArr[regCounter]) - startTimeStamp;
        // qDebug() << "шкала времени. regCounter=" << regCounter << " deltaTime=" << deltaTime;
        if(deltaTime > 0) startTimeStamp = startTimeStamp + deltaTime;

        ui->widget_plot_1->xAxis->setRange(startTimeStamp - windowWide, startTimeStamp);

        ui->widget_plot_1->graph(i)->setData(regDataArray[regNum_plot1[i]].regTimeArr, regDataArray[regNum_plot1[i]].regValueScaledArr, false);
    }
    ui->widget_plot_1->replot();

    //------------ второй график с двумя осями
    ui->widget_plot_2->yAxis->setRange(0, yAxis_3);
    ui->widget_plot_2->yAxis->setLabel(sampleNumList[sampleNum_plot2[0]]);

    ui->widget_plot_2->yAxis2->setRange(0, yAxis_4);
    ui->widget_plot_2->yAxis2->setLabel(sampleNumList[sampleNum_plot2[1]]);

    for(int i = 0; i < 2; i++){
        int sampleCounter = sampleDataArray[regNum_plot1[i]].counterSamplePlot; // положение счетчика буфера парсинга измерений
        if (sampleCounter > 0) sampleCounter--;
        int deltaTime = int(sampleDataArray[sampleNum_plot2[i]].sampleTimeArr[sampleCounter]) - startTimeStamp;
        if(deltaTime > 0) startTimeStamp = startTimeStamp + deltaTime;

        ui->widget_plot_2->xAxis->setRange(startTimeStamp - windowWide, startTimeStamp);
        ui->widget_plot_2->graph(i)->setData(sampleDataArray[sampleNum_plot2[i]].sampleTimeArr, sampleDataArray[sampleNum_plot2[i]].sampleValueScaledArr, false);
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

void MainWindow::on_comboBox_plot1_currentIndexChanged(int index)
{
    QString maxScale = "99";
    if(regDataArray[index].flagReg & IREGF_MAXVAL_PRESENT){
        maxScale = QString::number(regDataArray[index].maxValue.Reg16, 10);
    }
    else maxScale = QString::number(regDataArray[index].max.Reg16, 10);
    if(maxScale.toInt() <= 0) maxScale = "999";
    ui->lineEdit_yAxis_1->setText(maxScale);
}

void MainWindow::on_comboBox_plot2_currentIndexChanged(int index)
{
    QString maxScale = "99";
    if(regDataArray[index].flagReg & IREGF_MAXVAL_PRESENT){
        maxScale = QString::number(regDataArray[index].maxValue.Reg16, 10);
    }
    else maxScale = QString::number(regDataArray[index].max.Reg16, 10);
    if(maxScale.toInt() <= 0) maxScale = "999";
    ui->lineEdit_yAxis_2->setText(maxScale);
}

void MainWindow::on_comboBox_plot3_currentIndexChanged(int index)
{
    QString maxScale = QString::number(sampleDataArray[index + SampleCanIdClass::CAN_START_SAMPLE_ID].maxValue.Reg16, 10);
    if(maxScale.toInt() <= 0 ) maxScale = "999";
    ui->lineEdit_yAxis_3->setText(maxScale);
}

void MainWindow::on_comboBox_plot4_currentIndexChanged(int index)
{
    QString maxScale = QString::number(sampleDataArray[index + SampleCanIdClass::CAN_START_SAMPLE_ID].maxValue.Reg16, 10);
    if(maxScale.toInt() <= 0) maxScale = "999";
    ui->lineEdit_yAxis_4->setText(maxScale);
}

