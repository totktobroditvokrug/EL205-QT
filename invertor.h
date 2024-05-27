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


/* ==========================================================================
 * Управляющий регистр IREG_INV_CONTROL
 * ========================================================================== */

/* команда "запустить" */
#define INV_CTRL_START		(1 << 0)

/* команда "остановить" */
#define INV_CTRL_STOP		(1 << 1)

/* Направление (Реверс) */
#define INV_CTRL_LEFT_DIRECTION	(1 << 2)
#define INV_CTRL_RIGHT_DIRECTION	(1 << 3)

/* аварийный стоп */
#define INV_CTRL_ALARM		(1 << 4)

/* Биты управления оптимизацией */
#define INV_CTRL_CURRENT_OPT		(1 << 5)
#define INV_CTRL_POWER_OPT		(1 << 6)
#define INV_CTRL_CLEAR_OPT		(1 << 7)

/* Установка типа мотора */
#define INV_CTRL_ASYN_MOTOR		(1 << 8)
#define INV_CTRL_VENT_MOTOR		(1 << 9)

/* Установка типа ШИМ */
#define INV_CTRL_OVERPWM1_ON		(1 << 10)
#define INV_CTRL_OVERPWM2_ON		(1 << 11)
#define INV_CTRL_OVERPWM_OFF		(1 << 12)

/* Установка режима предразряда шины */
#define INV_CTRL_HARM_CALC_ON	(1 << 13)
#define INV_CTRL_HARM_CALC_OFF	(1 << 14)
/* Сброс аппаратных ошибок */
#define INV_CTRL_FLT_CLR		(1 << 15)

/* маска всех бит управляющего регистра */
#define INV_CTRL_MASK		(	\
        INV_CTRL_START		\
    |	INV_CTRL_STOP		\
    |	INV_CTRL_LEFT_DIRECTION	\
    |	INV_CTRL_RIGHT_DIRECTION	\
    |	INV_CTRL_ALARM		\
    |	INV_CTRL_CURRENT_OPT		\
    |	INV_CTRL_POWER_OPT		\
    |	INV_CTRL_CLEAR_OPT		\
    |	INV_CTRL_ASYN_MOTOR		\
    |	INV_CTRL_VENT_MOTOR		\
    |	INV_CTRL_OVERPWM1_ON		\
    |	INV_CTRL_OVERPWM2_ON		\
    |	INV_CTRL_OVERPWM_OFF		\
    |	INV_CTRL_HARM_CALC_ON	\
    |	INV_CTRL_HARM_CALC_OFF	\
    |	INV_CTRL_FLT_CLR		\
)


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


/* ==========================================================================
 * Регистр ошибок инвертора IREG_INV_FAULT
 * ========================================================================== */
/* Предел тока инвертора */
#define INV_FLT_IMAX		(1 << 0)
/* Ток утечки фаз инвертора */
#define INV_FLT_IZ			(1 << 1)
/* Монитор параллельных ветвей инвертора, ресольвера */
#define INV_FLT_PAR_RSLV_MON	(1 << 2)
/* Монитор питания цифровой цепи контроллера */
#define INV_FLT_CTR_MON		(1 << 3)
/* Перегрев реактора, новый */
#define INV_FLT_CLK_MON		(1 << 4)
/* Монитор входного питания источника */
#define INV_FLT_MB_MON		(1 << 5)
/* Аппаратные защиты драйверов 0, 1, 2 */
#define INV_FLT_DR0		(1 << 6)
#define INV_FLT_DR1		(1 << 7)
#define INV_FLT_DR2		(1 << 8)
/* Тестовый режим инвертора */
#define INV_FLT_TEST		(1 << 9)
/* Ошибка связи с термодатчиками */
#define INV_FLT_TEMP_LINK	(1 << 10)
/* Тепловая защита инвертора */
#define INV_FLT_TEMP		(1 << 11)
/* Тепловая защита по воздуху */
#define INV_FLT_AIR_TEMP	(1 << 12)
/* Аварийная кнопка */
#define INV_FLT_ALARM_SW	(1 << 13)
/* Тепловая защита выпрямителя */
#define INV_FLT_TTEMP		(1 << 14)
/* Перегрев реактора, старый */
#define INV_FLT_REACT_ERR	(1 << 15)

/* ==========================================================================
 * Регистр расширенного статуса инвертора IREG_INV_STATUS5, сохраняемый!
 * ========================================================================== */

/* Младший бит конфигурации платы расширения */
#define STS5_EXP_CONF_LST_BIT	0
/* Биты конфигурации платы расширения */
#define STS5_EXP_CONF0			(1 << (STS5_EXP_CONF_LST_BIT + 0))
#define STS5_EXP_CONF1			(1 << (STS5_EXP_CONF_LST_BIT + 1))
/* Конфигурация платы расширения */
#define STS5_EXP_CONF_2b (	\
    STS5_EXP_CONF0 |	\
    STS5_EXP_CONF1		\
)
/* Варианты конфигурации платы расширения */			/* EXP_CONF1:EXP_CONF0 */
#define STS5_EXP_LEGACY		(0 * STS5_EXP_CONF0)			/* 0:0 */
#define STS5_EXP_485		(1 * STS5_EXP_CONF0)			/* 0:1 */
#define STS5_EXP_SPI		(2 * STS5_EXP_CONF0)			/* 1:0 */
#define STS5_EXP_OTHERS		(3 * STS5_EXP_CONF0)			/* 1:1 */

/* Младший бит конфигурации ПЧ */
#define STS5_FC_CONF_LST_BIT	2
/* Биты конфигурации ПЧ */
#define STS5_FC_CONF0	(1 << (STS5_FC_CONF_LST_BIT + 0))
#define STS5_FC_CONF1	(1 << (STS5_FC_CONF_LST_BIT + 1))
#define STS5_FC_CONF2	(1 << (STS5_FC_CONF_LST_BIT + 2))
#define STS5_FC_CONF3	(1 << (STS5_FC_CONF_LST_BIT + 3))
#define STS5_FC_CONF4	(1 << (STS5_FC_CONF_LST_BIT + 4))
#define STS5_FC_CONF5	(1 << (STS5_FC_CONF_LST_BIT + 5))
/* Конфигурация ПЧ */
#define STS5_FC_CONF_6b (\
    STS5_FC_CONF0 | \
    STS5_FC_CONF1 | \
    STS5_FC_CONF2 | \
    STS5_FC_CONF3 | \
    STS5_FC_CONF4 | \
    STS5_FC_CONF5 \
)

/* Младший бит типа ПЧ */
#define STS5_FC_TYPE_LST_BIT		8
/* Биты типа ПЧ */
#define STS5_FC_TYPE0	(1 << (STS5_FC_TYPE_LST_BIT + 0))
#define STS5_FC_TYPE1	(1 << (STS5_FC_TYPE_LST_BIT + 1))
/* Тип ПЧ */
#define STS5_FC_TYPE_2b (	\
    STS5_FC_TYPE0 |	\
    STS5_FC_TYPE1		\
)
/* Варианты типов ПЧ */
#define STS5_FC_SUBPUMP		(0 * STS5_FC_TYPE0)
#define STS5_FC_TRAC		(1 * STS5_FC_TYPE0)

/* Частота CAN шины */
#define STS5_CAN_BDRT_250		(1 << 10)

/* Маска регистра STS5 */
#define INV_STS5_MASK (	\
    STS5_EXP_CONF_2b |	\
    STS5_FC_CONF_6b	 |	\
    STS5_FC_TYPE_2b		\
)

#endif // INVERTOR_H
