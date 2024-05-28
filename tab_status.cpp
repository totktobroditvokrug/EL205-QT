#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "invertor.h"
#include "iface.h"
#include "stylehelper.h"

//---------- проверка статуса работы инвертора
void MainWindow::checkStatus()
{
    checkInvStatus();
    checkInvStatus_2();
    checkInvStatus_3();
    checkInvStatus_flt();
    checkInvStatus_5();
}


void MainWindow::checkInvStatus()
{
    if(!regDataArray[RegnumClass::IREG_INV_STATUS].flagNewData) return;
 //   if(ui->tabWidget_registerWidget->currentIndex() != 6) return; // если виджет неактивен, статусы не анализируем

    QString currentStatus = "--> ";
    QString valueStatus = "-";

    qint16 invStatus = regDataArray[RegnumClass::IREG_INV_STATUS].value.Reg16;
    if (IregInvStatus != NULL){
        if(IregInvStatus == invStatus) return;
    }
    IregInvStatus = invStatus;

    valueStatus = QString::number(invStatus, 2).rightJustified(16, '0');
    valueStatus.insert(4, " ").insert(9, " ").insert(14, " ");
    ui->lineEdit_status->setText(valueStatus);

//    if (invStatus & INV_STS_STARTED) currentStatus += ("Система запущена; \n");
//    else currentStatus += ("Система остановлена; \n");
    if (invStatus & INV_STS_STARTED){

      if (invStatus & INV_STS_TO_STOP_MODE) {
          ui->pushButton_startInv->setStyleSheet(StyleHelper::getWaitButtonStyle());
          ui->pushButton_startInv_panel->setStyleSheet(StyleHelper::getWaitButtonStyle());
          ui->pushButton_showPanel->setIcon(QIcon(":/images/wait_small.png"));
      }
      else{
         ui->pushButton_startInv->setStyleSheet(StyleHelper::getStartedButtonStyle());
         ui->pushButton_startInv_panel->setStyleSheet(StyleHelper::getStartedButtonStyle());
         ui->pushButton_showPanel->setIcon(QIcon(":/images/start_small.png"));
      }
      currentStatus += ("Система запущена; \n");
    }
    else{
      ui->pushButton_startInv->setStyleSheet(StyleHelper::getStartButtonStyle());
      ui->pushButton_startInv_panel->setStyleSheet(StyleHelper::getStartButtonStyle());
      ui->pushButton_showPanel->setIcon(QIcon(":/images/stop_small.png"));
      currentStatus += ("Система остановлена; \n");
    }

    if (invStatus & INV_STS_TO_STOP_MODE)  currentStatus += ("Режим остановки; \n");
    if (invStatus & INV_STS_WAIT_RECT_START) currentStatus += ("Ожидает запуска выпрямителя; \n");
    if (invStatus & INV_STS_STOPPED_REGISTERS) currentStatus += ("Остановлен по изменению важного регистра; \n");
//    if (invStatus & INV_STS_STOPPED_EXTERNAL)  currentStatus += ("Остановлен по команде извне (CAN, Modbus); \n");
    if (invStatus & INV_STS_STOPPED_EXTERNAL){
        ui->pushButton_stopInv->setStyleSheet(StyleHelper::getStopedButtonStyle());
        ui->pushButton_stopInv_panel->setStyleSheet(StyleHelper::getStopedButtonStyle());
        currentStatus += ("Остановлен по команде извне (CAN, Modbus); \n");
    }
    else{
        ui->pushButton_stopInv->setStyleSheet(StyleHelper::getStopButtonStyle());
        ui->pushButton_stopInv_panel->setStyleSheet(StyleHelper::getStopButtonStyle());
    }

    if (invStatus & INV_STS_WAIT_RECT_STOP) currentStatus += ("Ожидает останова выпрямителя; \n");
    if (invStatus & INV_STS_FAULT_STOPPED) currentStatus += ("Остановлен по причине FAULT; \n");
    if (invStatus & INV_STS_I_LIMIT) currentStatus += ("Токоограничение; \n");
    if (invStatus & INV_STS_ULOW) currentStatus += ("Недостаточно напряжения; \n");
//    if (invStatus & INV_STS_STOPPED_ALARM) currentStatus += ("Аварийный останов; \n");
    if (invStatus & INV_STS_STOPPED_ALARM){
        ui->pushButton_alarmInv->setStyleSheet(StyleHelper::getStopedButtonStyle());
        ui->pushButton_alarmInv_panel->setStyleSheet(StyleHelper::getStopedButtonStyle());
        currentStatus += ("Аварийный останов; \n");
    }
    else{
      ui->pushButton_alarmInv->setStyleSheet(StyleHelper::getStopButtonStyle());
      ui->pushButton_alarmInv_panel->setStyleSheet(StyleHelper::getStopButtonStyle());
    }

    if (invStatus & INV_STS_UD_LOW_FAULT) currentStatus += ("Остановлен по снижению напряжения на шине; \n");
    if (invStatus & INV_STS_TO_STOP_MODE) currentStatus += ("Режим плавной остановки инвертора; \n");
    if (invStatus & INV_STS_URECT_SHORTCIRCUIT) currentStatus += ("Остановлен по КЗ от выпрямителя; \n");

    currentStatus += "------ \n"; // агрегированные состояния

    if (invStatus & INV_STS_EXT_STP_BITS) currentStatus += ("Внешняя остановка; \n");
    if (invStatus & INV_STS_ERR_BITS) currentStatus += ("Остановлена запись архива; \n");

    ui->textEdit_status->setText(currentStatus);
    ui->textEdit_invertorStatus->setText(currentStatus); // панель в регистрах
}

void MainWindow::checkInvStatus_2()
{
    if(!regDataArray[RegnumClass::IREG_INV_STATUS2].flagNewData) return;
    QString currentStatus = "--> ";
    QString valueStatus = "-";

    qint16 invStatus = regDataArray[RegnumClass::IREG_INV_STATUS2].value.Reg16;
    if (IregInvStatus_2 != NULL){
        if(IregInvStatus_2 == invStatus) return;
    }
    IregInvStatus_2 = invStatus;

    valueStatus = QString::number(invStatus, 2).rightJustified(16, '0');
    valueStatus.insert(4, " ").insert(9, " ").insert(14, " ");
    ui->lineEdit_status_2->setText(valueStatus);

    if (invStatus & INV_STS2_FC_IT_ERR) currentStatus += ("Токовая защита инвертора; \n");
    if (invStatus & INV_STS2_AST_ERR)  currentStatus += ("Ошибка автонастройки; \n");
    if (invStatus & INV_STS2_I_LIMIT_FAST) currentStatus += ("Флаг срабатывания токоограничения - свободен; \n");
    if (invStatus & INV_STS2_CURRENT_OPT) currentStatus += ("Оптимизация по току; \n");
    if (invStatus & INV_STS2_POWER_OPT)  currentStatus += ("Оптимизация по мощности; \n");
    if (invStatus & INV_STS2_OPT_DONE) currentStatus += ("Оптимизация выполнена; \n");
    if (invStatus & INV_STS2_FLSH_RD_ERR) currentStatus += ("Ошибка чтения флеш уставок; \n");
    if (invStatus & INV_STS2_FLSH_WR_ERR) currentStatus += ("Ошибка записи флеш уставок; \n");
    if (invStatus & INV_STS2_DISCHARGE_ON) currentStatus += ("Включить разряд емкости (останов турбины для вентильного мотора); \n");
    if (invStatus & INV_STS2_DISCHARGE) currentStatus += ("Идет разряд емкости; \n");
    if (invStatus & INV_STS2_DISCHARGE_ERR) currentStatus += ("Ошибка разряда емкости (останова турбины вентильного мотора); \n");
    if (invStatus & INV_STS2_VC_ERR) currentStatus += ("Ошибка векторного алгоритма; \n");
    if (invStatus & INV_STS2_M_IFAST_ERR) currentStatus += ("Остановлен по КЗ от выпрямителя; \n");
    if (invStatus & INV_STS2_M_I2T_ERR) currentStatus += ("Быстрая токовая защита двигателя; \n");
    if (invStatus & INV_STS2_I_LIM_ERR) currentStatus += ("Защита ПЧ по токоограничению; \n");
    if (invStatus & INV_STS2_FLSH_VAL_ERR) currentStatus += ("Ошибка диапазона уставок флеш; \n");

    ui->textEdit_status_2->setText(currentStatus);
}

void MainWindow::checkInvStatus_3()
{
    if(!regDataArray[RegnumClass::IREG_INV_STATUS3].flagNewData) return;

    QString currentStatus = "--> ";
    QString valueStatus = "-";

    qint16 invStatus = regDataArray[RegnumClass::IREG_INV_STATUS3].value.Reg16;
    if (IregInvStatus_3 != NULL){
        if(IregInvStatus_3 == invStatus) return;
    }
    IregInvStatus_3 = invStatus;

    valueStatus = QString::number(invStatus, 2).rightJustified(16, '0');
    valueStatus.insert(4, " ").insert(9, " ").insert(14, " ");
    ui->lineEdit_status_3->setText(valueStatus);

    if (invStatus & INV_STS3_RIGHT_DIR){
      currentStatus += ("Правое направление вращения; \n");
      ui->radioButton_directRotation->setChecked(true);
    }
    else{
       currentStatus += ("Левое направление вращения; \n");
       ui->radioButton_reversRotation->setChecked(true);
    }
//    if (invStatus & INV_STS3_OVERPWM1)  currentStatus += ("ОверШИМ тип 1; \n");
//    if (invStatus & INV_STS3_OVERPWM2) currentStatus += ("ОверШИМ тип 2; \n");
    if (invStatus & INV_STS3_M_TYPE){
       currentStatus += ("Тип двигателя - вентильный; \n");
       ui->radioButton_vent->setChecked(true);
    }
    else{
      currentStatus += ("Тип двигателя - асинхронный; \n");
      ui->radioButton_async->setChecked(true);
    }
    if (invStatus & INV_STS3_PMM_DIS)  currentStatus += ("Запрет вентильного двигателя; \n");
//    if (invStatus & INV_STS3_RES_TYPE0) currentStatus += ("ОС для подавления резонанса TYPE0; \n");
//    if (invStatus & INV_STS3_RES_TYPE1) currentStatus += ("ОС для подавления резонанса TYPE1; \n");
    if (invStatus & INV_STS3_DT_SINGLE) currentStatus += ("Режим контроля связи с термодатчиками - хоть один; \n");
    else currentStatus += ("Режим контроля связи с термодатчиками - все; \n");
    if (invStatus & INV_STS3_DT_CRITIC) currentStatus += ("Критический температурный режим; \n");
    if (invStatus & INV_STS3_PW_ERR_ON) currentStatus += ("Включение мониторов питания; \n");
    else currentStatus += ("Выключение мониторов питания; \n");
    if (invStatus & INV_STS3_LX_USE) currentStatus += ("Использовать выходную индуктивность Lx; \n");
    else currentStatus += ("Не использовать выходную индуктивность Lx; \n");
    if (invStatus & INV_STS3_DT_SU) currentStatus += ("Датчик тока на выходе СУ (после фильтра); \n");
    else currentStatus += ("Датчик тока на выходе инвертора (до фильтра); \n");
    if (invStatus & INV_STS3_ZERO_UDROP) currentStatus += ("Включить разрешение сброса напряженя до нуля; \n");
    else currentStatus += ("Выключить разрешение сброса напряженя до нуля; \n");
    if (invStatus & INV_STS3_NORM_MV_FREF) currentStatus += ("Включен нормированный разгон - торможение; \n");
    else currentStatus += ("Выключен нормированный разгон - торможение; \n");
    if (invStatus & INV_STS3_HARM_CALC_OFF){
        currentStatus += ("Выключен гармонический анализ; \n");
        ui->checkBox_harmonicAnalysis->setChecked(false);
    }
    else{
      currentStatus += ("Включен гармонический анализ; \n");
      ui->checkBox_harmonicAnalysis->setChecked(true);
    }
    if (invStatus & INV_STS3_RMS_POWER) currentStatus += ("Мощность из действ. значения; \n");
    else currentStatus += ("Мощность из анализатора; \n");

    currentStatus += "------ \n"; // агрегированные состояния

    quint16 pwmType = invStatus & INV_STS3_PWM_TYPE;

    if(pwmType == STS3_SINPWM){
       currentStatus += ("ШИМ - SIN ; \n");
       ui->radioButton_PWM_SIN->setChecked(true);
    }

    if(pwmType == STS3_OVERPWM1){
       currentStatus += ("ШИМ - OverPWM1 ; \n");
       ui->radioButton_PWM_OVER1->setChecked(true);
    }

    if(pwmType == STS3_OVERPWM2){
       currentStatus += ("ШИМ - OverPWM2 ; \n");
       ui->radioButton_PWM_OVER2->setChecked(true);
    }

    if(pwmType == STS3_OVERPWM_RES) currentStatus += ("ШИМ - тип зарезервирован ; \n");

    quint16 resType = invStatus & INV_STS3_RES_TYPE;
    if(resType == STS3_RES_ANGLE) currentStatus += ("Подавление резонанса - по углу ; \n");
    if(resType == STS3_RES_TORQUE) currentStatus += ("Подавление резонанса - по моменту ; \n");
    if(resType == STS3_RES_POWER) currentStatus += ("Подавление резонанса - по мощности ; \n");
    if(resType == STS3_RES_RESERVED) currentStatus += ("Подавление резонанса - зарезервированное ; \n");

    ui->textEdit_status_3->setText(currentStatus);

    ui->groupBox_direction->setEnabled(true);
    ui->groupBox_motorType->setEnabled(true);
    ui->groupBox_PWM_Type->setEnabled(true);
    ui->checkBox_harmonicAnalysis->setEnabled(true);

}

void MainWindow::checkInvStatus_flt()
{
    if(!regDataArray[RegnumClass::IREG_INV_FAULT].flagNewData) return;
    QString currentStatus = "--> ";
    QString valueStatus = "-";

    qint16 invStatus = regDataArray[RegnumClass::IREG_INV_FAULT].value.Reg16;
    if (IregInvStatus_flt != NULL){
        if(IregInvStatus_flt == invStatus) return;
    }
    IregInvStatus_flt = invStatus;

    valueStatus = QString::number(invStatus, 2).rightJustified(16, '0');
    valueStatus.insert(4, " ").insert(9, " ").insert(14, " ");
    ui->lineEdit_status_flt->setText(valueStatus);

    if (invStatus & INV_FLT_IMAX) currentStatus += ("Предел тока инвертора; \n");
    if (invStatus & INV_FLT_IZ)  currentStatus += ("Ток утечки фаз инвертора; \n");
    if (invStatus & INV_FLT_PAR_RSLV_MON) currentStatus += ("Монитор параллельных ветвей инвертора, ресольвера; \n");
    if (invStatus & INV_FLT_CTR_MON) currentStatus += ("Монитор питания цифровой цепи контроллера; \n");
    if (invStatus & INV_FLT_CLK_MON)  currentStatus += ("Перегрев реактора, новый; \n");
    if (invStatus & INV_FLT_MB_MON) currentStatus += ("Монитор входного питания источника; \n");
    if (invStatus & INV_FLT_DR0) currentStatus += ("Аппаратная защита драйвера 0; \n");
    if (invStatus & INV_FLT_DR1) currentStatus += ("Аппаратная защита драйвера 1; \n");
    if (invStatus & INV_FLT_DR2) currentStatus += ("Аппаратная защита драйвера 2; \n");
    if (invStatus & INV_FLT_TEST) currentStatus += ("Тестовый режим инвертора; \n");
    if (invStatus & INV_FLT_TEMP_LINK) currentStatus += ("Ошибка связи с термодатчиками; \n");
    if (invStatus & INV_FLT_TEMP) currentStatus += ("Тепловая защита инвертора; \n");
    if (invStatus & INV_FLT_AIR_TEMP) currentStatus += ("Тепловая защита по воздуху; \n");
    if (invStatus & INV_FLT_ALARM_SW) currentStatus += ("Аварийная кнопка; \n");
    if (invStatus & INV_FLT_TTEMP) currentStatus += ("Тепловая защита выпрямителя; \n");
    if (invStatus & INV_FLT_REACT_ERR) currentStatus += ("Перегрев реактора, старый; \n");

    currentStatus += "------ \n"; // агрегированные состояния

    int flt = (invStatus & (~FLT_NOT_STOPPED_MASK)) & INV_FAULT_MASK; // маска на разрешенные фаулты
    if(flt != 0){
         currentStatus += ("Ошибка, вызывающая останов; \n");
         ui->pushButton_rstFlt->setStyleSheet("color: red");
    }
    else ui->pushButton_rstFlt->setStyleSheet("color: green");

 //   ui->pushButton_rstFlt->setEnabled(true);

    ui->textEdit_status_flt->setText(currentStatus);
}

void MainWindow::checkInvStatus_5()
{
    if(!regDataArray[RegnumClass::IREG_INV_STATUS5].flagNewData) return;

    QString currentStatus = "--> ";
    QString valueStatus = "-";

    qint16 invStatus = regDataArray[RegnumClass::IREG_INV_STATUS5].value.Reg16;
    if (IregInvStatus_5 != NULL){
        if(IregInvStatus_5 == invStatus) return;
    }
    IregInvStatus_5 = invStatus;

    valueStatus = QString::number(invStatus, 2).rightJustified(16, '0');
    valueStatus.insert(4, " ").insert(9, " ").insert(14, " ");
    ui->lineEdit_status_5->setText(valueStatus);

    if (invStatus & STS5_EXP_LEGACY) currentStatus += ("Конфигурация платы расширения - LEGACY; \n");
    if (invStatus & STS5_EXP_485) currentStatus += ("Конфигурация платы расширения - 485; \n");
    if (invStatus & STS5_EXP_SPI) currentStatus += ("Конфигурация платы расширения - SPI; \n");
    if (invStatus & STS5_EXP_OTHERS) currentStatus += ("Конфигурация платы расширения - OTHERS; \n");
    if (invStatus & STS5_FC_CONF0)  currentStatus += ("Бит конфигурации ПЧ - CONF0; \n");
    if (invStatus & STS5_FC_CONF1)  currentStatus += ("Бит конфигурации ПЧ - CONF1; \n");
    if (invStatus & STS5_FC_CONF2)  currentStatus += ("Бит конфигурации ПЧ - CONF2; \n");
    if (invStatus & STS5_FC_CONF3)  currentStatus += ("Бит конфигурации ПЧ - CONF3; \n");
    if (invStatus & STS5_FC_CONF4)  currentStatus += ("Бит конфигурации ПЧ - CONF4; \n");
    if (invStatus & STS5_FC_CONF5)  currentStatus += ("Бит конфигурации ПЧ - CONF5; \n");
    if (invStatus & STS5_FC_SUBPUMP) currentStatus += ("Тип ПЧ - SUBPUMP; \n");
    if (invStatus & STS5_FC_TRAC) currentStatus += ("Тип ПЧ - TRAC; \n");
    if (invStatus & STS5_CAN_BDRT_250) currentStatus += ("Частота CAN шины - 250; \n");
    else currentStatus += ("Частота CAN шины - 125; \n");
    if (invStatus & STS5_EXP_CONF_2b)  currentStatus += ("Маска регистра STS5 - EXP_CONF_2b; \n");
    if (invStatus & STS5_FC_CONF_6b) currentStatus += ("Маска регистра STS5 - FC_CONF_6b); \n");
    if (invStatus & STS5_FC_TYPE_2b) currentStatus += ("Маска регистра STS5 - FC_TYPE_2b; \n");

    ui->textEdit_status_5->setText(currentStatus);
}


void MainWindow::on_pushButton_startInv_panel_clicked()
{
    QString commandString = AddCRC((glueAdapterHeader() + glueString(changeHiLowBytes_uint(INV_CTRL_START), IREG_INV_CONTROL)), 2).toHex();
  //  ui->textEdit_commandCRC->append(commandString);
    writeSerialPort(commandString);
}

void MainWindow::on_pushButton_stopInv_panel_clicked()
{
    QString commandString = AddCRC((glueAdapterHeader() + glueString(changeHiLowBytes_uint(INV_CTRL_STOP), IREG_INV_CONTROL)), 2).toHex();
//    ui->textEdit_commandCRC->append(commandString);
    writeSerialPort(commandString);
}

void MainWindow::on_pushButton_alarmInv_panel_clicked()
{
    QString commandString = AddCRC((glueAdapterHeader() + glueString(changeHiLowBytes_uint(INV_CTRL_ALARM), IREG_INV_CONTROL)), 2).toHex();
//    ui->textEdit_commandCRC->append(commandString);
    writeSerialPort(commandString);
}

void MainWindow::on_pushButton_startInv_clicked()
{
    QString commandString = AddCRC((glueAdapterHeader() + glueString(changeHiLowBytes_uint(INV_CTRL_START), IREG_INV_CONTROL)), 2).toHex();
 //   ui->textEdit_commandCRC->append(commandString);
    writeSerialPort(commandString);
}

void MainWindow::on_pushButton_stopInv_clicked()
{
    QString commandString = AddCRC((glueAdapterHeader() + glueString(changeHiLowBytes_uint(INV_CTRL_STOP), IREG_INV_CONTROL)), 2).toHex();
//    ui->textEdit_commandCRC->append(commandString);
    writeSerialPort(commandString);
}

void MainWindow::on_pushButton_alarmInv_clicked()
{
    QString commandString = AddCRC((glueAdapterHeader() + glueString(changeHiLowBytes_uint(INV_CTRL_ALARM), IREG_INV_CONTROL)), 2).toHex();
 //   ui->textEdit_commandCRC->append(commandString);
    writeSerialPort(commandString);
}


void MainWindow::on_radioButton_directRotation_clicked(bool checked)
{
    if(checked){
        ui->groupBox_direction->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() + glueString(changeHiLowBytes_uint(INV_CTRL_RIGHT_DIRECTION), IREG_INV_CONTROL)), 2).toHex();
    //    ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}


void MainWindow::on_radioButton_reversRotation_clicked(bool checked)
{
    if(checked){
        ui->groupBox_direction->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() + glueString(changeHiLowBytes_uint(INV_CTRL_LEFT_DIRECTION), IREG_INV_CONTROL)), 2).toHex();
     //   ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_async_clicked(bool checked)
{
    if(checked){
        ui->groupBox_motorType->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() + glueString(changeHiLowBytes_uint(INV_CTRL_ASYN_MOTOR), IREG_INV_CONTROL)), 2).toHex();
    //    ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_vent_clicked(bool checked)
{
    if(checked){
        ui->groupBox_motorType->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() + glueString(changeHiLowBytes_uint(INV_CTRL_VENT_MOTOR), IREG_INV_CONTROL)), 2).toHex();
     //   ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_PWM_SIN_clicked(bool checked)
{
    if(checked){
        ui->groupBox_PWM_Type->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() + glueString(changeHiLowBytes_uint(INV_CTRL_OVERPWM_OFF), IREG_INV_CONTROL)), 2).toHex();
  //      ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_PWM_OVER1_clicked(bool checked)
{
    if(checked){
        ui->groupBox_PWM_Type->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() + glueString(changeHiLowBytes_uint(INV_CTRL_OVERPWM1_ON), IREG_INV_CONTROL)), 2).toHex();
     //   ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_PWM_OVER2_clicked(bool checked)
{
    if(checked){
        ui->groupBox_PWM_Type->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() + glueString(changeHiLowBytes_uint(INV_CTRL_OVERPWM2_ON), IREG_INV_CONTROL)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_checkBox_harmonicAnalysis_clicked(bool checked)
{
    QString commandString;
    ui->checkBox_harmonicAnalysis->setEnabled(false);
    if(checked){
        commandString = AddCRC((glueAdapterHeader() + glueString(changeHiLowBytes_uint(INV_CTRL_HARM_CALC_ON), IREG_INV_CONTROL)), 2).toHex();
      //  ui->textEdit_commandCRC->append(commandString);
    }
    else{
        commandString = AddCRC((glueAdapterHeader() + glueString(changeHiLowBytes_uint(INV_CTRL_HARM_CALC_OFF), IREG_INV_CONTROL)), 2).toHex();
     //   ui->textEdit_commandCRC->append(commandString);
    }
    writeSerialPort(commandString);
}


void MainWindow::on_pushButton_rstFlt_clicked()
{
//    ui->pushButton_rstFlt->setEnabled(false);

    QString commandString = AddCRC((glueAdapterHeader() + glueString(changeHiLowBytes_uint(INV_CTRL_FLT_CLR), IREG_INV_CONTROL)), 2).toHex();
  //  ui->textEdit_commandCRC->append(commandString);
    writeSerialPort(commandString);
}
