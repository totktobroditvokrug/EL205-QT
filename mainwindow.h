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

#include "configfile.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

//signals:
//    void sendUconv(int uConv, int uConvMax);  //  будем посылать для вектора значение напряжение
//    void sendIconv(int iA, int iB, int iC, int iMax, int phA, int phB, int phC);  //  будем посылать для вектора значения тока

private slots:

    QString readCurrentDate(); //  работа с текущим временем для плоттера

    // настройка адаптера
    void initTabCan(); // уставки по умолчанию, активация кнопок
    void on_pushButton_searchListPort_clicked();
    void on_pushButton_connect_clicked();
    void on_pushButton_disconnect_clicked();

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

    void getFreqInv();
    QString getRegisterInv(int regNum, qint16 valueInt);
    void setRegistersCombobox();

    QString glueString(quint16 data, quint8 registerInv);
    QString glueAdapterHeader();

    QString scaledValue(qint16 value, qint16 scale, qint16 max);

    void on_pushButton_sendCommand_clicked();

    void on_pushButton_startInv_panel_clicked();
    void on_pushButton_stopInv_panel_clicked();
    void on_pushButton_alarmInv_panel_clicked();

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

    // работа с графиками
    void checkVector();

    void addGraph();
    void addPointToGraph();
    void on_pushButton_holdPlot_clicked();
    void on_comboBox_plot1_currentIndexChanged(int index);
    void on_comboBox_plot2_currentIndexChanged(int index);
    void on_comboBox_plot3_currentIndexChanged(int index);
    void on_comboBox_plot4_currentIndexChanged(int index);
    void on_horizontalSlider_max_axis_1_valueChanged(int value);
    void on_horizontalSlider_max_axis_2_valueChanged(int value);
    void on_horizontalSlider_max_axis_3_valueChanged(int value);
    void on_horizontalSlider_max_axis_4_valueChanged(int value);
    void on_lineEdit_yAxis_1_editingFinished();
    void on_lineEdit_yAxis_2_editingFinished();
    void on_lineEdit_yAxis_3_editingFinished();
    void on_lineEdit_yAxis_4_editingFinished();
    void init_scale();
    void on_horizontalSlider_scalePlot_valueChanged(int value);
    void on_lineEdit_scalePlot_editingFinished();

    // работа со статусами инвертора
    void initStatus();
    void checkStatus();
    void checkInvStatus();
    void checkInvStatus_2();
    void checkInvStatus_3();
    void checkInvStatus_flt();
    void checkInvStatus_5();

    void on_lineEdit_freqPlot_editingFinished();


    void on_radioButton_directRotation_clicked(bool checked);

    void on_radioButton_reversRotation_clicked(bool checked);

    void on_radioButton_async_clicked(bool checked);

    void on_radioButton_vent_clicked(bool checked);

    void on_radioButton_PWM_SIN_clicked(bool checked);

    void on_radioButton_PWM_OVER1_clicked(bool checked);

    void on_radioButton_PWM_OVER2_clicked(bool checked);

    void on_checkBox_harmonicAnalysis_clicked(bool checked);

    void on_pushButton_rstFlt_clicked();

    void on_radioButton_resDempAngle_clicked(bool checked);

    void on_radioButton_resDempTorque_clicked(bool checked);

    void on_radioButton_resDempPower_clicked(bool checked);

    void on_radioButton_termosensorSingle_clicked(bool checked);

    void on_radioButton_termosensorCritic_clicked(bool checked);

    void on_radioButton_termosensorAll_clicked(bool checked);

    void on_checkBox_powerMonitor_clicked(bool checked);

    void on_checkBox_currSensOnFilter_clicked(bool checked);

    void on_checkBox_rationingTime_clicked(bool checked);

    void on_checkBox_enableZeroUdc_clicked(bool checked);

    void on_checkBox_useOutInductance_clicked(bool checked);

    void on_checkBox_powerIsRMS_clicked(bool checked);

    void on_checkBox_pmmOn_clicked(bool checked);

    void on_radioButton_expLegacy_clicked(bool checked);

    void on_radioButton_exp485_clicked(bool checked);

    void on_radioButton_expSPI_clicked(bool checked);

    void on_radioButton_expOthers_clicked(bool checked);

    void on_radioButton_trac_clicked(bool checked);

    void on_radioButton_subpump_clicked(bool checked);

    void on_comboBox_canFreqInv_currentIndexChanged(int index);

    void on_checkBox_allowCAN_freqChanges_clicked(bool checked);

    void on_checkBox_extOut_0_clicked(bool checked);

    void on_checkBox_extOut_1_clicked(bool checked);

    void on_checkBox_extOut_2_clicked(bool checked);

    void on_radioButton_baseConverter_clicked(bool checked);

    void on_radioButton_ISING_RSING_clicked(bool checked);

    void on_radioButton_IQUAD_RDOUB_2_clicked(bool checked);

    void on_radioButton_ISING_RDOUB_clicked(bool checked);

    void on_radioButton_IDOUB_RSING_clicked(bool checked);

    void on_radioButton_IDOUB_RDOUB_clicked(bool checked);

    void on_radioButton_ITRIP_RSING_clicked(bool checked);

    void on_radioButton_ITRIP_RDOUB_clicked(bool checked);

    void on_radioButton_IQUAD_RSING_clicked(bool checked);

    void on_radioButton_IQUAD_RDOUB_clicked(bool checked);

    void on_radioButton_IQUAD_RSING_2_clicked(bool checked);

    void on_pushButton_flashErrClr_clicked();

    void on_pushButton_flashWrAll_clicked();

//    void on_listWidget_portInfo_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidget_portInfo_itemClicked(QListWidgetItem *item);

//    void on_horizontalSlider_sliderMoved(int position);

    // работа с таблицей UIF

    void addGraph_UF();
    void addPointUF();
    void checkUF();

    void on_table_UF_cellDoubleClicked(int row, int column);

    void on_table_UF_cellChanged(int row, int column);

    void on_lineEdit_f_nom_editingFinished();

    void on_lineEdit_f_err_editingFinished();

    void on_lineEdit_u_nom_editingFinished();

    void on_lineEdit_ku_start_editingFinished();

    void on_lineEdit_i_nom_editingFinished();

    void on_lineEdit_ki_start_editingFinished();

    void on_checkBox_pwmControl_clicked();

    void on_checkBox_paramsOutChange_clicked();

private:
    Ui::MainWindow *ui;

    class ConfigFile *cfg;

    QSerialPort *serial;
    QTimer *timer;  // таймер опроса CAN адаптера
    QTimer *timerPlotter; // таймер вывода регистров на дисплей

    QTimer *timerPlotterUF; // таймер вывода графика UIF

    QTimer *timerDate;  // таймер текущей даты
    int numberOfRestartsStartTime;  // количество перезапусков при переполнении метки времени адаптера

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

    // осциллограммы
    QVector<double> xPlot, yPlot;
    double startTimeStamp;  // первое пришедшее значение метки времени
    bool plot_1_isBusy;
    bool plot_2_isBusy;

    QStringList testListRegister;

    // статусы инвертора
    quint16 IregInvStatus;
    quint16 IregInvStatus_2;
    quint16 IregInvStatus_3;
    quint16 IregInvStatus_flt;
    quint16 IregInvStatus_5;
};
#endif // MAINWINDOW_H
