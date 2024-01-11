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

#endif // INVERTOR_H
