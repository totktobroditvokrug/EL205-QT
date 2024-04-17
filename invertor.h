#ifndef INVERTOR_H
#define INVERTOR_H


// Запись регистра
#define REGISTER_WRITE_ID    0x0A00

// длина данных для записи регистра
#define IREG_INV_LEN  0x03

// длина данных команды записи регистра для адаптера
#define ADAPT_REG_SET_LEN  0x08

// длина данных команды записи регистра для адаптера
#define ADAPT_TYPE_STD_CAN  0x00

// регистр вкл/выкл
#define IREG_INV_CONTROL  0x5A


/* команда "запустить" */
#define INV_CTRL_START		0x0100
/* команда "остановить" */
#define INV_CTRL_STOP		0x0200
/* аварийный стоп */
#define INV_CTRL_ALARM		0x1000

/* ==========================================================================
 * Регистр статуса инвертора IREG_INV_STATUS
 * ========================================================================== */

/* система запущена */
#define INV_STS_STARTED			(1 << 0)

/* ожидает запуска выпрямителя */
#define INV_STS_WAIT_RECT_START		(1 << 1)

/* Инвертор остановлен по изменению какого-либо важного регистра */
#define INV_STS_STOPPED_REGISTERS	(1 << 2)

/* Инвертор остановлен по команде извне (CAN, Modbus) */
#define INV_STS_STOPPED_EXTERNAL	(1 << 3)

/* ожидает останова выпрямителя */
#define INV_STS_WAIT_RECT_STOP		(1 << 4)

/* Остановлен по причине FAULT */
#define INV_STS_FAULT_STOPPED		(1 << 5)

/* Фактическое направление вращения */
#define INV_STS_RIGHT_DIR			(1 << 6)

/* Токоограничение */
#define INV_STS_I_LIMIT				(1 << 7)

/* Недостаточно напряжения */
#define INV_STS_ULOW				(1 << 8)

/* остановлен аварийной кнопкой */
#define INV_STS_STOPPED_ALARM		(1 << 9)

/* остановлен по снижению напряжения на шине */
#define INV_STS_UD_LOW_FAULT		(1 << 10)

/* остановлен по снижению напряжения на шине */
#define INV_STS_UD_HIGH_FAULT		(1 << 11)

/* напряжение не в норме так или эдак */
#define INV_STS_UD_FAULT \
    (INV_STS_UD_LOW_FAULT | INV_STS_UD_HIGH_FAULT)

/* режим плавной остановки инвертора */
#define INV_STS_TO_STOP_MODE		(1 << 12)

/* Активный режим (R, D - "1"; P, N - "0") */
#define INV_STS_RUN					(1 << 13)

/* остановлен по КЗ зареганному от выпрямителя */
#define INV_STS_URECT_SHORTCIRCUIT	(1 << 14)

#endif // INVERTOR_H
