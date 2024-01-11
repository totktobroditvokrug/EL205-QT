#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QTextStream>
#include <QMessageBox>

#include <QFileDialog>
#include <iostream>

#include <QList>
#include <QStringList>
#include <QListWidgetItem>

#include <QTimer>

#include <QTableWidget>
#include <QTableWidgetItem>

#include<QDebug>

#include "adapter_commands.h"
#include <iface.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // настройка адаптера
    void initTabCan(); // уставки по умолчанию, активация кнопок
    void on_pushButton_searchListPort_clicked();
    void on_pushButton_connect_clicked();
    void on_pushButton_disconnect_clicked();
    void on_pushButton_sendMessage_clicked();

    // чтение данных из CAN
    void on_pushButton_startRead_clicked(); // опрос в цикле таймера
    void on_pushButton_readOnce_clicked();  // единичный опрос
    void on_pushButton_stopRead_clicked();
    void on_pushButton_clear_clicked();
    void on_comboBox_canFreq_currentIndexChanged(int index); // частота опроса CAN шины
    void on_comboBox_readAllCan_currentIndexChanged(int index); // выбор фильтра CAN шины
//    void writeConfigAdapter(QString configString); //
//    void on_pushButton_setConfigAdapter_clicked(); // конфигурация адаптера по комбобоксам
    void init_setConfigAdapter(); // инициализация адаптера по умолчанию

    // получение данных от CAN адаптера
    void writeSerialPort(QString dataWriteString);
    QByteArray readSerialPort();
    void readStream(); // Чтение потока из serialport
    void on_lineEdit_volumeTextRead_editingFinished(); //максимальный размер блока для вывода выбранных посылок
    void on_lineEdit_freqSampl_editingFinished(); // частота опроса CAN-адаптера

    // элементы меню
    void on_actionQuit_triggered(); // выход из приложения

    void on_actionOpen_register_map_triggered(); // тестовые элементы, можно убрать
    void on_actionCreate_register_map_triggered();

    // загрузка списка регистров
    void on_pushButton_genRegFromEnum_clicked(); // сгенерировать из enum
    void on_pushButton_saveRegToFile_clicked();
    void on_pushButton_readRegFromFile_clicked();
    void on_listWidget_regNum_itemClicked(QListWidgetItem *item);
    void on_pushButton_selectAll_clicked(); // установить все check списка
    void on_pushButton_resetAll_clicked(); // снять все check списка
    void addItemFromlistwidget(QListWidgetItem *item, quint8 index, QString regName);
    void deleteItemFromlistwidget(QListWidgetItem *item, quint8 index);

    // вывод значений регистров в отдельной вкладке
    void regDisplay();

    // работа с таблицей регистров
    void createRegistersTable();
    void addRowRegistersTable(int index, QString regName);
    void deleteRowRegistersTable(int index);
    void regDisplayTable();





private:
    Ui::MainWindow *ui;

    QSerialPort *serial;
    QTimer *timer;  // таймер опроса CAN адаптера
 //   QTimer *timerRegDisplay; // таймер вывода регистров на дисплей
    QVector<QString> regNumList; // формирование списка регистров
    registerFields regDataArray[IREG_INV_ALL_END_REGISTERS]; // все данные по регистрам
};
#endif // MAINWINDOW_H
