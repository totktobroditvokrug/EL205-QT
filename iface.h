#ifndef IFACE_H
#define IFACE_H

#include <QObject>
#include <QMetaEnum>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QList>
#include "registers.h"

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



struct registerFields{
    QByteArray regData7{7, Qt::Uninitialized}; // поле данных
    quint8 idBody;                              // идентификатор измерений.регистров.установки регистров
    bool displayed = false;
//    /* для измерений */
//    struct {
//        int16_t value;		/* измеренное значение */
//        int16_t max;		/* максимальное значение */
//        int16_t scale;		/* реальное макс. значение */
//        int16_t zero;		/* уровень нуля измерения */
//    } sample;


//    /*  для регистров */
//    struct {
//        uint8_t number;		/* номер регистра */
//        uint8_t flags;		/* флаги регистра */

//        union {
//            struct {
//                /* значение */
//                int16_t	value;
//                /* минимум */
//                int16_t	min;
//                /* максимум */
//                int16_t max;
//            };
//            struct {
//                /* масштаб */
//                int16_t	scale;
//                /* максимальное зн. (например АЦП) */
//                int16_t max_value;
//                /* нуль */
//                int16_t zero;
//            };

//            struct {
//                uint16_t status;
//                uint16_t mask;
//            };
//        };
//    } reg;

//    /* установка регистра */
//    struct {
//        uint8_t number;
//        union {
//            int16_t value;
//            uint16_t status;
//        };
//    } reg_set;
};

QString handleRegData(quint8 idBody, QByteArray standartArrayDATA, registerFields *regDataArray, quint8 regNum, QString regName);

#endif // IFACE_H
