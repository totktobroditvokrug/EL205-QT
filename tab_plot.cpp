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

    ui->lineEdit_scalePlot->setText(QString::number(MAX_PLOT_SCALE/10, 10));
    ui->horizontalSlider_scalePlot->setMaximum(MAX_PLOT_SCALE);
    ui->horizontalSlider_scalePlot->setMinimum(0);
    ui->horizontalSlider_scalePlot->setValue(MAX_PLOT_SCALE/10);
}

void MainWindow::addPointToGraph(){

    quint32 time_stamp_32 = regDataArray[RegnumClass::IREG_FREQ].time_stamp_32;
    if(quint32(startTimeStamp) == 0) {
        startTimeStamp = double(time_stamp_32); // стартовый отрезок времени с каждым запуском программы
        // qDebug() << "стартуем с отметки " << quint32(startTimeStamp);
        QTimer::singleShot(5000, this, SLOT(init_scale()));
        return;
    }

    if(ui->tabWidget_registerWidget->currentIndex() != 7){
       // qDebug() << "графики не рисуем";
       return; // если виджет неактивен, графики не рисуем
    }

    if(plot_1_isBusy || plot_2_isBusy){ // проверка, что программа не запустится, если предыдущая не закончила
        ui->lineEdit_freqPlot->setStyleSheet("color: red");
        return;
    }
    else ui->lineEdit_freqPlot->setStyleSheet("color: green");

    plot_1_isBusy = true;
    plot_2_isBusy = true;


    // по выбранным в комбобоксе регистрам смотрим их значения
    int regNum_1 = ui->comboBox_plot1->currentIndex();
    int regNum_2 = ui->comboBox_plot2->currentIndex();
    int regNum_plot1[2] = {regNum_1, regNum_2};

    int yAxis_1 = ui->horizontalSlider_max_axis_1->value(); // ui->lineEdit_yAxis_1->text().toInt();
    int yAxis_2 = ui->horizontalSlider_max_axis_2->value(); // ui->lineEdit_yAxis_2->text().toInt();
    int yAxis_1_min = ui->lineEdit_yAxis_1_min->text().toInt();
    int yAxis_2_min = ui->lineEdit_yAxis_2_min->text().toInt();

    int sampleNum_1 = ui->comboBox_plot3->currentIndex() + SampleCanIdClass::CAN_START_SAMPLE_ID;
    int sampleNum_2 = ui->comboBox_plot4->currentIndex() + SampleCanIdClass::CAN_START_SAMPLE_ID;
    int sampleNum_plot2[2] = {sampleNum_1, sampleNum_2};

    int yAxis_3 = ui->horizontalSlider_max_axis_3->value(); // ui->lineEdit_yAxis_3->text().toInt();
    int yAxis_4 = ui->horizontalSlider_max_axis_4->value(); // ui->lineEdit_yAxis_4->text().toInt();
    int yAxis_3_min = ui->lineEdit_yAxis_3_min->text().toInt();
    int yAxis_4_min = ui->lineEdit_yAxis_4_min->text().toInt();

    int windowWide = ui->horizontalSlider_scalePlot->value(); // ui->lineEdit_scalePlot->text().toInt(); // размер экрана плоттера


    //----------------- первый график с двумя осями
    ui->widget_plot_1->yAxis->setRange(yAxis_1_min, yAxis_1);
    ui->widget_plot_1->yAxis->setLabel(regNumList[regNum_plot1[0]]);

    ui->widget_plot_1->yAxis2->setRange(yAxis_2_min, yAxis_2);
    ui->widget_plot_1->yAxis2->setLabel(regNumList[regNum_plot1[1]]);

    testListRegister.clear();

    for(int i = 0; i < 2; i++){ // вывод двух графиков на первом плоттере
        int regCounter = regDataArray[regNum_plot1[i]].counterRegPlot; // положение счетчика буфера парсинга данных
        if (regCounter > 0) regCounter--;
        double deltaTime = double(regDataArray[regNum_plot1[i]].regTimeArr[regCounter]) - startTimeStamp;
        // qDebug() << "шкала времени. regCounter=" << regCounter << " deltaTime=" << deltaTime;
        if(deltaTime > 0) startTimeStamp = startTimeStamp + deltaTime;

        ui->widget_plot_1->xAxis->setRange(startTimeStamp - windowWide, startTimeStamp);

        ui->widget_plot_1->graph(i)->setData(regDataArray[regNum_plot1[i]].regTimeArr, regDataArray[regNum_plot1[i]].regValueScaledArr, false);
    }
    ui->widget_plot_1->replot();
    // ui->widget_plot_1->update();
    plot_1_isBusy = false;

    //------------ второй график с двумя осями
    ui->widget_plot_2->yAxis->setRange(yAxis_3_min, yAxis_3);
    ui->widget_plot_2->yAxis->setLabel(sampleNumList[sampleNum_plot2[0]]);

    ui->widget_plot_2->yAxis2->setRange(yAxis_4_min, yAxis_4);
    ui->widget_plot_2->yAxis2->setLabel(sampleNumList[sampleNum_plot2[1]]);

    for(int i = 0; i < 2; i++){ // вывод двух графиков на втором плоттере
        int sampleCounter = sampleDataArray[regNum_plot1[i]].counterSamplePlot; // положение счетчика буфера парсинга измерений
        if (sampleCounter > 0) sampleCounter--;
        int deltaTime = int(sampleDataArray[sampleNum_plot2[i]].sampleTimeArr[sampleCounter]) - int(startTimeStamp); // goto поправил шкалу
        if(deltaTime > 0) startTimeStamp = startTimeStamp + deltaTime;

        ui->widget_plot_2->xAxis->setRange(startTimeStamp - windowWide, startTimeStamp);
        ui->widget_plot_2->graph(i)->setData(sampleDataArray[sampleNum_plot2[i]].sampleTimeArr, sampleDataArray[sampleNum_plot2[i]].sampleValueScaledArr, false);
    }

    ui->widget_plot_2->replot();
    // ui->widget_plot_2->update();
    plot_2_isBusy = false;
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
    int maxScale = 99;
    if((regDataArray[index].flagReg & IREGF_MAXVAL_PRESENT) && (regDataArray[index].flagReg & IREGF_SCALE_PRESENT)){
        qint16 scaleReg16 = regDataArray[index].scale.Reg16;
        qint16 maxValueReg16 = regDataArray[index].maxValue.Reg16;
        qint16 maxReg16 = regDataArray[index].max.Reg16;
        if ((scaleReg16 == 0) || (maxReg16 == 0) || (maxValueReg16 == 0)) maxScale = 0;
        else maxScale = maxReg16 / maxValueReg16 * scaleReg16;
    }
    else maxScale = regDataArray[index].max.Reg16;

    if(maxScale <= 0) maxScale = 9999;
    ui->lineEdit_yAxis_1->setText(QString::number(maxScale, 10));

    ui->horizontalSlider_max_axis_1->setMinimum(0);
    ui->horizontalSlider_max_axis_1->setMaximum(maxScale);
    ui->horizontalSlider_max_axis_1->setValue(maxScale);
}

void MainWindow::on_comboBox_plot2_currentIndexChanged(int index)
{
    int maxScale = 99;
    if((regDataArray[index].flagReg & IREGF_MAXVAL_PRESENT) && (regDataArray[index].flagReg & IREGF_SCALE_PRESENT)){
        qint16 scaleReg16 = regDataArray[index].scale.Reg16;
        qint16 maxValueReg16 = regDataArray[index].maxValue.Reg16;
        qint16 maxReg16 = regDataArray[index].max.Reg16;
        if ((scaleReg16 == 0) || (maxReg16 == 0) || (maxValueReg16 == 0)) maxScale = 0;
        else maxScale = maxReg16 / maxValueReg16 * scaleReg16;
    }
    else maxScale = regDataArray[index].max.Reg16;
    if(maxScale <= 0) maxScale = 9999;
    ui->lineEdit_yAxis_2->setText(QString::number(maxScale, 10));

    ui->horizontalSlider_max_axis_2->setMinimum(0);
    ui->horizontalSlider_max_axis_2->setMaximum(maxScale);
    ui->horizontalSlider_max_axis_2->setValue(maxScale);
}

void MainWindow::on_comboBox_plot3_currentIndexChanged(int index)
{
    int maxScale = sampleDataArray[index + SampleCanIdClass::CAN_START_SAMPLE_ID].scale.Reg16;
    if(maxScale <= 0 ) maxScale = 9999;
    ui->lineEdit_yAxis_3->setText( QString::number(maxScale, 10));

    ui->horizontalSlider_max_axis_3->setMinimum(0);
    ui->horizontalSlider_max_axis_3->setMaximum(maxScale);
    ui->horizontalSlider_max_axis_3->setValue(maxScale);
}

void MainWindow::on_comboBox_plot4_currentIndexChanged(int index)
{
    int maxScale = sampleDataArray[index + SampleCanIdClass::CAN_START_SAMPLE_ID].scale.Reg16;
    if(maxScale <= 0 ) maxScale = 9999;
    ui->lineEdit_yAxis_4->setText( QString::number(maxScale, 10));

    ui->horizontalSlider_max_axis_4->setMinimum(0);
    ui->horizontalSlider_max_axis_4->setMaximum(maxScale);
    ui->horizontalSlider_max_axis_4->setValue(maxScale);
}

void MainWindow::on_horizontalSlider_max_axis_1_valueChanged(int value)
{
    ui->lineEdit_yAxis_1->setText(QString::number(value, 10));
}

void MainWindow::on_horizontalSlider_max_axis_2_valueChanged(int value)
{
    ui->lineEdit_yAxis_2->setText(QString::number(value, 10));
}

void MainWindow::on_horizontalSlider_max_axis_3_valueChanged(int value)
{
    ui->lineEdit_yAxis_3->setText(QString::number(value, 10));
}

void MainWindow::on_horizontalSlider_max_axis_4_valueChanged(int value)
{
    ui->lineEdit_yAxis_4->setText(QString::number(value, 10));
}

void MainWindow::on_lineEdit_yAxis_1_editingFinished()
{
  ui->horizontalSlider_max_axis_1->setValue(ui->lineEdit_yAxis_1->text().toInt());
}

void MainWindow::on_lineEdit_yAxis_2_editingFinished()
{
  ui->horizontalSlider_max_axis_2->setValue(ui->lineEdit_yAxis_2->text().toInt());
}

void MainWindow::on_lineEdit_yAxis_3_editingFinished()
{
  ui->horizontalSlider_max_axis_3->setValue(ui->lineEdit_yAxis_3->text().toInt());
}

void MainWindow::on_lineEdit_yAxis_4_editingFinished()
{
  ui->horizontalSlider_max_axis_4->setValue(ui->lineEdit_yAxis_4->text().toInt());
}


void MainWindow::on_horizontalSlider_scalePlot_valueChanged(int value)
{
    ui->lineEdit_scalePlot->setText(QString::number(value, 10));
}

void MainWindow::on_lineEdit_scalePlot_editingFinished()
{
    ui->horizontalSlider_scalePlot->setValue(ui->lineEdit_scalePlot->text().toInt());
}


void MainWindow::init_scale(){
    ui->lineEdit_yAxis_1->setStyleSheet("color: blue");
    ui->lineEdit_yAxis_2->setStyleSheet("color: green");
    ui->lineEdit_yAxis_3->setStyleSheet("color: red");
    ui->lineEdit_yAxis_4->setStyleSheet("color: #808000");

    on_comboBox_plot1_currentIndexChanged(ui->comboBox_plot1->currentIndex());
    on_comboBox_plot2_currentIndexChanged(ui->comboBox_plot2->currentIndex());
    on_comboBox_plot3_currentIndexChanged(ui->comboBox_plot3->currentIndex());
    on_comboBox_plot4_currentIndexChanged(ui->comboBox_plot4->currentIndex());
}

void MainWindow::on_lineEdit_freqPlot_editingFinished() // изментиь находу частоту обновления осциллограмм
{
    timerPlotter->setInterval((ui->lineEdit_freqPlot->text().toInt()));
}
