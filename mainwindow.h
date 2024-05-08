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

#include <QHash>

#include "stylehelper.h"
#include "adapter_commands.h"
#include "iface.h"

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
//    void on_pushButton_sendMessage_clicked();

    // чтение данных из CAN
    void on_pushButton_startRead_clicked(); // опрос в цикле таймера
    void on_pushButton_readOnce_clicked();  // единичный опрос
    void on_pushButton_stopRead_clicked();
    void on_pushButton_clear_clicked();
    void on_comboBox_canFreq_currentIndexChanged(int index); // частота опроса CAN шины
    void on_comboBox_readAllCan_currentIndexChanged(int index); // выбор фильтра CAN шины
//    void writeConfigAdapter(QString configString); //
    void on_pushButton_setConfigAdapter_clicked(); // конфигурация адаптера по комбобоксам
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
    void genRegFromEnum(); // сгенерировать из enum
    void on_pushButton_saveRegToFile_clicked();
    void on_pushButton_readRegFromFile_clicked();
    void on_listWidget_regNum_itemClicked(QListWidgetItem *item);
    void on_pushButton_selectAll_clicked(); // установить все check списка
    void on_pushButton_resetAll_clicked(); // снять все check списка
    void addItemFromlistwidget(QListWidgetItem *item, quint8 index, QString regName);
    void deleteItemFromlistwidget(QListWidgetItem *item, quint8 index);
    void initComboBoxRegister();

    // работа со списком измерений
    void genSampleFromEnum();
    void createSamplesTable();
    void on_listWidget_sampleNum_itemClicked(QListWidgetItem *item);
    void addSampleFromlistwidget(QListWidgetItem *item, int index, QString regName);
    void deleteSampleFromlistwidget(QListWidgetItem *item, int index);
    void addRowSamplesTable(int index, QString sampleName);
    void deleteRowSamplesTable(int index);
    void sampleDisplayTable();
    void displayHashID();

    // работа с таблицей регистров
    void createRegistersTable();
    void addRowRegistersTable(int index, QString regName);
    void deleteRowRegistersTable(int index);
    void regDisplayTable();
    void createTableFromFile();
    void addRowTableFromFile(QString lineFromFile);
    void checkInvertorStatus();
    void getFreqInv();
    QString getRegisterInv(int regNum, qint16 valueInt);
    void setRegistersCombobox();

    QString glueString(quint16 data, quint8 registerInv);
    QString glueAdapterHeader();

    QString scaledValue(qint16 value, qint16 scale, qint16 max);

    void on_pushButton_sendCommand_clicked();

    void on_pushButton_startInv_clicked();
    void on_pushButton_stopInv_clicked();
    void on_pushButton_alarmInv_clicked();

    void on_tableRegister_cellChanged(int row, int column);
    void on_tableRegister_cellDoubleClicked(int row, int column);

    // работа с конфигурацией регистров из файла настроек
    void on_pushButton_saveTable_clicked();
    void on_pushButton_loadTable_clicked();
    void on_pushButton_checkRegistersFromFile_clicked();

    void checkValueRegister(int i, int value);

    void on_pushButton_setRegistersFromFile_clicked();

    void on_horizontalSlider_valueChanged(int value);
    void setSelectedRegisterSlider(int regNum);

    void on_lineEdit_registerValue_1_selectionChanged();

    void on_lineEdit_registerValue_2_selectionChanged();

    void on_lineEdit_registerValue_3_selectionChanged();

    void on_horizontalSlider_sliderReleased();

    void on_pushButton_showPanel_clicked();

    void on_pushButton_hidePanel_clicked();


    void on_pushButton_workDir_clicked();

    void on_radioButton_registers_clicked(bool checked);

    void on_radioButton_samples_clicked(bool checked);



    void on_pushButton_genFromEnum_clicked();

private:
    Ui::MainWindow *ui;

    QSerialPort *serial;
    QTimer *timer;  // таймер опроса CAN адаптера
 //   QTimer *timerRegDisplay; // таймер вывода регистров на дисплей

    QString workDirPath; //
    QVector<QString> regNumList; // формирование списка регистров
    QVector<QString> sampleNumList; // формирование списка регистров

    registerFields regDataArray[RegnumClass::IREG_INV_ALL_END_REGISTERS]; // все данные по регистрам
    samplesFields sampleDataArray[SampleCanIdClass::CAN_END_SAMPLE_ID]; // все данные по списку измерений

    int emptyBufferCounter; // количество попыток связи с can
    int selectedComboBox;

    QHash<quint16, QByteArray> canByIdStandart;  // хэш таблица стандартных ID (добавить архив)
    QHash<quint32, QByteArray> canByIdExtended;  // хэш таблица расширенных ID (добавить архив)

    static QString currentTime;
};
#endif // MAINWINDOW_H
