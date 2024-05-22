#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "invertor.h"
#include "iface.h"
#include "stylehelper.h"

//---------- проверка статуса работы инвертора
void MainWindow::checkStatus()
{
    QString currentStatus = "---";
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

//----------- для дальнейшего отображения статуса инвертора!!!!
/* система запущена */
//#define INV_STS_STARTED			(1 << 0)

/* ожидает запуска выпрямителя */
//#define INV_STS_WAIT_RECT_START		(1 << 1)

/* Инвертор остановлен по изменению какого-либо важного регистра */
//#define INV_STS_STOPPED_REGISTERS	(1 << 2)

/* Инвертор остановлен по команде извне (CAN, Modbus) */
//#define INV_STS_STOPPED_EXTERNAL	(1 << 3)

/* ожидает останова выпрямителя */
//#define INV_STS_WAIT_RECT_STOP		(1 << 4)

/* Остановлен по причине FAULT */
//#define INV_STS_FAULT_STOPPED		(1 << 5)

/* Фактическое направление вращения */
//#define INV_STS_RIGHT_DIR			(1 << 6)

/* Токоограничение */
//#define INV_STS_I_LIMIT				(1 << 7)

/* Недостаточно напряжения */
//#define INV_STS_ULOW				(1 << 8)

/* остановлен аварийной кнопкой */
//#define INV_STS_STOPPED_ALARM		(1 << 9)

/* остановлен по снижению напряжения на шине */
//#define INV_STS_UD_LOW_FAULT		(1 << 10)

/* остановлен по снижению напряжения на шине */
//#define INV_STS_UD_HIGH_FAULT		(1 << 11)

/* напряжение не в норме так или эдак */
//#define INV_STS_UD_FAULT \
//	(INV_STS_UD_LOW_FAULT | INV_STS_UD_HIGH_FAULT)

/* режим плавной остановки инвертора */
//#define INV_STS_TO_STOP_MODE		(1 << 12)

/* Активный режим (R, D - "1"; P, N - "0") */
//#define INV_STS_RUN					(1 << 13)

/* остановлен по КЗ зареганному от выпрямителя */
//#define INV_STS_URECT_SHORTCIRCUIT	(1 << 14)
