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

}

void MainWindow::checkInvStatus()
{
    QString currentStatus = "--> ";
    QString valueStatus = "-";

    qint16 invStatus = regDataArray[RegnumClass::IREG_INV_STATUS].value.Reg16;

    valueStatus = QString::number(invStatus, 2).rightJustified(16, '0');
    valueStatus.insert(4, " ").insert(9, " ").insert(14, " ");
    ui->lineEdit_status->setText(valueStatus);

    if (invStatus & INV_STS_STARTED) currentStatus += ("Система запущена \n");
    else currentStatus += ("Система остановлена \n");
    if (invStatus & INV_STS_TO_STOP_MODE)  currentStatus += ("Режим остановки \n");
    if (invStatus & INV_STS_WAIT_RECT_START) currentStatus += ("Ожидает запуска выпрямителя \n");
    if (invStatus & INV_STS_STOPPED_REGISTERS) currentStatus += ("Остановлен по изменению важного регистра \n");
    if (invStatus & INV_STS_STOPPED_EXTERNAL)  currentStatus += ("Остановлен по команде извне (CAN, Modbus) \n");
    if (invStatus & INV_STS_WAIT_RECT_STOP) currentStatus += ("Ожидает останова выпрямителя \n");
    if (invStatus & INV_STS_FAULT_STOPPED) currentStatus += ("Остановлен по причине FAULT \n");
    if (invStatus & INV_STS_I_LIMIT) currentStatus += ("Токоограничение \n");
    if (invStatus & INV_STS_ULOW) currentStatus += ("Недостаточно напряжения \n");
    if (invStatus & INV_STS_STOPPED_ALARM) currentStatus += ("Аварийный останов \n");
    if (invStatus & INV_STS_UD_LOW_FAULT) currentStatus += ("Остановлен по снижению напряжения на шине \n");
    if (invStatus & INV_STS_TO_STOP_MODE) currentStatus += ("Режим плавной остановки инвертора \n");
    if (invStatus & INV_STS_URECT_SHORTCIRCUIT) currentStatus += ("Остановлен по КЗ от выпрямителя \n");

    ui->textEdit_status->setText(currentStatus);
}

void MainWindow::checkInvStatus_2()
{
    QString currentStatus = "--> ";
    QString valueStatus = "-";

    qint16 invStatus = regDataArray[RegnumClass::IREG_INV_STATUS2].value.Reg16;

    valueStatus = QString::number(invStatus, 2).rightJustified(16, '0');
    valueStatus.insert(4, " ").insert(9, " ").insert(14, " ");
    ui->lineEdit_status_2->setText(valueStatus);

    if (invStatus & INV_STS2_FC_IT_ERR) currentStatus += ("Токовая защита инвертора \n");
    if (invStatus & INV_STS2_AST_ERR)  currentStatus += ("Ошибка автонастройки \n");
    if (invStatus & INV_STS2_I_LIMIT_FAST) currentStatus += ("Флаг срабатывания токоограничения - свободен \n");
    if (invStatus & INV_STS2_CURRENT_OPT) currentStatus += ("Оптимизация по току \n");
    if (invStatus & INV_STS2_POWER_OPT)  currentStatus += ("Оптимизация по мощности \n");
    if (invStatus & INV_STS2_OPT_DONE) currentStatus += ("Оптимизация выполнена \n");
    if (invStatus & INV_STS2_FLSH_RD_ERR) currentStatus += ("Ошибка чтения флеш уставок \n");
    if (invStatus & INV_STS2_FLSH_WR_ERR) currentStatus += ("Ошибка записи флеш уставок \n");
    if (invStatus & INV_STS2_DISCHARGE_ON) currentStatus += ("Включить разряд емкости (останов турбины для вентильного мотора) \n");
    if (invStatus & INV_STS2_DISCHARGE) currentStatus += ("Идет разряд емкости \n");
    if (invStatus & INV_STS2_DISCHARGE_ERR) currentStatus += ("Ошибка разряда емкости (останова турбины вентильного мотора) \n");
    if (invStatus & INV_STS2_VC_ERR) currentStatus += ("Ошибка векторного алгоритма \n");
    if (invStatus & INV_STS2_M_IFAST_ERR) currentStatus += ("Остановлен по КЗ от выпрямителя \n");
    if (invStatus & INV_STS2_M_I2T_ERR) currentStatus += ("Быстрая токовая защита двигателя \n");
    if (invStatus & INV_STS2_I_LIM_ERR) currentStatus += ("Защита ПЧ по токоограничению \n");
    if (invStatus & INV_STS2_FLSH_VAL_ERR) currentStatus += ("Ошибка диапазона уставок флеш \n");

    ui->textEdit_status_2->setText(currentStatus);
}

void MainWindow::checkInvStatus_3()
{
    QString currentStatus = "--> ";
    QString valueStatus = "-";

    qint16 invStatus = regDataArray[RegnumClass::IREG_INV_STATUS3].value.Reg16;

    valueStatus = QString::number(invStatus, 2).rightJustified(16, '0');
    valueStatus.insert(4, " ").insert(9, " ").insert(14, " ");
    ui->lineEdit_status_3->setText(valueStatus);

    if (invStatus & INV_STS3_RIGHT_DIR) currentStatus += ("Правое направление вращения \n");
    if (invStatus & INV_STS3_OVERPWM1)  currentStatus += ("ОверШИМ тип 1 \n");
    if (invStatus & INV_STS3_OVERPWM2) currentStatus += ("ОверШИМ тип 2 \n");
    if (invStatus & INV_STS3_M_TYPE) currentStatus += ("Тип двигателя - 1 \n");
    else currentStatus += ("Тип двигателя - 0 \n");
    if (invStatus & INV_STS3_PMM_DIS)  currentStatus += ("Запрет вентильного двигателя \n");
    if (invStatus & INV_STS3_RES_TYPE0) currentStatus += ("ОС для подавления резонанса TYPE0 \n");
    if (invStatus & INV_STS3_RES_TYPE1) currentStatus += ("ОС для подавления резонанса TYPE1 \n");
    if (invStatus & INV_STS3_DT_SINGLE) currentStatus += ("Режим контроля связи с термодатчиками - хоть один \n");
    else currentStatus += ("Режим контроля связи с термодатчиками - все \n");
    if (invStatus & INV_STS3_DT_CRITIC) currentStatus += ("Критический температурный режим \n");
    if (invStatus & INV_STS3_PW_ERR_ON) currentStatus += ("Включение мониторов питания \n");
    else currentStatus += ("Выключение мониторов питания \n");
    if (invStatus & INV_STS3_LX_USE) currentStatus += ("Использовать выходную индуктивность Lx \n");
    else currentStatus += ("Не использовать выходную индуктивность Lx \n");
    if (invStatus & INV_STS3_DT_SU) currentStatus += ("Датчик тока на выходе СУ (после фильтра) \n");
    else currentStatus += ("Датчик тока на выходе инвертора (до фильтра) \n");
    if (invStatus & INV_STS3_ZERO_UDROP) currentStatus += ("Включить разрешение сброса напряженя до нуля \n");
    else currentStatus += ("Выключить разрешение сброса напряженя до нуля \n");
    if (invStatus & INV_STS3_NORM_MV_FREF) currentStatus += ("Включен нормированный разгон - торможение \n");
    else currentStatus += ("Выключен нормированный разгон - торможение \n");
    if (invStatus & INV_STS3_HARM_CALC_OFF) currentStatus += ("Выключить гармонический анализ \n");
    else currentStatus += ("Включить гармонический анализ \n");
    if (invStatus & INV_STS3_RMS_POWER) currentStatus += ("Мощность из действ. значения \n");
    else currentStatus += ("Мощность откуда-то \n");

    ui->textEdit_status_3->setText(currentStatus);
}

