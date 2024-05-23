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

/* ==========================================================================
 * Регистр расширенного статуса инвертора IREG_INV_STATUS2
 * ========================================================================== */

/* Токовая защита инвертора */
#define INV_STS2_FC_IT_ERR		(1 << 0)
/* Ошибка автонастройки */
#define INV_STS2_AST_ERR		(1 << 1)
/* Флаг срабатывания токоограничения - свободен */
#define INV_STS2_I_LIMIT_FAST		(1 << 2)
/* Оптимизация по току */
#define INV_STS2_CURRENT_OPT		(1 << 3)
/* Оптимизация по мощности */
#define INV_STS2_POWER_OPT		(1 << 4)
/* Оптимизация выполнена */
#define INV_STS2_OPT_DONE		(1 << 5)
/* Ошибка чтения флеш уставок */
#define INV_STS2_FLSH_RD_ERR		(1 << 6)
/* Ошибка записи флеш уставок */
#define INV_STS2_FLSH_WR_ERR		(1 << 7)
/* Включить разряд емкости (останов турбины для вентильного мотора) */
#define INV_STS2_DISCHARGE_ON		(1 << 8)
/* Идет разряд емкости  */
#define INV_STS2_DISCHARGE		(1 << 9)
/* Ошибка разряда емкости (останова турбины вентильного мотора) */
#define INV_STS2_DISCHARGE_ERR		(1 << 10)
/* Ошибка векторного алгоритма */
#define INV_STS2_VC_ERR			(1 << 11)
/* Быстрая токовая защита двигателя */
#define INV_STS2_M_IFAST_ERR		(1 << 12)
/* Токовая защита двигателя (I2t характеристика) */
#define INV_STS2_M_I2T_ERR		(1 << 13)
/* Защита ПЧ по токоограничению */
#define INV_STS2_I_LIM_ERR		(1 << 14)
/* Ошибка диапазона уставок флеш */
#define INV_STS2_FLSH_VAL_ERR		(1 << 15)

/* ==========================================================================
 * Регистр расширенного статуса инвертора IREG_INV_STATUS3, сохраняемый!
 * ========================================================================== */
/* Правое направление вращения */
#define INV_STS3_RIGHT_DIR			(1 << 0)
/* ОверШИМ тип 1 */
#define INV_STS3_OVERPWM1			(1 << 1)
/* ОверШИМ тип 2 */
#define INV_STS3_OVERPWM2			(1 << 2)
/* Тип двигателя */
#define INV_STS3_M_TYPE				(1 << 3)
/* Запрет вентильного двигателя */
#define INV_STS3_PMM_DIS			(1 << 4)
/* Тип ОС для подавления резонанса */
#define INV_STS3_RES_TYPE0			(1 << 5)
#define INV_STS3_RES_TYPE1			(1 << 6)
/* Режимы контроля связи с термодатчиками - "0" (все) или "1" (хоть один) */
#define INV_STS3_DT_SINGLE			(1 << 7)
/* Критический температурный режим */
#define INV_STS3_DT_CRITIC			(1 << 8)
/* Включение мониторов питания */
#define INV_STS3_PW_ERR_ON			(1 << 9)
/* Использовать выходную индуктивность Lx */
#define INV_STS3_LX_USE				(1 << 10)
/* Датчик тока на выходе СУ (после фильтра) */
#define INV_STS3_DT_SU				(1 << 11)
/* Включить разрешение сброса напряженя до нуля */
#define INV_STS3_ZERO_UDROP			(1 << 12)
/* Включен нормированный разгон - торможение */
#define INV_STS3_NORM_MV_FREF		(1 << 13)
/* Выключить гармонический анализ */
#define INV_STS3_HARM_CALC_OFF		(1 << 14)
/* Мощность из действ. значения */
#define INV_STS3_RMS_POWER			(1 << 15)

#endif // INVERTOR_H
