#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
  //  resize(QDesktopWidget().availableGeometry(this).size() * 0.7); // не работает изменение окна
    QVector<QString> tempVector(IREG_INV_ALL_END_REGISTERS);
    for(int i=0; i<IREG_INV_ALL_END_REGISTERS; i++) {
        QString value = "unknown";
        tempVector[i] = value;
    }
    regNumList = tempVector;  // инициализация списка регистров
    adapterAnswerList.clear(); // ошибки адаптера и дежурные ответы
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(readStream()));

//    timerRegDisplay = new QTimer;
//    connect(timer, SIGNAL(timeout()), this, SLOT(regDisplay()));

    ui->setupUi(this);

    ui->tabWidget_registerWidget->setCurrentIndex(0); // окно при загрузке с настройками интерфейса

    ui->lineEdit_volumeTextRead->setText("50");
    ui->textEdit_dataRead->document()->setMaximumBlockCount(50); //
    ui->textEdit_adapterAnswer->document()->setMaximumBlockCount(100);

    qDebug() << "объявляем serial";
    serial = new QSerialPort;

    initTabCan(); // установить активность кнопок
    createRegistersTable();
}

MainWindow::~MainWindow()
{
    serial->close();

    delete ui;
//    delete timer;
//    delete serial;
}
