#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tab_plot.h"

tab_plot::tab_plot()
{

}

void MainWindow::addGraph(){
    ui->widget_plot->xAxis->setRange(0, PLOT_MAX_SIZE_ARR);
    ui->widget_plot->yAxis->setRange(0, 200); // попробуем вывести частоту
    ui->widget_plot->addGraph();
}

void MainWindow::addPointToGraph(QVector<double> x,  QVector<double> y){
    ui->widget_plot->graph(0)->setData(x, y, true);
    ui->widget_plot->replot();

}
