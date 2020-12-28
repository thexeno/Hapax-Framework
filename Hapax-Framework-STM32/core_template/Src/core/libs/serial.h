#ifndef SERIAL_H_
#define SERIAL_H_

#include "sys_types.h"
#include "conf.h"

#define SERIAL_TOTAL_INSTANCES CONF_CONSOLE_TOTAL_ENTITY

typedef enum ser_enumerations
{
    SER_OK = 0,
    SER_INIT_PEND,
    SER_TX_BUSY,
    SER_TX_DONE,
    SER_ERR
} ser_status_t;



typedef struct serial_device
{
	conf_serial_e serial;
	serial_generic_t source;
} serial_conf_t;

const serial_conf_t*  Serial_conf_get(void);
base_t Serial_conf_get_size(void);

ser_status_t Serial_init(const serial_conf_t* console);
ser_status_t Serial_get_status(conf_serial_e channel);
ser_status_t Serial_trig(conf_serial_e channel);
void Ser_put_buff(conf_serial_e channel, const char* str);
void Ser_put_char(conf_serial_e channel, char ch);
void Ser_put_integer(conf_serial_e channel, char* format, uint16_t n);
uint8_t Serial_get_char(conf_serial_e channel);
uint8_t Serial_get_TX_size(conf_serial_e channel);
uint8_t Serial_get_RX_size(conf_serial_e channel);
void Serial_RX_handler(serial_generic_t channel);
void Serial_TX_handler(serial_generic_t channel);


#endif
