#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
// #include <QDateTime>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    // объявление списка регистров и измерений с размерами, соответствующими enum
    regNumList = QVector<QString>(RegnumClass::IREG_INV_ALL_END_REGISTERS);
    sampleNumList = QVector<QString>(FcCanIdClass::CAN_END_SAMPLE_ID);

    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(readStream()));

    ui->setupUi(this);

    ui->tabWidget_registerWidget->setCurrentIndex(0); // окно при загрузке с настройками интерфейса

    ui->lineEdit_volumeTextRead->setText("50");
    ui->textEdit_dataRead->document()->setMaximumBlockCount(50); //

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
    on_pushButton_genRegFromEnum_clicked(); // заранее подгружаем список всех регистров инвертора

    initTabCan(); // установить активность кнопок
    createRegistersTable();
    createSamplesTable();
    initComboBoxRegister(); // три кастомных регистра в заголовке таблицы значений
    ui->checkBox_lossConnection->setChecked(true); // по умолчанию останавливаем процесс при потере связи
    on_pushButton_searchListPort_clicked(); // заранее загрузить доступные порты
//    currentTime = QDateTime::currentDateTime().toString("dd.MM.yyyy");
    ui->statusbar->showMessage("ver. 07-05-2024");

}

MainWindow::~MainWindow()
{
    serial->close(); // освободить порт при закрытии программы
    delete ui;
}

