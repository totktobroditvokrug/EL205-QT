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
}

MainWindow::~MainWindow()
{
    serial->close();

    delete ui;
}


//------------------Dывод значений регистров на дисплей-----------------
void MainWindow::regDisplay() //
{
    ui->textEdit_regDisplay->clear(); // временная заглушка для вывода данных. Заменить на генерацию item
    for(int i=0; i<IREG_INV_ALL_END_REGISTERS; i++){
        if(regDataArray[i].displayed){
            ui->textEdit_regDisplay->append(regDataArray[i].displayString);
        }
    }
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionOpen_register_map_triggered()
{
    qDebug() << "открываем файл с картой регистров";
    QFileDialog dialogOpen;
    QString fileName = dialogOpen.getOpenFileName(nullptr, "Выберите файл", "/", "Текстовый файл (*.txt)");
    qDebug() << "Выбранный файл: " << fileName;
    QFile file(fileName);
    if(file.open(QIODevice::ReadWrite | QIODevice::Text)){
        QTextStream stream(&file);
//        s << (RegnumClass::regnumList()).join("\n");
        QString str;

        stream >> str;
        //ui->textEdit_listRegnum->clear();
        //ui->textEdit_listRegnum->setText(str);
    }
    else ui->statusbar->showMessage("Ошибка: error opening output file");
}

void MainWindow::on_actionCreate_register_map_triggered()
{

}
