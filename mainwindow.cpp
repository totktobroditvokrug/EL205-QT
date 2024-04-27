#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>


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
    adapterAnswerList.clear(); // ошибки адаптера и дежурные ответы
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(readStream()));

//    timerRegDisplay = new QTimer;
//    connect(timer, SIGNAL(timeout()), this, SLOT(regDisplay()));


//    QByteArray standartArrayID = QByteArray::fromHex("0000"); // ID сообщения 4 байта
//    QByteArray standartArrayDATA = QByteArray::fromHex("00000000");

//    canByID.insert(standartArrayID, standartArrayDATA);

    ui->setupUi(this);

    ui->tabWidget_registerWidget->setCurrentIndex(0); // окно при загрузке с настройками интерфейса

    ui->lineEdit_volumeTextRead->setText("50");
    ui->textEdit_dataRead->document()->setMaximumBlockCount(50); //
    ui->textEdit_adapterAnswer->document()->setMaximumBlockCount(100);


    serial = new QSerialPort;  // объявляем serial

    on_pushButton_genRegFromEnum_clicked(); // заранее подгружаем список всех регистров инвертора
    initTabCan(); // установить активность кнопок
    createRegistersTable();
    initComboBoxRegister(); // три кастомных регистра в заголовке таблицы значений
    ui->checkBox_lossConnection->setChecked(true); // по умолчанию останавливаем процесс при потере связи
    on_pushButton_searchListPort_clicked(); // заранее загрузить доступные порты
}

MainWindow::~MainWindow()
{
    serial->close(); // освободить порт при закрытии программы
    delete ui;
}

