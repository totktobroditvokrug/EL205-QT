#ifndef MEASURE_H
#define MEASURE_H

#include <QDebug>
#include <QString>
#include <QStringList>


class measure
{
public:
    measure();
    static void debug();
    static void readMeasureListFromFile();
};


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

/* типы сообщений по CAN-шине */
DEF_CAN_ENUM {

    /* Установка регистра */
    DEF_CAN_CONST(CAN_SET_REGISTER, 10)
    /* Передача содержимого регистра */
    DEF_CAN_CONST(CAN_SHOW_REGISTER, 20)
    /* передача масштабов к регистру */
    DEF_CAN_CONST(CAN_SHOW_REGISTER_SCALES, 30)
    /* запрос содержимого регистра */
    DEF_CAN_CONST(CAN_REGISTER_REQUEST, 40)

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
} fc_can_id;



#endif // MEASURE_H
