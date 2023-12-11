#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QListWidgetItem>
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
    void on_pushButton_searchListPort_clicked();

    void on_pushButton_connect_clicked();

    void on_pushButton_disconnect_clicked();

    void on_pushButton_sendMessage_clicked();

    void on_pushButton_startRead_clicked();

    void on_pushButton_readOnce_clicked();

    void on_pushButton_stopRead_clicked();

    void on_pushButton_clear_clicked();

    void on_comboBox_canFreq_currentIndexChanged(int index);

    void writeSerialPort(QString dataWriteString);
    QByteArray readSerialPort();
    void readStream();

    void on_pushButton_setConfigAdapter_clicked();

    void on_comboBox_readAllCan_currentIndexChanged(int index);

    void on_lineEdit_volumeTextRead_editingFinished();

    void on_lineEdit_freqSampl_editingFinished();

    void on_actionQuit_triggered();

    void on_actionOpen_register_map_triggered();

    void on_actionCreate_register_map_triggered();

    void on_pushButton_genRegFromEnum_clicked();

    void on_pushButton_saveRegToFile_clicked();

    void on_pushButton_readRegFromFile_clicked();

    void on_listWidget_regNum_itemClicked(QListWidgetItem *item);

    void on_pushButton_loadToDisplay_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QTimer *timer;
    QVector<QString> regNumList;
    registerFields regDataArray[IREG_INV_ALL_END_REGISTERS];
//    QString regDataArray[10];
};
#endif // MAINWINDOW_H
