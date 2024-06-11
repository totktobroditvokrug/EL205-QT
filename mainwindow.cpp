#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
// #include <QDateTime>
#include "plot/qcustomplot.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    // объявление списка регистров и измерений с размерами, соответствующими enum
    regNumList = QVector<QString>(RegnumClass::IREG_INV_ALL_END_REGISTERS);
    sampleNumList = QVector<QString>(SampleCanIdClass::CAN_END_SAMPLE_ID);

    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(readStream()));

    ui->setupUi(this);

    ui->tabWidget_registerWidget->setCurrentIndex(0); // окно при загрузке с настройками интерфейса

    ui->lineEdit_volumeTextRead->setText("50");
    ui->textEdit_dataRead->document()->setMaximumBlockCount(50); //

  //  ui->lineEdit_availableByte->setEnabled(false);

    serial = new QSerialPort;  // объявляем serial

    workDirPath = "C:/"; // инициализация рабочей директории
    ui->lineEdit_workDir->setText(workDirPath);

    ui->radioButton_registers->setChecked(true);
    ui->listWidget_sampleNum->hide();
    ui->textEdit_selectedSampleNum->hide();

    ui->label_selectAlias->setStyleSheet(StyleHelper::getRegistersStyle());
    ui->radioButton_registers->setStyleSheet(StyleHelper::getRadioButtonRegistersStyle());
    ui->label_selectAlias->setText("Selected registers");

    ui->radioButton_registers->isChecked();
    genRegFromEnum(); // заранее подгружаем список всех регистров инвертора
    genSampleFromEnum(); // заранее подгружаем список измерений инвертора

    initTabCan(); // установить активность кнопок
    createRegistersTable();
    createSamplesTable();
    initComboBoxRegister(); // три кастомных регистра в заголовке таблицы значений
    ui->checkBox_lossConnection->setChecked(true); // по умолчанию останавливаем процесс при потере связи
    on_pushButton_searchListPort_clicked(); // заранее загрузить доступные порты


    //работа с осциллограммами
    xPlot = QVector<double>(PLOT_MAX_SIZE_ARR);
    yPlot = QVector<double>(PLOT_MAX_SIZE_ARR);
    startTimeStamp = 0; // пока ноль, пытаемся присвоить пришедшее значение из адаптера
    addGraph();  // 2 графика с двумя осями

    plot_1_isBusy = false;
    plot_2_isBusy = false;


    // работа со статусом инвертора
    initStatus();

//    currentTime = QDateTime::currentDateTime().toString("dd.MM.yyyy");
    ui->statusbar->showMessage("ver. 06-06-2024");

}

MainWindow::~MainWindow()
{
    serial->close(); // освободить порт при закрытии программы
    delete ui;
}

