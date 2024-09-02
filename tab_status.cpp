#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "invertor.h"
#include "iface.h"
#include "stylehelper.h"


//---------- проверка статуса работы инвертора
void MainWindow::initStatus()
{
    IregInvStatus = NULL;
    IregInvStatus_2 = NULL;
    IregInvStatus_3 = NULL;
    IregInvStatus_flt = NULL;
    IregInvStatus_5 = NULL;

    ui->checkBox_allowCAN_freqChanges->setChecked(false);
    ui->checkBox_allowCAN_freqChanges->setStyleSheet("color: red");

    QStringList canFreqList;
    canFreqList << "125" << "250";
    ui->comboBox_canFreqInv->addItems(canFreqList);
    ui->comboBox_canFreqInv->setEnabled(false);

    ui->lineEdit_status->setReadOnly(true);
    ui->lineEdit_status_2->setReadOnly(true);
    ui->lineEdit_status_3->setReadOnly(true);
    ui->lineEdit_status_flt->setReadOnly(true);
    ui->lineEdit_status_5->setReadOnly(true);

    ui->textEdit_status->setReadOnly(true);
    ui->textEdit_status_2->setReadOnly(true);
    ui->textEdit_status_3->setReadOnly(true);
    ui->textEdit_status_flt->setReadOnly(true);
    ui->textEdit_status_5->setReadOnly(true);

    ui->checkBox_extOut_0->setEnabled(true);
    ui->checkBox_extOut_1->setEnabled(true);
    ui->checkBox_extOut_2->setEnabled(true);
}

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
    regDataArray[RegnumClass::IREG_INV_STATUS].flagNewData = false;
 //   if(ui->tabWidget_registerWidget->currentIndex() != 6) return; // если виджет неактивен, статусы не анализируем

    QString currentStatus = "--> ";
    QString valueStatus = "-";

    quint16 invStatus = quint16(regDataArray[RegnumClass::IREG_INV_STATUS].value.Reg16);

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

    if (IregInvStatus != NULL){
        if(IregInvStatus == invStatus) return;
    }
    IregInvStatus = invStatus;

    ui->textEdit_status->setText(currentStatus);
    ui->textEdit_invertorStatus->setText(currentStatus); // панель в регистрах
}

void MainWindow::checkInvStatus_2()
{
    if(!regDataArray[RegnumClass::IREG_INV_STATUS2].flagNewData) return;
    regDataArray[RegnumClass::IREG_INV_STATUS2].flagNewData = false;

    QString currentStatus = "--> ";
    QString valueStatus = "-";

    quint16 invStatus = quint16(regDataArray[RegnumClass::IREG_INV_STATUS2].value.Reg16);

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

    currentStatus += "------ \n"; // агрегированные состояния

    int fltFlsh = (invStatus & INV_STS2_FLSH_ERR_BITS); // маска на разрешенные фаулты
    if(fltFlsh != 0){
         currentStatus += ("Ошибка микросхемы флеш; \n");
         ui->pushButton_flashErrClr->setStyleSheet("color: red");
    }
    else ui->pushButton_flashErrClr->setStyleSheet("color: green");

    if (IregInvStatus_2 != NULL){
        if(IregInvStatus_2 == invStatus) return;
    }
    IregInvStatus_2 = invStatus;

    ui->textEdit_status_2->setText(currentStatus);
}

void MainWindow::checkInvStatus_3()
{
    if(!regDataArray[RegnumClass::IREG_INV_STATUS3].flagNewData) return;
    regDataArray[RegnumClass::IREG_INV_STATUS3].flagNewData = false;

    QString currentStatus = "--> ";
    QString valueStatus = "-";

    quint16 invStatus = quint16(regDataArray[RegnumClass::IREG_INV_STATUS3].value.Reg16);

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

    if (invStatus & INV_STS3_M_TYPE){
       currentStatus += ("Тип двигателя - вентильный; \n");
       ui->radioButton_vent->setChecked(true);
    }
    else{
      currentStatus += ("Тип двигателя - асинхронный; \n");
      ui->radioButton_async->setChecked(true);
    }

    if (invStatus & INV_STS3_PMM_DIS){
        currentStatus += ("Запрет вентильного двигателя; \n");
        ui->checkBox_pmmOn->setChecked(false);
    }
    else{
       currentStatus += ("Разрешить вентильный двигатель; \n");
       ui->checkBox_pmmOn->setChecked(true);
    }

    if (invStatus & INV_STS3_PW_ERR_ON){
        currentStatus += ("Включение мониторов питания; \n");
        ui->checkBox_powerMonitor->setChecked(true);
    }
    else{
        currentStatus += ("Выключение мониторов питания; \n");
        ui->checkBox_powerMonitor->setChecked(false);
    }

    if (invStatus & INV_STS3_LX_USE){
        currentStatus += ("Использовать выходную индуктивность Lx; \n");
        ui->checkBox_useOutInductance->setChecked(true);
    }
    else{
       currentStatus += ("Не использовать выходную индуктивность Lx; \n");
       ui->checkBox_useOutInductance->setChecked(false);
    }

    if (invStatus & INV_STS3_DT_SU){
       currentStatus += ("Датчик тока на выходе СУ (после фильтра); \n");
       ui->checkBox_currSensOnFilter->setChecked(true);
    }
    else{
       currentStatus += ("Датчик тока на выходе инвертора (до фильтра); \n");
       ui->checkBox_currSensOnFilter->setChecked(false);
    }

    if (invStatus & INV_STS3_ZERO_UDROP){
        currentStatus += ("Включить разрешение сброса напряженя до нуля; \n");
        ui->checkBox_enableZeroUdc->setChecked(true);
    }
    else{
        currentStatus += ("Выключить разрешение сброса напряженя до нуля; \n");
        ui->checkBox_enableZeroUdc->setChecked(false);
    }

    if (invStatus & INV_STS3_NORM_MV_FREF){
        currentStatus += ("Включен нормированный разгон - торможение; \n");
        ui->checkBox_rationingTime->setChecked(true);
    }
    else{
        currentStatus += ("Выключен нормированный разгон - торможение; \n");
        ui->checkBox_rationingTime->setChecked(false);
    }

    if (invStatus & INV_STS3_HARM_CALC_OFF){
        currentStatus += ("Выключен гармонический анализ; \n");
        ui->checkBox_harmonicAnalysis->setChecked(false);
    }
    else{
      currentStatus += ("Включен гармонический анализ; \n");
      ui->checkBox_harmonicAnalysis->setChecked(true);
    }

    if (invStatus & INV_STS3_RMS_POWER){
        currentStatus += ("Мощность из действ. значения; \n");
        ui->checkBox_powerIsRMS->setChecked(true);
    }
    else{
       currentStatus += ("Мощность из анализатора; \n");
       ui->checkBox_powerIsRMS->setChecked(false);
    }

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
    if(resType == STS3_RES_ANGLE){
        currentStatus += ("Подавление резонанса - по углу ; \n");
        ui->radioButton_resDempAngle->setChecked(true);
    }

    if(resType == STS3_RES_TORQUE){
        currentStatus += ("Подавление резонанса - по моменту ; \n");
        ui->radioButton_resDempTorque->setChecked(true);
    }
    if(resType == STS3_RES_POWER){
       currentStatus += ("Подавление резонанса - по мощности ; \n");
       ui->radioButton_resDempPower->setChecked(true);
    }
    if(resType == STS3_RES_RESERVED) currentStatus += ("Подавление резонанса - зарезервированное ; \n");

    // обработка термодатчиков
    if (invStatus & INV_STS3_DT_SINGLE){
        currentStatus += ("Режим контроля связи с термодатчиками - хоть один; \n");
        ui->radioButton_termosensorSingle->setChecked(true);
    }
    else{
      currentStatus += ("Режим контроля связи с термодатчиками - все; \n");
      ui->radioButton_termosensorAll->setChecked(true);
    }
    if (invStatus & INV_STS3_DT_CRITIC){
      currentStatus += ("Критический температурный режим; \n");
      ui->radioButton_termosensorCritic->setChecked(true);
    }

    ui->groupBox_direction->setEnabled(true);
    ui->groupBox_motorType->setEnabled(true);
    ui->groupBox_PWM_Type->setEnabled(true);   
    ui->groupBox_resonansDemping->setEnabled(true);
    ui->groupBox_termosensMode->setEnabled(true);

    ui->checkBox_powerMonitor->setEnabled(true);
    ui->checkBox_currSensOnFilter->setEnabled(true);
    ui->checkBox_harmonicAnalysis->setEnabled(true);
    ui->checkBox_enableZeroUdc->setEnabled(true);
    ui->checkBox_rationingTime->setEnabled(true);
    ui->checkBox_useOutInductance->setEnabled(true);
    ui->checkBox_pmmOn->setEnabled(true);
    ui->checkBox_powerIsRMS->setEnabled(true);

    if (IregInvStatus_3 != NULL){
        if(IregInvStatus_3 == invStatus) return;
    }
    IregInvStatus_3 = invStatus;

    ui->textEdit_status_3->setText(currentStatus);
}

void MainWindow::checkInvStatus_flt()
{
    if(!regDataArray[RegnumClass::IREG_INV_FAULT].flagNewData) return;
    regDataArray[RegnumClass::IREG_INV_FAULT].flagNewData = false;

    QString currentStatus = "--> ";
    QString valueStatus = "-";

    quint16 invStatus = quint16(regDataArray[RegnumClass::IREG_INV_FAULT].value.Reg16);

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
    if (IregInvStatus_flt != NULL){
        if(IregInvStatus_flt == invStatus) return;
    }
    IregInvStatus_flt = invStatus;

    ui->textEdit_status_flt->setText(currentStatus);
}

void MainWindow::checkInvStatus_5()
{
    if(!regDataArray[RegnumClass::IREG_INV_STATUS5].flagNewData) return;
    regDataArray[RegnumClass::IREG_INV_STATUS5].flagNewData = false;

    QString currentStatus = "--> ";
    QString valueStatus = "-";

    quint16 invStatus = quint16(regDataArray[RegnumClass::IREG_INV_STATUS5].value.Reg16);

    valueStatus = QString::number(invStatus, 2).rightJustified(16, '0');
    valueStatus.insert(4, " ").insert(9, " ").insert(14, " ");
    ui->lineEdit_status_5->setText(valueStatus);

    int expType = (invStatus & STS5_EXP_CONF_2b);  // маска на два бита типа платы расширения
    if (expType == STS5_EXP_LEGACY){
      currentStatus += ("Конфигурация платы расширения - LEGACY; \n");
      ui->radioButton_expLegacy->setChecked(true);
    }
    else ui->radioButton_expLegacy->setChecked(false);

    if (expType == STS5_EXP_485){
       currentStatus += ("Конфигурация платы расширения - 485; \n");
       ui->radioButton_exp485->setChecked(true);
    }
    else ui->radioButton_exp485->setChecked(false);

    if (expType == STS5_EXP_SPI){
       currentStatus += ("Конфигурация платы расширения - SPI; \n");
       ui->radioButton_expSPI->setChecked(true);
    }
    else ui->radioButton_expSPI->setChecked(false);

    if (expType == STS5_EXP_OTHERS){
        currentStatus += ("Конфигурация платы расширения - OTHERS; \n");
        ui->radioButton_expOthers->setChecked(true);
    }
    else ui->radioButton_expOthers->setChecked(false);

    int convType = ((invStatus & STS5_FC_CONF_6b) >> STS5_FC_CONF_LST_BIT);  // маска на 6 бит конструкции ЧРП
    if (convType == FC_CONF_BASE){
       currentStatus += ("Конфигурация ПЧ - BASE; \n");
       ui->radioButton_baseConverter->setChecked(true);
    }
    else ui->radioButton_baseConverter->setChecked(true);
    if (convType == FC_CONF_X1_ISING_RSING){
        currentStatus += ("Конфигурация ПЧ - X1_ISING_RSING; \n");
        ui->radioButton_ISING_RSING->setChecked(true);
    }
    else ui->radioButton_ISING_RSING->setChecked(false);
    if (convType == FC_CONF_X1_ISING_RDOUB){
       currentStatus += ("Конфигурация ПЧ - X1_ISING_RDOUB; \n");
       ui->radioButton_ISING_RDOUB->setChecked(true);
    }
    else ui->radioButton_ISING_RDOUB->setChecked(false);
    if (convType == FC_CONF_X1_IDOUB_RSING){
        currentStatus += ("Конфигурация ПЧ - X1_IDOUB_RSING; \n");
        ui->radioButton_IDOUB_RSING->setChecked(true);
    }
    else ui->radioButton_IDOUB_RSING->setChecked(false);
    if (convType == FC_CONF_X1_IDOUB_RDOUB){
        currentStatus += ("Конфигурация ПЧ - X1_IDOUB_RDOUB; \n");
        ui->radioButton_IDOUB_RDOUB->setChecked(true);
    }
    else ui->radioButton_IDOUB_RDOUB->setChecked(false);
    if (convType == FC_CONF_X1_ITRIP_RSING){
        currentStatus += ("Конфигурация ПЧ - X1_ITRIP_RSING; \n");
        ui->radioButton_ITRIP_RSING->setChecked(true);
    }
    else ui->radioButton_ITRIP_RSING->setChecked(false);
    if (convType == FC_CONF_X1_ITRIP_RDOUB){
        currentStatus += ("Конфигурация ПЧ - X1_ITRIP_RDOUB; \n");
        ui->radioButton_ITRIP_RDOUB->setChecked(true);
    }
    else ui->radioButton_ITRIP_RDOUB->setChecked(false);
    if (convType == FC_CONF_X1_IQUAD_RSING){
         currentStatus += ("Конфигурация ПЧ - X1_IQUAD_RSING; \n");
         ui->radioButton_IQUAD_RSING->setChecked(true);
    }
    else ui->radioButton_IQUAD_RSING->setChecked(false);
    if (convType == FC_CONF_X1_IQUAD_RDOUB){
        currentStatus += ("Конфигурация ПЧ - X1_IQUAD_RDOUB; \n");
        ui->radioButton_IQUAD_RDOUB->setChecked(true);
    }
    else ui->radioButton_IQUAD_RDOUB->setChecked(false);
    if (convType == FC_CONF_X2_IQUAD_RSING){
        currentStatus += ("Конфигурация ПЧ - X2_IQUAD_RSING; \n");
        ui->radioButton_IQUAD_RSING_2->setChecked(true);
    }
    else ui->radioButton_IQUAD_RSING_2->setChecked(false);
    if (convType == FC_CONF_X2_IQUAD_RDOUB){
        currentStatus += ("Конфигурация ПЧ - X2_IQUAD_RDOUB; \n");
        ui->radioButton_IQUAD_RDOUB_2->setChecked(true);
    }
    else ui->radioButton_IQUAD_RDOUB_2->setChecked(false);

    int invType = (invStatus & STS5_FC_TYPE_2b);  // маска на два бита типа работы инвертора
    if (invType == STS5_FC_SUBPUMP){
       currentStatus += ("Тип ПЧ - SUBPUMP; \n");
       ui->radioButton_subpump->setChecked(true);
    }
    else ui->radioButton_subpump->setChecked(false);

    if (invType == STS5_FC_TRAC){
       currentStatus += ("Тип ПЧ - TRAC; \n");
       ui->radioButton_trac->setChecked(true);
    }
    else ui->radioButton_trac->setChecked(false);

    if (invStatus & STS5_CAN_BDRT_250){
        ui->comboBox_canFreqInv->setCurrentIndex(1);
        currentStatus += ("Частота CAN шины - 250; \n");
    }
    else{
        currentStatus += ("Частота CAN шины - 125; \n");
        ui->comboBox_canFreqInv->setCurrentIndex(0);
    }

    // изменение бита ШИМ STS5_PWM_CTRL
    if (invStatus & STS5_PWM_CTRL){
       currentStatus += ("Управление ШИМ включено; \n");
      ui->checkBox_pwmControl->setChecked(true);
    }
    else ui->checkBox_pwmControl->setChecked(false);
    ui->checkBox_pwmControl->setEnabled(true);

    // изменение бита вывода параметров STS5_PARAMS_OUT_MODE
    if (invStatus & STS5_PARAMS_OUT_MODE){
        currentStatus += ("Параметры в сэмплах; \n");
        ui->checkBox_paramsOutChange->setChecked(true);
    }
    else{
        currentStatus += ("Параметры в регистрах; \n");
        ui->checkBox_paramsOutChange->setChecked(false);
    }
    ui->checkBox_paramsOutChange->setEnabled(true);

    // при запрете на изменение скорсти CAN не даем активность комбобоксу скоростей
    if(ui->checkBox_allowCAN_freqChanges->isChecked())  ui->comboBox_canFreqInv->setEnabled(true);

    ui->groupBox_expBoard->setEnabled(true);
    ui->groupBox_convType->setEnabled(true);
    ui->groupBox_converterDesign->setEnabled(true);

    if (IregInvStatus_5 != NULL){
        if(IregInvStatus_5 == invStatus) return;
    }
    IregInvStatus_5 = invStatus;

    ui->textEdit_status_5->setText(currentStatus);
}


void MainWindow::on_pushButton_startInv_panel_clicked()
{
    QString commandString = AddCRC((glueAdapterHeader() +
                            glueString(changeHiLowBytes_uint(INV_CTRL_START), IREG_INV_CONTROL)), 2).toHex();
  //  ui->textEdit_commandCRC->append(commandString);
    writeSerialPort(commandString);
}

void MainWindow::on_pushButton_stopInv_panel_clicked()
{
    QString commandString = AddCRC((glueAdapterHeader() +
                            glueString(changeHiLowBytes_uint(INV_CTRL_STOP), IREG_INV_CONTROL)), 2).toHex();
//    ui->textEdit_commandCRC->append(commandString);
    writeSerialPort(commandString);
}

void MainWindow::on_pushButton_alarmInv_panel_clicked()
{
    QString commandString = AddCRC((glueAdapterHeader() +
                            glueString(changeHiLowBytes_uint(INV_CTRL_ALARM), IREG_INV_CONTROL)), 2).toHex();
//    ui->textEdit_commandCRC->append(commandString);
    writeSerialPort(commandString);
}

void MainWindow::on_pushButton_startInv_clicked()
{
    QString commandString = AddCRC((glueAdapterHeader() +
                            glueString(changeHiLowBytes_uint(INV_CTRL_START), IREG_INV_CONTROL)), 2).toHex();
 //   ui->textEdit_commandCRC->append(commandString);
    writeSerialPort(commandString);
}

void MainWindow::on_pushButton_stopInv_clicked()
{
    QString commandString = AddCRC((glueAdapterHeader() +
                            glueString(changeHiLowBytes_uint(INV_CTRL_STOP), IREG_INV_CONTROL)), 2).toHex();
//    ui->textEdit_commandCRC->append(commandString);
    writeSerialPort(commandString);
}

void MainWindow::on_pushButton_alarmInv_clicked()
{
    QString commandString = AddCRC((glueAdapterHeader() +
                            glueString(changeHiLowBytes_uint(INV_CTRL_ALARM), IREG_INV_CONTROL)), 2).toHex();
 //   ui->textEdit_commandCRC->append(commandString);
    writeSerialPort(commandString);
}

void MainWindow::on_radioButton_directRotation_clicked(bool checked)
{
    if(checked){
        ui->groupBox_direction->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                                glueString(changeHiLowBytes_uint(INV_CTRL_RIGHT_DIRECTION), IREG_INV_CONTROL)), 2).toHex();
    //    ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_reversRotation_clicked(bool checked)
{
    if(checked){
        ui->groupBox_direction->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                                glueString(changeHiLowBytes_uint(INV_CTRL_LEFT_DIRECTION), IREG_INV_CONTROL)), 2).toHex();
     //   ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_async_clicked(bool checked)
{
    if(checked){
        ui->groupBox_motorType->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                                glueString(changeHiLowBytes_uint(INV_CTRL_ASYN_MOTOR), IREG_INV_CONTROL)), 2).toHex();
    //    ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_vent_clicked(bool checked)
{
    if(checked){
        ui->groupBox_motorType->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                                glueString(changeHiLowBytes_uint(INV_CTRL_VENT_MOTOR), IREG_INV_CONTROL)), 2).toHex();
     //   ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_PWM_SIN_clicked(bool checked)
{
    if(checked){
        ui->groupBox_PWM_Type->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                                glueString(changeHiLowBytes_uint(INV_CTRL_OVERPWM_OFF), IREG_INV_CONTROL)), 2).toHex();
  //      ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_PWM_OVER1_clicked(bool checked)
{
    if(checked){
        ui->groupBox_PWM_Type->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                                glueString(changeHiLowBytes_uint(INV_CTRL_OVERPWM1_ON), IREG_INV_CONTROL)), 2).toHex();
     //   ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_PWM_OVER2_clicked(bool checked)
{
    if(checked){
        ui->groupBox_PWM_Type->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                                glueString(changeHiLowBytes_uint(INV_CTRL_OVERPWM2_ON), IREG_INV_CONTROL)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_pushButton_rstFlt_clicked()
{
//    ui->pushButton_rstFlt->setEnabled(false);

    QString commandString = AddCRC((glueAdapterHeader() +
                            glueString(changeHiLowBytes_uint(INV_CTRL_FLT_CLR), IREG_INV_CONTROL)), 2).toHex();
  //  ui->textEdit_commandCRC->append(commandString);
    writeSerialPort(commandString);
}

void MainWindow::on_radioButton_resDempAngle_clicked(bool checked)
{
    if(checked){
        ui->groupBox_resonansDemping->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                                glueString(changeHiLowBytes_uint(INV_CTRL2_RES_ANGLE), RegnumClass::IREG_INV_CTRL2)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_resDempTorque_clicked(bool checked)
{
    if(checked){
        ui->groupBox_resonansDemping->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                                glueString(changeHiLowBytes_uint(INV_CTRL2_RES_TORQUE), RegnumClass::IREG_INV_CTRL2)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_resDempPower_clicked(bool checked)
{
    if(checked){
        ui->groupBox_resonansDemping->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                                glueString(changeHiLowBytes_uint(INV_CTRL2_RES_POWER), RegnumClass::IREG_INV_CTRL2)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_termosensorSingle_clicked(bool checked)
{
    if(checked){
        ui->groupBox_termosensMode->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                                glueString(changeHiLowBytes_uint(INV_CTRL2_DT_SINGLE_MODE), RegnumClass::IREG_INV_CTRL2)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_termosensorCritic_clicked(bool checked)
{
    if(checked){
        ui->groupBox_termosensMode->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                                glueString(changeHiLowBytes_uint(INV_CTRL2_DT_CRITIC_MODE), RegnumClass::IREG_INV_CTRL2)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_termosensorAll_clicked(bool checked)
{
    if(checked){
        ui->groupBox_termosensMode->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                                glueString(changeHiLowBytes_uint(INV_CTRL2_DT_ALL_MODE), RegnumClass::IREG_INV_CTRL2)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}


// проверка чек-боксов

void MainWindow::on_checkBox_harmonicAnalysis_clicked(bool checked)
{
    QString commandString;
    ui->checkBox_harmonicAnalysis->setEnabled(false);
    if(checked){
        commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(INV_CTRL_HARM_CALC_ON), IREG_INV_CONTROL)), 2).toHex();
      //  ui->textEdit_commandCRC->append(commandString);
    }
    else{
        commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(INV_CTRL_HARM_CALC_OFF), IREG_INV_CONTROL)), 2).toHex();
     //   ui->textEdit_commandCRC->append(commandString);
    }
    writeSerialPort(commandString);
}

void MainWindow::on_checkBox_powerMonitor_clicked(bool checked)
{
    QString commandString;
    ui->checkBox_powerMonitor->setEnabled(false);
    if(checked){
        commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(INV_CTRL2_PW_ERR_ON), RegnumClass::IREG_INV_CTRL2)), 2).toHex();
      //  ui->textEdit_commandCRC->append(commandString);
    }
    else{
        commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(INV_CTRL2_PW_ERR_OFF), RegnumClass::IREG_INV_CTRL2)), 2).toHex();
     //   ui->textEdit_commandCRC->append(commandString);
    }
    writeSerialPort(commandString);
}

void MainWindow::on_checkBox_currSensOnFilter_clicked(bool checked)
{
    QString commandString;
    ui->checkBox_currSensOnFilter->setEnabled(false);
    if(checked){
        commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(INV_CTRL2_DT_SU_ON), RegnumClass::IREG_INV_CTRL2)), 2).toHex();
      //  ui->textEdit_commandCRC->append(commandString);
    }
    else{
        commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(INV_CTRL2_DT_SU_OFF), RegnumClass::IREG_INV_CTRL2)), 2).toHex();
     //   ui->textEdit_commandCRC->append(commandString);
    }
    writeSerialPort(commandString);
}

void MainWindow::on_checkBox_rationingTime_clicked(bool checked)
{
    QString commandString;
    ui->checkBox_rationingTime->setEnabled(false);
    if(checked){
        commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(INV_CTRL2_NORM_MV_FREF_ON), RegnumClass::IREG_INV_CTRL2)), 2).toHex();
      //  ui->textEdit_commandCRC->append(commandString);
    }
    else{
        commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(INV_CTRL2_NORM_MV_FREF_OFF), RegnumClass::IREG_INV_CTRL2)), 2).toHex();
     //   ui->textEdit_commandCRC->append(commandString);
    }
    writeSerialPort(commandString);
}

void MainWindow::on_checkBox_enableZeroUdc_clicked(bool checked)
{
    QString commandString;
    ui->checkBox_enableZeroUdc->setEnabled(false);
    if(checked){
        commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(INV_CTRL2_ZERO_UDROP_ON), RegnumClass::IREG_INV_CTRL2)), 2).toHex();
      //  ui->textEdit_commandCRC->append(commandString);
    }
    else{
        commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(INV_CTRL2_ZERO_UDROP_OFF), RegnumClass::IREG_INV_CTRL2)), 2).toHex();
     //   ui->textEdit_commandCRC->append(commandString);
    }
    writeSerialPort(commandString);
}

void MainWindow::on_checkBox_useOutInductance_clicked(bool checked)
{
    QString commandString;
    ui->checkBox_useOutInductance->setEnabled(false);
    if(checked){
        commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(INV_CTRL2_LX_USE_ON), RegnumClass::IREG_INV_CTRL2)), 2).toHex();
      //  ui->textEdit_commandCRC->append(commandString);
    }
    else{
        commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(INV_CTRL2_LX_USE_OFF), RegnumClass::IREG_INV_CTRL2)), 2).toHex();
     //   ui->textEdit_commandCRC->append(commandString);
    }
    writeSerialPort(commandString);
}

void MainWindow::on_checkBox_powerIsRMS_clicked(bool checked)
{
    QString commandString;
    ui->checkBox_powerIsRMS->setEnabled(false);
    if(checked){
        commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(CTRL3_RMS_POWER_ON), RegnumClass::IREG_CTRL3)), 2).toHex();
      //  ui->textEdit_commandCRC->append(commandString);
    }
    else{
        commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(CTRL3_RMS_POWER_OFF), RegnumClass::IREG_CTRL3)), 2).toHex();
     //   ui->textEdit_commandCRC->append(commandString);
    }
    writeSerialPort(commandString);
}

void MainWindow::on_checkBox_pmmOn_clicked(bool checked)
{
    QString commandString;
    ui->checkBox_pmmOn->setEnabled(false);
    if(checked){
        commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(INT_CTRL_PMM_ON), RegnumClass::IREG_INV_INT_CTRL)), 2).toHex();
      //  ui->textEdit_commandCRC->append(commandString);
    }
    else{
        commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(INT_CTRL_PMM_OFF), RegnumClass::IREG_INV_INT_CTRL)), 2).toHex();
     //   ui->textEdit_commandCRC->append(commandString);
    }
    writeSerialPort(commandString);
}

void MainWindow::on_checkBox_pwmControl_clicked()
{
   // qDebug() << "изменился чекбокс управления ШИМ"; // INT_CTRL_PWM_CTRL_CHANGE
    QString commandString;
    ui->checkBox_pwmControl->setEnabled(false);

    // на любое изменение чекбокса пишем бит в регистр управления
    commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(INT_CTRL_PWM_CTRL_CHANGE), RegnumClass::IREG_INV_INT_CTRL)), 2).toHex();

    writeSerialPort(commandString);
}

void MainWindow::on_checkBox_paramsOutChange_clicked()
{
   // qDebug() << "изменился чекбокс управления параметрами"; // CTRL3_PARAMS_OUT_CHANGE
    QString commandString;
    ui->checkBox_paramsOutChange->setEnabled(false);
    commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(CTRL3_PARAMS_OUT_CHANGE), RegnumClass::IREG_CTRL3)), 2).toHex();

    writeSerialPort(commandString);
}





void MainWindow::on_radioButton_expLegacy_clicked(bool checked)
{
    if(checked){
        ui->groupBox_expBoard->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                                glueString(changeHiLowBytes_uint(CTRL3_EXP_LEGACY_SET), RegnumClass::IREG_CTRL3)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_exp485_clicked(bool checked)
{
    if(checked){
        ui->groupBox_expBoard->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                                glueString(changeHiLowBytes_uint(CTRL3_EXP_485_SET), RegnumClass::IREG_CTRL3)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_expSPI_clicked(bool checked)
{
    if(checked){
        ui->groupBox_expBoard->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                                glueString(changeHiLowBytes_uint(CTRL3_EXP_SPI_SET), RegnumClass::IREG_CTRL3)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_expOthers_clicked(bool checked)
{
    if(checked){
        ui->groupBox_expBoard->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                                glueString(changeHiLowBytes_uint(CTRL3_EXP_OTHERS_SET), RegnumClass::IREG_CTRL3)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_trac_clicked(bool checked)
{
    if(checked){
        ui->groupBox_convType->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                                glueString(changeHiLowBytes_uint(CTRL3_FC_TRAC_SET), RegnumClass::IREG_CTRL3)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_subpump_clicked(bool checked)
{
    if(checked){
        ui->groupBox_convType->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                                glueString(changeHiLowBytes_uint(CTRL3_FC_SUBPUMP_SET), RegnumClass::IREG_CTRL3)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_comboBox_canFreqInv_currentIndexChanged(int index)
{
    ui->comboBox_canFreqInv->setEnabled(false);
    quint16 commandCanFreqInv;
    switch (index) {
    case 0: commandCanFreqInv = INT_CTRL_SET_BDRT_125; break;
    case 1: commandCanFreqInv = INT_CTRL_SET_BDRT_250; break;
    default: commandCanFreqInv = INT_CTRL_SET_BDRT_250; break;
    }
    QString commandString = AddCRC((glueAdapterHeader() +
                            glueString(changeHiLowBytes_uint(commandCanFreqInv), RegnumClass::IREG_INV_INT_CTRL)), 2).toHex();
//     ui->textEdit_commandCRC->append(commandString);
    writeSerialPort(commandString);
}

void MainWindow::on_checkBox_allowCAN_freqChanges_clicked(bool checked)
{
    if(checked) QMessageBox::warning(this, "Внимание","Изменение скорости CAN шины. Возможен конфлик скоростей и потеря связи.");
    else ui->comboBox_canFreqInv->setEnabled(false);
}

//
void MainWindow::on_checkBox_extOut_0_clicked(bool checked)
{
    QString commandString;
  //  ui->checkBox_pmmOn->setEnabled(false);
    if(checked){
        commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(INT_CTRL_SET_EXT_OUT0), RegnumClass::IREG_INV_INT_CTRL)), 2).toHex();
      //  ui->textEdit_commandCRC->append(commandString);
    }
    else{
        commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(INT_CTRL_CLR_EXT_OUT0), RegnumClass::IREG_INV_INT_CTRL)), 2).toHex();
     //   ui->textEdit_commandCRC->append(commandString);
    }
    writeSerialPort(commandString);
}

void MainWindow::on_checkBox_extOut_1_clicked(bool checked)
{
    QString commandString;
  //  ui->checkBox_pmmOn->setEnabled(false);
    if(checked){
        commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(INT_CTRL_SET_EXT_OUT1), RegnumClass::IREG_INV_INT_CTRL)), 2).toHex();
      //  ui->textEdit_commandCRC->append(commandString);
    }
    else{
        commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(INT_CTRL_CLR_EXT_OUT1), RegnumClass::IREG_INV_INT_CTRL)), 2).toHex();
     //   ui->textEdit_commandCRC->append(commandString);
    }
    writeSerialPort(commandString);
}

void MainWindow::on_checkBox_extOut_2_clicked(bool checked)
{
    QString commandString;
  //  ui->checkBox_pmmOn->setEnabled(false);
    if(checked){
        commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(INT_CTRL_SET_EXT_OUT2), RegnumClass::IREG_INV_INT_CTRL)), 2).toHex();
      //  ui->textEdit_commandCRC->append(commandString);
    }
    else{
        commandString = AddCRC((glueAdapterHeader() +
                        glueString(changeHiLowBytes_uint(INT_CTRL_CLR_EXT_OUT2), RegnumClass::IREG_INV_INT_CTRL)), 2).toHex();
     //   ui->textEdit_commandCRC->append(commandString);
    }
    writeSerialPort(commandString);
}


// Настройка конструкции ЧРП

void MainWindow::on_radioButton_baseConverter_clicked(bool checked)
{
    if(checked){
        ui->groupBox_converterDesign->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                glueString(changeHiLowBytes_uint(CTRL3_FC_BASE_SET), RegnumClass::IREG_CTRL3)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_ISING_RSING_clicked(bool checked)
{
    if(checked){
        ui->groupBox_converterDesign->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                glueString(changeHiLowBytes_uint(CTRL3_FC_X1_ISING_RSING_SET), RegnumClass::IREG_CTRL3)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_ISING_RDOUB_clicked(bool checked)
{
    if(checked){
        ui->groupBox_converterDesign->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                glueString(changeHiLowBytes_uint(CTRL3_FC_X1_ISING_RDOUB_SET), RegnumClass::IREG_CTRL3)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_IDOUB_RSING_clicked(bool checked)
{
    if(checked){
        ui->groupBox_converterDesign->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                glueString(changeHiLowBytes_uint(CTRL3_FC_X1_IDOUB_RSING_SET), RegnumClass::IREG_CTRL3)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_IDOUB_RDOUB_clicked(bool checked)
{
    if(checked){
        ui->groupBox_converterDesign->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                glueString(changeHiLowBytes_uint(CTRL3_FC_X1_IDOUB_RDOUB_SET), RegnumClass::IREG_CTRL3)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_ITRIP_RSING_clicked(bool checked)
{
    if(checked){
        ui->groupBox_converterDesign->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                glueString(changeHiLowBytes_uint(CTRL3_FC_X1_ITRIP_RSING_SET), RegnumClass::IREG_CTRL3)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_ITRIP_RDOUB_clicked(bool checked)
{
    if(checked){
        ui->groupBox_converterDesign->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                glueString(changeHiLowBytes_uint(CTRL3_FC_X1_ITRIP_RDOUB_SET), RegnumClass::IREG_CTRL3)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_IQUAD_RSING_clicked(bool checked)
{
    if(checked){
        ui->groupBox_converterDesign->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                glueString(changeHiLowBytes_uint(CTRL3_FC_X1_IQUAD_RSING_SET), RegnumClass::IREG_CTRL3)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_IQUAD_RDOUB_clicked(bool checked)
{
    if(checked){
        ui->groupBox_converterDesign->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                glueString(changeHiLowBytes_uint(CTRL3_FC_X1_IQUAD_RDOUB_SET), RegnumClass::IREG_CTRL3)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_IQUAD_RSING_2_clicked(bool checked)
{
    if(checked){
        ui->groupBox_converterDesign->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                glueString(changeHiLowBytes_uint(CTRL3_FC_X2_IQUAD_RSING_SET), RegnumClass::IREG_CTRL3)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

void MainWindow::on_radioButton_IQUAD_RDOUB_2_clicked(bool checked)
{
    if(checked){
        ui->groupBox_converterDesign->setEnabled(false);
        QString commandString = AddCRC((glueAdapterHeader() +
                glueString(changeHiLowBytes_uint(CTRL3_FC_X2_IQUAD_RDOUB_SET), RegnumClass::IREG_CTRL3)), 2).toHex();
   //     ui->textEdit_commandCRC->append(commandString);
        writeSerialPort(commandString);
    }
}

// работа с флэш

void MainWindow::on_pushButton_flashErrClr_clicked()
{
    QString commandString = AddCRC((glueAdapterHeader() +
                            glueString(changeHiLowBytes_uint(INT_CTRL_FLSH_ERR_CLR), IREG_INV_CONTROL)), 2).toHex();
 //   ui->textEdit_commandCRC->append(commandString);
    writeSerialPort(commandString);
}

void MainWindow::on_pushButton_flashWrAll_clicked()
{
    QString commandString = AddCRC((glueAdapterHeader() +
                            glueString(changeHiLowBytes_uint(INT_CTRL_FLSH_WR_ALL), IREG_INV_CONTROL)), 2).toHex();
 //   ui->textEdit_commandCRC->append(commandString);
    writeSerialPort(commandString);
}
