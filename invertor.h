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
 * Управляющий регистр IREG_INV_CTRL2
 * ========================================================================== */
/* Источник подавления резонанса - угол */
#define INV_CTRL2_RES_ANGLE		(1 << 0)
/* Источник подавления резонанса - момент */
#define INV_CTRL2_RES_TORQUE		(1 << 1)
/* Источник подавления резонанса - мощность */
#define INV_CTRL2_RES_POWER		(1 << 2)

/* Включить режим контроля температур - "1" (до последнего)
 * Для трех групп - тиристоров, IGBT и воздуха */
#define INV_CTRL2_DT_SINGLE_MODE	(1 << 3)
/* Режим контроля температур  - класть хер на все (что можно)
 * А можно пока только на ошибку термодатчика тиристоров(( */
#define INV_CTRL2_DT_CRITIC_MODE	(1 << 4)
/* Режим контроля температур - "0"(по всем) включить */
#define INV_CTRL2_DT_ALL_MODE		(1 << 5)

/* Включить/отключить контроль мониторов питания */
#define INV_CTRL2_PW_ERR_ON		(1 << 6)
#define INV_CTRL2_PW_ERR_OFF		(1 << 7)

/* Датчик тока на выходе фильтра */
#define INV_CTRL2_DT_SU_ON		(1 << 8)
/* Датчик тока на выходе ПЧ */
#define INV_CTRL2_DT_SU_OFF		(1 << 9)

/* Включить/отключить разрешение сброса до нуля */
#define INV_CTRL2_ZERO_UDROP_ON		(1 << 10)
#define INV_CTRL2_ZERO_UDROP_OFF	(1 << 11)

/* Включить/Отключить нормированный разгон - торможение */
#define INV_CTRL2_NORM_MV_FREF_ON	(1 << 12)
#define INV_CTRL2_NORM_MV_FREF_OFF	(1 << 13)

/* Использовать выходную индуктивность */
#define INV_CTRL2_LX_USE_ON		(1 << 14)
/* Использовать  индуктивность  */
#define INV_CTRL2_LX_USE_OFF		(1 << 15)

/* маска всех бит управляющего регистра CTRL2 */
#define INV_CTRL2_MASK		( \
        INV_CTRL2_RES_ANGLE		\
    |	INV_CTRL2_RES_TORQUE		\
    |	INV_CTRL2_RES_POWER		\
    |	INV_CTRL2_DT_SINGLE_MODE	\
    |	INV_CTRL2_DT_CRITIC_MODE	\
    |	INV_CTRL2_DT_ALL_MODE		\
    |	INV_CTRL2_PW_ERR_ON		\
    |	INV_CTRL2_PW_ERR_OFF		\
    |	INV_CTRL2_DT_SU_ON		\
    |	INV_CTRL2_DT_SU_OFF		\
    |	INV_CTRL2_ZERO_UDROP_ON		\
    |	INV_CTRL2_ZERO_UDROP_OFF	\
    |	INV_CTRL2_NORM_MV_FREF_ON	\
    |	INV_CTRL2_NORM_MV_FREF_OFF	\
    |	INV_CTRL2_LX_USE_ON	\
    |	INV_CTRL2_LX_USE_OFF	\
)

/* ==========================================================================
 * Управляющий регистр IREG_CTRL3
 * ========================================================================== */

/* Включить/выключить измерение мощности по действующим значениям. */
#define CTRL3_RMS_POWER_ON		(1 << 0)
#define CTRL3_RMS_POWER_OFF		(1 << 1)

/* Задать конфигурацию платы расширения */
#define CTRL3_EXP_CONF_SET		(1 << 2)

/* Младший бит установки конфигурации платы расширения */
#define CTRL3_EXP_CONF_LST_BIT	3			/* =3 */

#define CTRL3_EXP_CONF0		(1 << (CTRL3_EXP_CONF_LST_BIT + 0))
#define CTRL3_EXP_CONF1		(1 << (CTRL3_EXP_CONF_LST_BIT + 1))

/* Биты конфигурации платы расширения */
#define CTRL3_EXP_CONF_2b	(\
    CTRL3_EXP_CONF0 |	\
    CTRL3_EXP_CONF1		\
)

/* Варианты конфигурации платы расширения */
#define CTRL3_EXP_LEGACY		(0 * CTRL3_EXP_CONF0)
#define CTRL3_EXP_485			(1 * CTRL3_EXP_CONF0)
#define CTRL3_EXP_SPI			(2 * CTRL3_EXP_CONF0)
#define CTRL3_EXP_OTHERS		(3 * CTRL3_EXP_CONF0)
/* Задание конфигурации платы расширения */
#define CTRL3_EXP_LEGACY_SET	(CTRL3_EXP_CONF_SET | CTRL3_EXP_LEGACY)
#define CTRL3_EXP_485_SET		(CTRL3_EXP_CONF_SET | CTRL3_EXP_485)
#define CTRL3_EXP_SPI_SET		(CTRL3_EXP_CONF_SET | CTRL3_EXP_SPI)
#define CTRL3_EXP_OTHERS_SET	(CTRL3_EXP_CONF_SET | CTRL3_EXP_OTHERS)

/*
Устаревшие биты управления
#define INV_CTRL3_EXP_LEGACY		(1 << 2)
#define INV_CTRL3_EXP_485			(1 << 3)
#define INV_CTRL3_EXP_SPI			(1 << 4)
#define INV_CTRL3_EXP_OTHERS		(1 << 5)
 */

/* Задать конфигурацию ПЧ */
#define CTRL3_FC_CONF_SET		(1 << 5)		/* (1 << 5) */

/* Младший бит установки конфигурации ПЧ */
#define CTRL3_FC_CONF_LST_BIT	6
/* Установка конфигурации ПЧ */
#define CTRL3_FC_CONF_6b			(	\
    (1 << (CTRL3_FC_CONF_LST_BIT + 0)) | \
    (1 << (CTRL3_FC_CONF_LST_BIT + 1)) | \
    (1 << (CTRL3_FC_CONF_LST_BIT + 2)) | \
    (1 << (CTRL3_FC_CONF_LST_BIT + 3)) | \
    (1 << (CTRL3_FC_CONF_LST_BIT + 4)) | \
    (1 << (CTRL3_FC_CONF_LST_BIT + 5)) 	 \
)

/* Задать тип ПЧ */
#define CTRL3_FC_TYPE_SET		(1 << 12)

#define CTRL3_PARAMS_OUT_CHANGE  (1 << 15)

/* Младший бит установки типа ПЧ */
#define CTRL3_FC_TYPE_LST_BIT	13

#define CTRL3_FC_TYPE0		(1 << (CTRL3_FC_TYPE_LST_BIT + 0))
#define CTRL3_FC_TYPE1		(1 << (CTRL3_FC_TYPE_LST_BIT + 1))

/* Биты типа ПЧ */
#define CTRL3_FC_TYPE_2b	(		\
    CTRL3_FC_TYPE0 |	\
    CTRL3_FC_TYPE1		\
)
/* Варианты типа ПЧ */
#define CTRL3_FC_SUBPUMP		(0 * CTRL3_FC_TYPE0)
#define CTRL3_FC_TRAC			(1 * CTRL3_FC_TYPE0)

#define CTRL3_FC_SUBPUMP_SET	(CTRL3_FC_TYPE_SET | CTRL3_FC_SUBPUMP)
#define CTRL3_FC_TRAC_SET		(CTRL3_FC_TYPE_SET | CTRL3_FC_TRAC)

typedef enum {
    FC_CONF_BASE = 0,
    FC_CONF_X1_ISING_RSING,
    FC_CONF_X1_ISING_RDOUB,
    FC_CONF_X1_IDOUB_RSING,
    FC_CONF_X1_IDOUB_RDOUB,
    FC_CONF_X1_ITRIP_RSING,
    FC_CONF_X1_ITRIP_RDOUB,
    FC_CONF_X1_IQUAD_RSING,
    FC_CONF_X1_IQUAD_RDOUB,
    FC_CONF_X2_IQUAD_RSING,
    FC_CONF_X2_IQUAD_RDOUB,
    FC_CONFS_COUNT
} _fc_conf;


#define CTRL3_FC_BASE_SET	( CTRL3_FC_CONF_SET |	\
    (FC_CONF_BASE << CTRL3_FC_CONF_LST_BIT)	\
)
#define CTRL3_FC_X1_ISING_RSING_SET	( CTRL3_FC_CONF_SET |	\
    (FC_CONF_X1_ISING_RSING << CTRL3_FC_CONF_LST_BIT)	\
)
#define CTRL3_FC_X1_ISING_RDOUB_SET	( CTRL3_FC_CONF_SET |	\
    (FC_CONF_X1_ISING_RDOUB << CTRL3_FC_CONF_LST_BIT)	\
)
#define CTRL3_FC_X1_IDOUB_RSING_SET	( CTRL3_FC_CONF_SET |	\
    (FC_CONF_X1_IDOUB_RSING << CTRL3_FC_CONF_LST_BIT)	\
)
#define CTRL3_FC_X1_IDOUB_RDOUB_SET	( CTRL3_FC_CONF_SET |	\
    (FC_CONF_X1_IDOUB_RDOUB << CTRL3_FC_CONF_LST_BIT)	\
)
#define CTRL3_FC_X1_ITRIP_RSING_SET	( CTRL3_FC_CONF_SET |	\
    (FC_CONF_X1_ITRIP_RSING << CTRL3_FC_CONF_LST_BIT)	\
)
#define CTRL3_FC_X1_ITRIP_RDOUB_SET	( CTRL3_FC_CONF_SET |	\
    (FC_CONF_X1_ITRIP_RDOUB << CTRL3_FC_CONF_LST_BIT)	\
)
#define CTRL3_FC_X1_IQUAD_RSING_SET	( CTRL3_FC_CONF_SET |	\
    (FC_CONF_X1_IQUAD_RSING << CTRL3_FC_CONF_LST_BIT)	\
)
#define CTRL3_FC_X1_IQUAD_RDOUB_SET	( CTRL3_FC_CONF_SET |	\
    (FC_CONF_X1_IQUAD_RDOUB << CTRL3_FC_CONF_LST_BIT)	\
)
#define CTRL3_FC_X2_IQUAD_RSING_SET	( CTRL3_FC_CONF_SET |	\
    (FC_CONF_X2_IQUAD_RSING << CTRL3_FC_CONF_LST_BIT)	\
)
#define CTRL3_FC_X2_IQUAD_RDOUB_SET	( CTRL3_FC_CONF_SET |	\
    (FC_CONF_X2_IQUAD_RDOUB << CTRL3_FC_CONF_LST_BIT)	\
)

/* маска всех бит управляющего регистра CTRL3 */
#define CTRL3_MASK		( 		\
        CTRL3_RMS_POWER_ON		\
    |	CTRL3_RMS_POWER_OFF		\
    |	CTRL3_EXP_CONF_SET		\
    |	CTRL3_EXP_CONF_2b		\
    |	CTRL3_FC_CONF_SET		\
    |	CTRL3_FC_CONF_6b		\
    |	CTRL3_FC_TYPE_SET		\
    |	CTRL3_FC_TYPE_2b		\
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

/* Один бит зарезервирован под критичный флаг */
#define INV_STS_RECT_NO_CHARGE		(1 << 15)

/* Стоп биты связанные с остановкой извне */
#define INV_STS_EXT_STP_BITS (\
        INV_STS_STOPPED_REGISTERS	\
    |	INV_STS_STOPPED_EXTERNAL	\
    |	INV_STS_STOPPED_ALARM		\
)

/* Внутр. ошибки, по которым останавливается запись архива */
#define INV_STS_ERR_BITS (\
        INV_STS_FAULT_STOPPED		\
    |	INV_STS_UD_LOW_FAULT		\
    |	INV_STS_UD_HIGH_FAULT		\
    |	INV_STS_URECT_SHORTCIRCUIT	\
    |	INV_STS_RECT_NO_CHARGE		\
)

/* Стоп биты связанные с режимом остановки */
#define INV_STS_STP_BITS (\
    INV_STS_EXT_STP_BITS | INV_STS_ERR_BITS	\
)

#define INV_STS_MASK (\
        INV_STS_STARTED				\
    |	INV_STS_WAIT_RECT_START		\
    |	INV_STS_STOPPED_REGISTERS	\
    |	INV_STS_STOPPED_EXTERNAL	\
    |	INV_STS_WAIT_RECT_STOP		\
    |	INV_STS_FAULT_STOPPED		\
    |	INV_STS_RIGHT_DIR			\
    |	INV_STS_I_LIMIT				\
    |	INV_STS_ULOW				\
    |	INV_STS_STOPPED_ALARM		\
    |	INV_STS_UD_LOW_FAULT		\
    |	INV_STS_UD_HIGH_FAULT		\
    |	INV_STS_TO_STOP_MODE		\
    |	INV_STS_RUN					\
    |	INV_STS_URECT_SHORTCIRCUIT	\
    |	INV_STS_RECT_NO_CHARGE		\
)

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

/* Стоп биты расширенного регистра статуса */
#define INV_STS2_STP_BITS		(	\
        INV_STS2_FC_IT_ERR		\
    |	INV_STS2_M_I2T_ERR		\
    |	INV_STS2_M_IFAST_ERR		\
    |	INV_STS2_I_LIM_ERR		\
    |	INV_STS2_AST_ERR		\
    |	INV_STS2_VC_ERR			\
)
/* Ошибки, по которым останавливается запись архива */
#define INV_STS2_ERR_BITS		(	\
        INV_STS2_FC_IT_ERR		\
    |	INV_STS2_AST_ERR		\
    |	INV_STS2_M_I2T_ERR		\
    |	INV_STS2_M_IFAST_ERR		\
    |	INV_STS2_I_LIM_ERR		\
    |	INV_STS2_VC_ERR			\
)
/* Биты управления оптимизацией */
#define INV_STS2_OPT_BITS		(	\
        INV_STS2_CURRENT_OPT		\
    |	INV_STS2_POWER_OPT		\
)

#define INV_STS2_MASK		(		\
        INV_STS2_FC_IT_ERR		\
    |	INV_STS2_AST_ERR		\
    |	INV_STS2_I_LIMIT_FAST		\
    |	INV_STS2_CURRENT_OPT		\
    |	INV_STS2_POWER_OPT		\
    |	INV_STS2_OPT_DONE		\
    |	INV_STS2_FLSH_RD_ERR		\
    |	INV_STS2_FLSH_WR_ERR		\
    |	INV_STS2_DISCHARGE_ON		\
    |	INV_STS2_DISCHARGE		\
    |	INV_STS2_DISCHARGE_ERR		\
    |	INV_STS2_VC_ERR			\
    |	INV_STS2_M_IFAST_ERR		\
    |	INV_STS2_M_I2T_ERR		\
    |	INV_STS2_I_LIM_ERR		\
    |	INV_STS2_FLSH_VAL_ERR		\
)
/* Биты ошибок  флеш */
#define INV_STS2_FLSH_ERR_BITS		(	\
        INV_STS2_FLSH_RD_ERR	|	\
        INV_STS2_FLSH_WR_ERR	|	\
        INV_STS2_FLSH_VAL_ERR		\
)

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

/* Тип ШИМ */
/* установить тип ШИМ  в регистре статуса:
 * 0 - синус,
 * INV_STS3_OVERPWM1 - ОверШИМ1 (трапеция),
 * INV_STS3_OVERPWM2 - "шестиугольник"
 * (варьирование скоростью вектора). */
#define INV_STS3_PWM_TYPE	(		\
        INV_STS3_OVERPWM1			\
    |	INV_STS3_OVERPWM2			\
)

/* Варианты конфигурации типов ШИМ */
#define STS3_SINPWM         (0 * INV_STS3_OVERPWM1)
#define STS3_OVERPWM1       (1 * INV_STS3_OVERPWM1)
#define STS3_OVERPWM2       (2 * INV_STS3_OVERPWM1)
#define STS3_OVERPWM_RES    (3 * INV_STS3_OVERPWM1)

/* Тип подавления резонанса */
#define INV_STS3_RES_TYPE	(		\
        INV_STS3_RES_TYPE0			\
    |	INV_STS3_RES_TYPE1			\
)

/* Варианты конфигурации подавления резонанса */
#define STS3_RES_ANGLE		(0 * INV_STS3_RES_TYPE0)
#define STS3_RES_TORQUE		(1 * INV_STS3_RES_TYPE0)
#define STS3_RES_POWER		(2 * INV_STS3_RES_TYPE0)
#define STS3_RES_RESERVED	(3 * INV_STS3_RES_TYPE0)

#define INV_STS3_MASK		(		\
        INV_STS3_RIGHT_DIR		\
    |	INV_STS3_OVERPWM1			\
    |	INV_STS3_OVERPWM2			\
    |	INV_STS3_M_TYPE				\
    |	INV_STS3_PMM_DIS			\
    |	INV_STS3_RES_TYPE0			\
    |	INV_STS3_RES_TYPE1			\
    |	INV_STS3_DT_SINGLE			\
    |	INV_STS3_DT_CRITIC			\
    |	INV_STS3_PW_ERR_ON			\
    |	INV_STS3_LX_USE				\
    |	INV_STS3_DT_SU				\
    |	INV_STS3_ZERO_UDROP			\
    |	INV_STS3_NORM_MV_FREF		\
    |	INV_STS3_HARM_CALC_OFF		\
    |	INV_STS3_RMS_POWER			\
)

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

#define INV_FAULT_MASK		(\
        INV_FLT_IMAX		\
    |	INV_FLT_IZ			\
    |	INV_FLT_PAR_RSLV_MON		\
    |	INV_FLT_CTR_MON		\
    |	INV_FLT_CLK_MON		\
    |	INV_FLT_MB_MON		\
    |	INV_FLT_DR0		\
    |	INV_FLT_DR1		\
    |	INV_FLT_DR2		\
    |	INV_FLT_TEST		\
    |	INV_FLT_TEMP_LINK	\
    |	INV_FLT_TEMP		\
    |	INV_FLT_AIR_TEMP	\
    |	INV_FLT_ALARM_SW	\
    |	INV_FLT_TTEMP		\
    |	INV_FLT_REACT_ERR	\
)
#define FLT_NOT_STOPPED_MASK	(\
        INV_FLT_CLK_MON		\
    |	INV_FLT_TEST		\
)
    // |	INV_FLT_ALARM_SW	)

#define FLT_SMOOTH_STOPPED_MASK	(\
        INV_FLT_TEMP_LINK	\
    |	INV_FLT_TEMP		\
    |	INV_FLT_AIR_TEMP	\
    |	INV_FLT_TTEMP		\
    |	INV_FLT_REACT_ERR	\
)
#define FLT_INST_STOPPED_MASK	(INV_FAULT_MASK &	\
        (~(FLT_SMOOTH_STOPPED_MASK | FLT_NOT_STOPPED_MASK)))


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

/* Режим вывода параметров */
#define STS5_PARAMS_OUT_MODE  (1 << 11)

/* Управление частотой ШИМ */
#define STS5_PWM_CTRL      (1 << 12)

/* Маска регистра STS5 */
#define INV_STS5_MASK (	\
    STS5_EXP_CONF_2b |	\
    STS5_FC_CONF_6b	 |	\
    STS5_FC_TYPE_2b		\
)

/* ==========================================================================
 * Внутренний управляющий регистр контоллера IREG_INV_INT_CTRL
 * ========================================================================== */

/* Включить дискретный выход EXT0 */
#define INT_CTRL_SET_EXT_OUT0			(1 << 0)
/* Выключить дискретный выход EXT0 */
#define INT_CTRL_CLR_EXT_OUT0			(1 << 1)
/* Включить дискретный выход EXT0 */
#define INT_CTRL_SET_EXT_OUT1			(1 << 2)
/* Выключить дискретный выход EXT0 */
#define INT_CTRL_CLR_EXT_OUT1			(1 << 3)
/* Включить дискретный выход EXT2 */
#define INT_CTRL_SET_EXT_OUT2			(1 << 5)
/* Выключить дискретный выход EXT2 */
#define INT_CTRL_CLR_EXT_OUT2			(1 << 6)

/* Установить частоту CAN 125 кБит */
#define INT_CTRL_SET_BDRT_125			(1 << 7)
/* Установить частоту CAN 250 кБит */
#define INT_CTRL_SET_BDRT_250			(1 << 8)

/* Бит на переключение управления ШИМ на ходу */
#define INT_CTRL_PWM_CTRL_CHANGE			(1 << 9)

/* Произошел ресет прокси по ошибке CAN */
#define INT_CTRL_PROXY_CANERR_RST		(1 << 10)
/* Произошел авторесет прокси по потере связи */
#define INT_CTRL_PROXY_LINK_RST			(1 << 11)
/* Сброс битов ошибок флеш */
#define INT_CTRL_FLSH_ERR_CLR			(1 << 12)
/* Записать всю флеш */
#define INT_CTRL_FLSH_WR_ALL			(1 << 13)
/* Разрешить ВД */
#define INT_CTRL_PMM_ON					(1 << 14)
/* Запретить ВД */
#define INT_CTRL_PMM_OFF				(1 << 15)

/* маска всех битов внутр. управляющего регистра */
#define INT_CTRL_MASK		(	\
        INT_CTRL_SET_EXT_OUT0				\
    |	INT_CTRL_CLR_EXT_OUT0				\
    |	INT_CTRL_SET_EXT_OUT1				\
    |	INT_CTRL_CLR_EXT_OUT1				\
    |	INT_CTRL_SET_EXT_OUT2				\
    |	INT_CTRL_CLR_EXT_OUT2				\
    |	INT_CTRL_SET_BDRT_125			\
    |	INT_CTRL_PROXY_CANERR_RST	\
    |	INT_CTRL_PROXY_EXT_RST		\
    |	INT_CTRL_SET_BDRT_250		\
    |	INT_CTRL_PROXY_LINK_RST		\
    |	INT_CTRL_FLSH_ERR_CLR		\
    |	INT_CTRL_FLSH_WR_ALL			\
    |	INT_CTRL_PMM_ON				\
    |	INT_CTRL_PMM_OFF				\
)


#endif // INVERTOR_H
