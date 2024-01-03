#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QVector<QString> tempVector(IREG_INV_ALL_END_REGISTERS);
    for(int i=0; i<IREG_INV_ALL_END_REGISTERS; i++) {
        QString value = "unknown";
        tempVector[i] = value;
    }
    regNumList = tempVector;  // инициализация списка регистров

    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(readStream()));

    timerRegDisplay = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(regDisplay()));

    ui->setupUi(this);

    ui->lineEdit_volumeTextRead->setText("50");
    ui->textEdit_dataRead->document()->setMaximumBlockCount(50); //
    serial = new QSerialPort;

    initTabCan(); // установить активность кнопок
    createRegistersTable();
}

MainWindow::~MainWindow()
{
    serial->close();

    delete ui;
}


