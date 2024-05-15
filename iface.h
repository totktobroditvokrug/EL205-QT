#ifndef IFACE_H
#define IFACE_H

#include <QObject>
#include <QMetaEnum>

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QList>

#include <QVector>

// Установка регистра
#define REGISTER_SET_ID    10
//Передача содержимого регистра
#define REGISTER_SHOW_ID  20
// передача масштабов к регистру
#define REGISTER_SHOW_SCALES_ID 30
// запрос содержимого регистра
#define REGISTER_REQUEST_ID 40
// измерения
#define SAMPLE_ID 100


//--------- флаги регистров ---------
#define IREGF_SCALE_PRESENT	(1 << 0)
#define IREGF_ZERO_PRESENT	(1 << 1)
#define IREGF_MAXVAL_PRESENT	(1 << 2)
/* масштаб, ноль или максимум есть */
#define IREGF_SCALES_PRESENT \
    (IREGF_SCALE_PRESENT | IREGF_ZERO_PRESENT | IREGF_MAXVAL_PRESENT)

/* минимум есть (иначе минимум - ноль) */
#define IREGF_MIN_PRESENT	(1 << 3)
/* максимум есть ( иначе максимум - 1000 ) */
#define IREGF_MAX_PRESENT	(1 << 4)
/* Есть минимум и максимум */
#define IREGF_MINMAX_PRESENT	(IREGF_MIN_PRESENT | IREGF_MAX_PRESENT)


/* Маска представлена */
#define IREGF_MASK_PRESENT	(1 << 5)

/* Данный регистр содержит статус */
#define IREGF_STATUS		(1 << 6)

/* регистр не может быть изменен (чисто информационный) */
#define IREGF_READONLY		(1 << 7)


#ifndef DEF_CAN_SAMPLE
#define DEF_CAN_SAMPLE(name, desc)			name,
#endif

#ifndef DEF_CAN_SAMPLE_CONST
#define DEF_CAN_SAMPLE_CONST(name, desc, value)		name = value,
#endif

#ifndef DEF_CAN_CONST
#define DEF_CAN_CONST(name, value)			name = value,
#endif

#ifndef DEF_CAN_CONST_NAME
#define DEF_CAN_CONST_NAME(name)			name,
#endif

#ifndef DEF_CAN_ENUM
#define DEF_CAN_ENUM		typedef enum
#endif

// максимальный размер буфера осциллограммы
const int PLOT_MAX_SIZE_ARR = 250;

class RegnumClass : public QObject
{
 Q_OBJECT
 Q_ENUMS(regnum)
public:

    enum regnum {

        /* ================ регистры выпрямителя =================== */
        /* начало регистров выпрямителя */
        IREG_THYR_START_REGISTERS	= 10,
        /* ================ регистры выпрямителя =================== */

        /* рабочий угол выпрямителя */
        IREG_THYR_ANGLEREF = IREG_THYR_START_REGISTERS,
        /* состояние выпрямителя */
        IREG_THYR_STATUS,

        /* Регистр управления тиристорным регулятором */
        IREG_THYR_CONTROL,

        /* Напряжение ошибки КЗ */
        IREG_THYR_USHORTCIRCUIT,

        /* конец регистров выпрямителя */
        IREG_THYR_END_REGISTERS,

        /* ================= регистры индикатора ==================== */
        IREG_IND_START_REGISTERS	= 20,

        /* регистр статуса индикатора */
        IREG_IND_STATUS	= IREG_IND_START_REGISTERS,
        /* Программы управления */
        IREG_IND_TEST_MOD_0,
        /* Лимит по частоте регулирования */
        IREG_IND_TEST_MOD_1,
        /* Лимит по моменту */
        IREG_IND_TEST_MOD_2,
        IREG_IND_TEST_MOD_3,
        IREG_IND_TEST0,
        IREG_IND_TEST1,
        IREG_IND_TEST2,
        IREG_IND_TEST3,
        IREG_IND_CTRL,
        /* конец регистров индикатора */
        IREG_IND_END_REGISTERS,


        /* ================ регистры платы измерений ================ */
        IREG_MEAS_START_REGISTERS	= 30,

        /* состояние выходов управления */
        IREG_MEAS_OUTPUTS	= IREG_MEAS_START_REGISTERS,

        /* состояние входов */
        IREG_MEAS_INPUTS,

        /* регистр включения выходов управления */
        IREG_MEAS_OUTPUT_ENABLE,

        /* регистр выключения выходов управления */
        IREG_MEAS_OUTPUT_DISABLE,

        /* Регистры включения-выключения вентилятора 1 */
        IREG_MEAS_VENT1_ON,
        IREG_MEAS_VENT1_OFF,
        IREG_MEAS_VENT1_SCAN_PERIOD,

        /* Температура при которой срабатывает защита */
        IREG_MEAS_TFAULT,
        /* Частота MODBUS 0х0026 */
        IREG_MEAS_BAUDRATE,
        /* Температуры вкл. и выкл. вент по дросселям фильтра - введена
         * фиксированная дельта, темп. выкл. не задействована */
        IREG_MEAS_VENTL_ON,
        IREG_MEAS_TEST_VAL,
        IREG_MEAS_PREHEAT_ON,
        IREG_MEAS_CTRL,
        IREG_MEAS_STATUS,
        IREG_MEAS_STS_ON,
        IREG_MEAS_STS_OFF,

        IREG_MEAS_END_REGISTERS,

        /* ================ регистры драйверов ==================== */

        IREG_DRIVERS_START_REGISTERS = 50,
        IREG_DRIVER0_STATUS = IREG_DRIVERS_START_REGISTERS,
        IREG_DRIVER1_STATUS,
        IREG_DRIVER2_STATUS,
        IREG_DRIVER3_STATUS,

        IREG_DRIVER0_CONTROL,
        IREG_DRIVER1_CONTROL,
        IREG_DRIVER2_CONTROL,
        IREG_DRIVER3_CONTROL,

        IREG_DRIVER0_FLT_LVL,
        IREG_DRIVER0_DIF_ERR,
        IREG_DRIVER1_FLT_LVL,
        IREG_DRIVER1_DIF_ERR,
        IREG_DRIVER2_FLT_LVL,
        IREG_DRIVER2_DIF_ERR,
        IREG_DRIVER3_FLT_LVL,
        IREG_DRIVER3_DIF_ERR,
        // IREG_INV_UPPER_0,
        // IREG_INV_UPPER_1,
        // IREG_INV_UPPER_2,
        // IREG_INV_UPPER_3,
        // IREG_INV_UPPER_4,
        // IREG_INV_UPPER_5,
        // IREG_INV_UPPER_6,
        // IREG_INV_UPPER_7,
        // IREG_INV_UPPER_8,
        // IREG_INV_UPPER_9,
        // IREG_INV_UPPER_10,
        // IREG_INV_UPPER_11,
        // IREG_INV_UPPER_12,
        // IREG_INV_UPPER_13,
        // IREG_INV_UPPER_14,
        // IREG_INV_UPPER_15,
        // IREG_INV_UPPER_16,
        // IREG_INV_UPPER_17,
        // IREG_INV_UPPER_18,
        // IREG_INV_UPPER_19,
        // IREG_INV_UPPER_20,
        // IREG_INV_UPPER_21,
        // IREG_INV_UPPER_22,
        // IREG_INV_UPPER_23,

        IREG_DRIVERS_END_REGISTERS,

    /*================== Регистры инвертора =====================================*/
        /* начало регистров инвертора */
        IREG_INV_START_REGISTERS	= 90,


    /*============= Внешние регистры инвертора, доступны через MODBUS ===========*/

        /* Управляющие регистры */
        IREG_INV_CONTROL	= IREG_INV_START_REGISTERS,
        IREG_INV_CTRL2,

        /* Регистр статуса (1) инвертора */
        IREG_INV_STATUS,
        /* Регистр статуса "2" инвертора */
        IREG_INV_STATUS2,
        /* Регистр статуса "3" инвертора, сохраняемый. */
        IREG_INV_STATUS3,
        /* Регистр ошибок инвертора */
        IREG_INV_FAULT,


        /* частота уставки после включения, тиков отображения */
        IREG_FREQ_REF,
        /* текущая частота вращения, тиков отображения */
        IREG_FREQ,
        /* Стартовая частота инвертора по умолчанию, тиков отображения */
        IREG_START_FREQ,
        /* Частота до которой скидывает ее автомат, Гц */
        IREG_FREQ_ERROR,
        /* максимум частоты уставки, Гц */
        IREG_FREQ_REF_MAX,

        /* период нарастания частоты */
        IREG_T_SPEEDUP,

        /* период снижения частоты */
        IREG_T_SPEEDDOWN,

        /* период снижения частоты при токоограничении */
        IREG_T_ILIMIT_SPEEDDOWN,

        /* период снижения частоты при токоограничении */
        IREG_T_ULOW_SPEEDDOWN,

        /* Номер текущего регулятора в очереди */
        IREG_CURRENT_REGULATOR,
        /* Регистры, определяющие последовательность старта */
        IREG_REGULATOR_QUEUE1,
        IREG_REGULATOR_QUEUE2,
        IREG_REGULATOR_QUEUE3,

        /* Компенсация напряжения по ШИМ */
        IREG_UOUT_PWM_COMP,
        /* Задержка выходного напряжения */
        IREG_U_OUT_OFST,

        /* Регистры защит ПЧ */
        IREG_FC_IRMS,
        IREG_FC_OVERLOAD,
        IREG_FC_T_OVERLOAD,
        /* Параметры выходного синус-фильтра */
        IREG_LF,
        IREG_CF,
        /* Коэффициенты масштабирования измерений ПЧ */
        IREG_FC_U_SCALE,
        IREG_FC_I_SCALE,

        /* Регистры защит по напряжению */
        IREG_UD_LOW_FAULT,
        IREG_UD_HIGH_FAULT,
        /* Регистр управления "3" инвертора */
        IREG_CTRL3,
        /* Регистр статуса "5" инвертора, сохраняемый. */
        IREG_INV_STATUS5,


    /*======================= Параметры ШИМ =====================================*/
        /* Частота ШИМ 33 */
        IREG_FREQ_PWM,
        /* Максимальный угол в режиме OverPWM2 */
        IREG_OVERPWM2_ANGLE,
        /* Степень постоянной времени отработки OverPWM */
        IREG_OVERPWM_GAIN,

    /*--------------------- Конец параметров ШИМ --------------------------------*/

    /*=================== Параметры двигателя (нагрузки)=========================*/

        /* Номинальная частота вращения двигателя 36 */
        IREG_M_F_NOM,
        /* Номинальное напряжение двигателя */
        IREG_M_U_NOM,
        /* Номинальный ток двигателя */
        IREG_M_I_NOM,
        /* Суммарная выходная индуктивность */
        IREG_LOUT_S,
        /* Параметры тепловой защиты двигателя */
        IREG_M_IRMS,		/* Перегруз по току двигателя */
        IREG_M_TRMS,		/* Время перегруза двигателя */
        /* Защита двигателя по току быстрая */
        IREG_M_I_FAST,
        /* Время отключения тепловой защиты двигателя при старте */
        IREG_T_BLANK,

    /*------------- Конец параметров двигателя (нагрузки)------------------------*/

    /*================= Характеристика U(f), I(f) ==============================*/
        IREG_UF_U0,
        IREG_IF_I0,
        IREG_UF_F1,
        IREG_UF_U1,
        IREG_IF_I1,
        IREG_UF_F2,
        IREG_UF_U2,
        IREG_IF_I2,
        IREG_UF_F3,
        IREG_UF_U3,
        IREG_IF_I3,
        IREG_UF_F4,
        IREG_UF_U4,
        IREG_IF_I4,
        IREG_UF_F5,
        IREG_UF_U5,
        IREG_IF_I5,
        IREG_UF_F6,
        IREG_UF_U6,
        IREG_IF_I6,
        /* коэфф. ОС компенсации по току */
        IREG_IFB_COMP,
    /*--------------- Конец характеристики U(f), I(f) ---------------------------*/

    /* ==================== Векторный регулятор ============================== */

        /* частота перехода на вектор 65 */
        IREG_F_VECT,
        /* Пропорциональная ОС векторного управления по фазе */
        /* Коэфф-т демпфирования момента для регулятора момента АД. */
        IREG_VFREQ_GAIN,
        /* Пропорциональная ОС векторного управления по фазе */
        /* Коэфф-т компенсации ускорения для регулятора момента АД */
        IREG_VFREQ_INTEG,
        /* Пропорциональная ОС векторного управления по частоте */
        IREG_VUOUT_PROP,
        /* Интегральная ОС векторного управления по частоте */
        IREG_VUOUT_INTEG,
        /* Постоянная времени перехода с вектора на U/f */
        /* Задержка времени измерения тока */
        IREG_VTOUF_INTEG,
        IREG_MAX_VC_DF,

    /* --------------- Конец векторного регулятора -----------------------------*/

    /* ==================== Подавление резонанса ===============================*/

        /* ОС подавления резонанса пропорциональная */
        IREG_RES_PROP_GAIN,
        /* ОС подавления резонанса интегральная */
        IREG_RES_INT_GAIN,
        /* Пусковая частота антирезонанса */
        IREG_RES_START_FREQ,

    /*------------------ Конец подавления резонанса -----------------------------*/

    /*======================== Пусковой режим ===================================*/

        /* частота вращения в пусковом режиме, тиков отображения */
        IREG_STARTUP_FREQ,
        /* угол качания */
        IREG_STARTUP_ANGLE_OSC,
        /* Количество качаний до перехода */
        IREG_STARTUP_OSC_COUNT,
        /* Количество оборотов */
        IREG_STARTUP_ROTATIONS,
        /* Напряжение импульса в стартапе */
        IREG_STARTUP_KU_PULSE,
        /* Токоограничение пускового режима */
        IREG_STARTUP_I_LIM,
        /* Токоограничение импульса расклинки пускового режима */
        IREG_STARTUP_I_LIM_PULSE,

    /*--------------------- Конец пускового режима ------------------------------*/

    /*================== Торможение турбинного вращения =========================*/

        /* Стартовая частота при торможении  авторотации 82 */
        IREG_DECEL_F_MAX,
        /* Стартовое напряжение при торможении авторотации */
        IREG_DECEL_U_MAX,
        /* частота форсирования */
        IREG_F_FORCE,
        /* напряжение форсирования */
        IREG_U_FORCE,
        /* Период снижения частоты при торможении авторотации */
        IREG_DECEL_SPEEDDOWN,
        /* Временно - измеренная выходная индуктивность с учетом COUT */
        IREG_LOUT,

    /*-------------------- Конец торможения турбины -----------------------------*/

    /*==================== Автонастройка при старте =============================*/

        /* Измерительное напряжение */
        IREG_AST_UMEAS,
        /* Измерительный ток */
        IREG_AST_IMEAS,

    /*---------------- Конец автонастройки при старте ---------------------------*/

    /*===================== Отображаемые параметры ==============================*/

        /* Напряжение DC шины, усредненное 90 */
        IREG_UD,
        /* Расчетное выходное напряжение */
        IREG_UOUT,
        /* Выходные токи - будут реализованы позже */
        IREG_IA_RMS,
        IREG_IB_RMS,
        IREG_IC_RMS,
        IREG_POUT_FAST,
        /* ЭДС двигателя минимальная (при Мout=0) */
        IREG_M_EDS_MIN,
        /* Расчетный момент двигателя */
        IREG_MOUT,
        /* Параметры из гармонического анализатора */
        IREG_IOUT,
        IREG_PHOUT,
        IREG_POUT,
        IREG_SOUT,
        /* Амплитуды и фазы выходных токов */
        IREG_IA,
        IREG_PHA,
        IREG_IB,
        IREG_PHB,
        IREG_IC,
        IREG_PHC,
    /*------------------ Конец отображаемых параметров --------------------------*/

        IREG_M_P_MAX,
        IREG_TORQUE_FB_COMP,
        IREG_LM,

        IREG_M_KI_START,
        IREG_FC_DI_LEVEL,
        IREG_M_EDS,
        IREG_LINK_MNTR,
        IREG_IRMS_T,
        /* Регистр статуса "4" инвертора, диагностический. */
        IREG_INV_STATUS4,

    /*======================== Тепловые защиты ==================================*/
        IREG_INV_T_FLT,
        IREG_INV_TT_FLT,
        IREG_INV_TAIR_FLT,
    /*----------------------- Конец тепловых защит ------------------------------*/

        IREG_RM,
        IREG_M_KU_START,
        IREG_I_LIM_MAX_T,
        IREG_I_LIM_ADD,
        IREG_DECEL_I_LIM,
        IREG_ROUT,
        IREG_ZP,
    /*---------------- конец внешних регистров инвертора ------------------------*/

    /*=========== Внутренние регистры ПЧ, доступны только по CAN! ===============*/

    /* Внимание! первый внутренний регистр обязан быть IREG_ZP
     * Добавил один регистр, закоммитить из проекта meas-2017. Можно еще один,
     * но не нужно впритык. */

        IREG_LR,
        IREG_RR,
        IREG_FLSH_RD_ERR_CNT,
        IREG_FRAME_LEN,

        IREG_TEST0,
        IREG_TEST1,
        IREG_TEST2,
        IREG_TEST3,
        IREG_TEST4,
        IREG_TEST5,
        IREG_SA_CONST_INT,
        IREG_SA_PHASE_INT,
    /*================ Прокси регистры платы измерений ========================*/
        IREG_SAVED_M_OUTPUTS,
        IREG_SAVED_M_INPUTS,
        IREG_SAVED_M_OUT_EN,
        IREG_SAVED_M_OUT_DIS,
        IREG_SAVED_M_CTRL,
        IREG_SAVED_M_V1_SCAN_PD,
        IREG_SAVED_M_BAUDRATE,
    /*-------------- Конец прокси регистров платы измерений -------------------*/
        IREG_TEST_DIGITAL,
        /* Тестовый аналоговый сигнал для отладки ПЧ 147 */
        IREG_TEST_ANALOG,
        /* Статус прокси сохраненный */
        IREG_SAVED_MEAS_STATUS,
        /* Частота уставки в Гц */
        IREG_FREQ_REF_HZ,
        /* Регистр управления режимами отладки ПЧ */
        IREG_TEST_TYPE,

        /* Порог тока, ниже которого чувствительность ОС по току
         * снижается. Пока сделан в исследовательских целях */
        IREG_RES_IAMP_VALID,
        /* Ранее изм индуктивность при автонастройке */
        /* Номер точки граничной частоты U(f) */
        IREG_THU_F_NUM,
        /* угол перехода к следующему шагу */
        IREG_STARTUP_ANGLE_SHIFT,
        /* Номинальное скольжение, Гц << FREQ_RT2 */
        IREG_M_DELTA_FREQ,
        /* Было напряжение разряда шины, не используется -
         * теперь критическая температура */
        IREG_TT_CRITIC,
        IREG_INV_INT_CTRL,
        IREG_INV_INT_STATUS,
        /* Предельная добавка угла подавления резонанса, х0,1, град. */
        IREG_SA_LIMIT,
        /* Коэф-т усиления выхода ЦАП (степень двойки) */
        IREG_DAC_GAIN_RT2,
        /* Фактические измерительные напряжение и ток при замере Lout */
        IREG_M_TORQUE_SET,
        IREG_K_LM_D,
        IREG_I_LIM_QUANTUM,
        IREG_SAVED_VENT1_ON,
        IREG_SAVED_VENT1_OFF,
        IREG_SAVED_VENTL_ON,
        IREG_SAVED_PREHEAT_ON,
    /* Достигли максимального количества регистров CAN */
        IREG_INV_ALL_END_REGISTERS
    };

static QStringList regnumList(void);
static QVector<QString> regnumArray(void);

private:
 RegnumClass() = delete;

};

class SampleCanIdClass : public QObject
{
 Q_OBJECT
 Q_ENUMS(fccanid)
public:

    /* типы сообщений по CAN-шине */
    enum fccanid{

        /* ===================== измерения, передаваемые по CAN ======== */
        DEF_CAN_CONST(CAN_START_SAMPLE_ID, 100)

        /* Измерения передаваемые тиристорным регулятором */
        DEF_CAN_CONST(CAN_THYR_SAMPLES, CAN_START_SAMPLE_ID)

        /* тиристорный регулятор */
        DEF_CAN_SAMPLE_CONST(
            CAN_THYR_UDC, /* =100 */ "Напряжение выходе выпрямителя",
            CAN_THYR_SAMPLES
        )
        DEF_CAN_SAMPLE(CAN_THYR_DR0_TT1, "Термодатчик тиристора 1 -> в dr-01.0")
        DEF_CAN_SAMPLE(CAN_THYR_TCPU, "Термодатчик CPU платы выпрямителя")
        DEF_CAN_SAMPLE(CAN_THYR_Uu, "Напряжение фазы U сети")
        DEF_CAN_SAMPLE(CAN_THYR_Uv, "Напряжение фазы V сети")
        DEF_CAN_SAMPLE(CAN_THYR_Uw, "Напряжение фазы W сети")
        DEF_CAN_SAMPLE(CAN_THYR_DR0_TT2, "Термодатчик тиристора 2 -> в dr-01.0")
        DEF_CAN_SAMPLE(CAN_THYR_T3, "Внешний термодатчик платы выпр. rc-05")
        DEF_CAN_SAMPLE(CAN_THYR_TAIR, "Термодатчик воздуха платы выпр. rc-05")
        DEF_CAN_SAMPLE(CAN_THYR_DR1_TT1, "Термодатчик тиристора 1 -> в dr-01.1")
        DEF_CAN_SAMPLE(CAN_THYR_DR1_TT2, "Термодатчик тиристора 2 -> в dr-01.1")
        DEF_CAN_CONST_NAME(CAN_THYR_SAMPLES_END) /* =111 */
        #define CAN_THYR_SAMPLES_COUNT (CAN_THYR_SAMPLES_END - CAN_THYR_SAMPLES)
        /* --------------------------------------------------------------*/

        /* измерения передаваемые индикатором */
        DEF_CAN_CONST(CAN_IND_SAMPLES, CAN_THYR_SAMPLES + 40)
        DEF_CAN_SAMPLE_CONST(
            CAN_IND_ADC1, /* =140 */ "Аналоговый вход 1",
            CAN_IND_SAMPLES
        )
        DEF_CAN_CONST_NAME(CAN_IND_SAMPLES_END) /* =111 */
        #define CAN_IND_SAMPLES_COUNT (CAN_IND_SAMPLES_END - CAN_IND_SAMPLES)
        /* --------------------------------------------------------------*/

        /* измерения передаваемые платой измерений */
        DEF_CAN_CONST(CAN_MEASURE_SAMPLES, CAN_IND_SAMPLES + 10)

        /* температуры с платы измерений */
        DEF_CAN_SAMPLE_CONST(
            CAN_MEAS_T1, /* =150 */ "T1 платы измерений",
            CAN_MEASURE_SAMPLES
        )
        DEF_CAN_SAMPLE(CAN_MEAS_T2, "T2 платы измерений")
        DEF_CAN_SAMPLE(CAN_MEAS_T3, "T3 платы измерений")
        DEF_CAN_SAMPLE(CAN_MEAS_T4, "T4 платы измерений")
        DEF_CAN_SAMPLE(CAN_MEAS_T5, "T5 платы измерений")
        DEF_CAN_SAMPLE(CAN_MEAS_T6, "T6 платы измерений")
        DEF_CAN_SAMPLE(CAN_MEAS_T7, "T7 платы измерений")
        DEF_CAN_CONST_NAME(CAN_MEASURE_SAMPLES_END) /* =157 */

        #define CAN_MEASURE_SAMPLES_COUNT \
            (CAN_MEASURE_SAMPLES_END - CAN_MEASURE_SAMPLES)
        /* --------------------------------------------------------------*/

        /* Измерения, передаваемые драйверами */
        DEF_CAN_CONST(CAN_DRIVERS_SAMPLES, CAN_MEASURE_SAMPLES + 50)

        DEF_CAN_SAMPLE_CONST(
            CAN_DR0_TAIR, /* =200 */ "Термодатчик воздуха драйвера 0",
            CAN_DRIVERS_SAMPLES
        )
        DEF_CAN_SAMPLE(CAN_DR0_TCPU, "Температура CPU драйвера 0")
        DEF_CAN_SAMPLE(CAN_DR0_PH0_T1, "Термодатчик фазы 0 драйвера 0")
        DEF_CAN_SAMPLE(CAN_DR0_USRC, "Напряжение питания драйвера 0")

        DEF_CAN_SAMPLE(CAN_DR1_TAIR, "Термодатчик воздуха драйвера 1")
        DEF_CAN_SAMPLE(CAN_DR1_TCPU, "Температура CPU драйвера 1")
        DEF_CAN_SAMPLE(CAN_DR0_PH1_T1, "Термодатчик фазы 1 драйвера 0")
        DEF_CAN_SAMPLE(CAN_DR1_USRC, "Напряжение питания драйвера 1")

        DEF_CAN_SAMPLE(CAN_DR2_TAIR, "Термодатчик возд. драйвера 2 - нет, резерв")
        DEF_CAN_SAMPLE(CAN_DR2_TCPU, "Температура CPU драйвера 2 - нет, резерв")
        DEF_CAN_SAMPLE(CAN_DR0_PH2_T1, "Термодатчик фазы 2 драйвера 0")
        DEF_CAN_SAMPLE(CAN_DR2_USRC, "Напряжение пит. драйвера 2 - нет, резерв")

        DEF_CAN_SAMPLE(CAN_DEXT_TAIR, "Термодат. возд. доп. устройства изм. темпер.")
        DEF_CAN_SAMPLE(CAN_DEXT_TCPU, "Температура CPU доп. устройства изм. темпер.")
        DEF_CAN_SAMPLE(CAN_DEXT_T1, "Термодатчик 1 доп. устройства изм. темпер.")
        DEF_CAN_SAMPLE(CAN_DEXT_USRC, "Напр. пит. доп. устройства изм. темпер.")

        DEF_CAN_SAMPLE(CAN_DR0_PH0_T2, "Термодатчик 2 фазы 0 драйвера 0")
        DEF_CAN_SAMPLE(CAN_DR0_PH1_T2, "Термодатчик 2 фазы 1 драйвера 0")
        DEF_CAN_SAMPLE(CAN_DR0_PH2_T2, "Термодатчик 2 фазы 2 драйвера 0")
        DEF_CAN_SAMPLE(CAN_DEXT_T2, "Термодатчик 2 доп. устройства изм. темпер.")

        DEF_CAN_SAMPLE(CAN_DR0_PH0_T3, "Термодатчик 3 фазы 0 драйвера 0")
        DEF_CAN_SAMPLE(CAN_DR0_PH1_T3, "Термодатчик 3 фазы 1 драйвера 0")
        DEF_CAN_SAMPLE(CAN_DR0_PH2_T3, "Термодатчик 3 фазы 2 драйвера 0")
        DEF_CAN_SAMPLE(CAN_DEXT_T3, "Термодатчик 3 доп. устройства изм. темпер.")

        DEF_CAN_SAMPLE(CAN_DR0_PH0_T4, "Термодатчик 4 фазы 0 драйвера 0")
        DEF_CAN_SAMPLE(CAN_DR0_PH1_T4, "Термодатчик 4 фазы 1 драйвера 0")
        DEF_CAN_SAMPLE(CAN_DR0_PH2_T4, "Термодатчик 4 фазы 2 драйвера 0")
        DEF_CAN_SAMPLE(CAN_DEXT_T4, "Термодатчик 4 доп. устройства изм. темпер.")

        DEF_CAN_SAMPLE(CAN_DR1_PH0_T1, "Термодатчик 1 фазы 0 драйвера 1")
        DEF_CAN_SAMPLE(CAN_DR1_PH0_T2, "Термодатчик 2 фазы 0 драйвера 1")
        DEF_CAN_SAMPLE(CAN_DR1_PH0_T3, "Термодатчик 3 фазы 0 драйвера 1")
        DEF_CAN_SAMPLE(CAN_DR1_PH0_T4, "Термодатчик 4 фазы 0 драйвера 1")

        DEF_CAN_SAMPLE(CAN_DR1_PH1_T1, "Термодатчик 1 фазы 1 драйвера 1")
        DEF_CAN_SAMPLE(CAN_DR1_PH1_T2, "Термодатчик 2 фазы 1 драйвера 1")
        DEF_CAN_SAMPLE(CAN_DR1_PH1_T3, "Термодатчик 3 фазы 1 драйвера 1")
        DEF_CAN_SAMPLE(CAN_DR1_PH1_T4, "Термодатчик 4 фазы 1 драйвера 1")

        DEF_CAN_SAMPLE(CAN_DR1_PH2_T1, "Термодатчик 1 фазы 2 драйвера 1")
        DEF_CAN_SAMPLE(CAN_DR1_PH2_T2, "Термодатчик 2 фазы 2 драйвера 1")
        DEF_CAN_SAMPLE(CAN_DR1_PH2_T3, "Термодатчик 3 фазы 2 драйвера 1")
        DEF_CAN_SAMPLE(CAN_DR1_PH2_T4, "Термодатчик 4 фазы 2 драйвера 1")

        DEF_CAN_SAMPLE(CAN_DEXT_T5, "Термодатчик 5 доп. устройства изм. темпер.")
        DEF_CAN_SAMPLE(CAN_DEXT_T6, "Термодатчик 6 доп. устройства изм. темпер.")
        DEF_CAN_SAMPLE(CAN_DEXT_T7, "Термодатчик 7 доп. устройства изм. темпер.")
        DEF_CAN_SAMPLE(CAN_DEXT_T8, "Термодатчик 8 доп. устройства изм. темпер.")
        DEF_CAN_SAMPLE(CAN_DEXT_T9, "Термодатчик 9 доп. устройства изм. темпер.")
        DEF_CAN_SAMPLE(CAN_DEXT_T10, "Термодатчик 10 доп. устройства изм. темпер.")
        DEF_CAN_SAMPLE(CAN_DEXT_T11, "Термодатчик 11 доп. устройства изм. темпер.")
        DEF_CAN_SAMPLE(CAN_DEXT_T12, "Термодатчик 12 доп. устройства изм. темпер.")

        DEF_CAN_CONST_NAME(CAN_DRIVERS_SAMPLES_END) /* =248 */

        #define CAN_DRIVERS_SAMPLES_COUNT \
            (CAN_DRIVERS_SAMPLES_END - CAN_DRIVERS_SAMPLES)
        /* --------------------------------------------------------------*/

        /* Измерения, передаваемые инвертором */
        DEF_CAN_CONST(CAN_INVERTOR_SAMPLES, CAN_DRIVERS_SAMPLES + 50)
        DEF_CAN_SAMPLE_CONST(
            CAN_INV_UD, /* =250 */ "Напряжение на инверторе",
            CAN_INVERTOR_SAMPLES
        )
        DEF_CAN_SAMPLE(CAN_INV_IA, "Ток фазы A инвертора")
        DEF_CAN_SAMPLE(CAN_INV_IB, "Ток фазы B инвертора")
        DEF_CAN_SAMPLE(CAN_INV_IC, "Ток фазы C инвертора")
        DEF_CAN_SAMPLE(CAN_INV_SMP0, "Задаваемое измерение контроллера")
        DEF_CAN_SAMPLE(CAN_INV_SMP1, "Фиксированное измерение контроллера 1")
        DEF_CAN_SAMPLE(CAN_INV_SMP2, "Фиксированное измерение контроллера 1")
        DEF_CAN_SAMPLE(CAN_INV_AIN1, "Аналоговый вход инвертора 1")
        DEF_CAN_SAMPLE(CAN_INV_AIN2, "Аналоговый вход инвертора 2")
        DEF_CAN_SAMPLE(CAN_INV_FIRMWARE_VER, "Версия прошивки")
        DEF_CAN_SAMPLE(CAN_INV_PFAST, "Мгновенная мощность")
        DEF_CAN_SAMPLE(CAN_INV_MOUT, "Текущий момент")
        DEF_CAN_SAMPLE(CAN_INV_TEXT, "Внешняя температура")
        DEF_CAN_SAMPLE(CAN_INV_ID, "Ток DC звена")
        DEF_CAN_SAMPLE(CAN_INV_PS_IND_ADC1, "Прокси аналогового входа индикатора")
        DEF_CAN_CONST_NAME(CAN_INVERTOR_SAMPLES_END) /* =265 */

        #define CAN_INVERTOR_SAMPLES_COUNT \
            (CAN_INVERTOR_SAMPLES_END - CAN_INVERTOR_SAMPLES)

        DEF_CAN_CONST_NAME(CAN_END_SAMPLE_ID)
    };

static QStringList fccanidList(void);
static QVector<QString> fccanidArray(void);

private:
 SampleCanIdClass() = delete;

};



struct registerFields{
    quint32 time_stamp_32;
    QString displayString;
    QByteArray regData7{7, Qt::Uninitialized}; // поле данных
    QByteArray regScales7{7, Qt::Uninitialized}; // поле масштабов
    quint8 flagReg; //  флаги регистров побитные
    // архив для плоттера
    QVector<qint16> regValue = (QVector<qint16>(PLOT_MAX_SIZE_ARR+10));
    QVector<quint32> regTime = (QVector<quint32>(PLOT_MAX_SIZE_ARR+10));
    int counterRegPlot = 0; // счетчик элементов плоттера

    bool flagNewData = false; // флаг получения обновленного значения

    union{          // значение регистра
       qint16 Reg16;
       struct {
           quint8 LowerByte;
           quint8 UpperByte;
       };
    } value;

    union{          // шкала регистра
       qint16 Reg16;
       struct {
           quint8 LowerByte;
           quint8 UpperByte;
       };
    } scale;

    union{          // максимальное значение переменной
       qint16 Reg16;
       struct {
           quint8 LowerByte;
           quint8 UpperByte;
       };
    } maxValue;

    union{          // минимальное значение регистра
       qint16 Reg16;
       struct {
           quint8 LowerByte;
           quint8 UpperByte;
       };
    } min;

    union{          // максимальное значение регистра
       qint16 Reg16;
       struct {
           quint8 LowerByte;
           quint8 UpperByte;
       };
    } max;
                                  // идентификатор измерений.регистров.установки регистров
    bool displayed = false;
};

struct samplesFields{
    quint32 time_stamp_32;
    QString displayString;
    bool displayed = false;   // выводить значение в таблице
    bool flagNewData = false; // флаг получения обновленного значения

    union{          // значение регистра
       qint16 Reg16;
       struct {
           quint8 LowerByte;
           quint8 UpperByte;
       };
    } value;

    union{          // максимальное значение переменной
       qint16 Reg16;
       struct {
           quint8 LowerByte;
           quint8 UpperByte;
       };
    } maxValue;

    union{          // шкала регистра
       qint16 Reg16;
       struct {
           quint8 LowerByte;
           quint8 UpperByte;
       };
    } scale;

    union{          // ноль регистра (не используется)
       qint16 Reg16;
       struct {
           quint8 LowerByte;
           quint8 UpperByte;
       };
    } zero;
};


struct standartID{
    union{ // структура для сборки ID из байтов парсинга
        quint16 Whole;
        struct {
            quint8 Body;
            quint8 Hdr;
        };
    } id;
};

//-----------------Все данные стандартного CAN из парсинга uart-----------
void handleAllStandartDataCan(
        quint32 time_stamp_32,
        QByteArray arrayDataFromCAN,
        registerFields *regDataArray,       
        QVector<QString> regNumList,
        samplesFields *sampleDataArray,
        QVector<QString> sampleNumList
);

#endif // IFACE_H
