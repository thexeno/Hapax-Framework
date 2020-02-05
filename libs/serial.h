#ifndef SERIAL_H_
#define SERIAL_H_


#include "uart_hal.h"
#include "utils/sys_types.h"
#include "utils/ring_buffer.h"


#define  SER_RB_SIZE 4U


struct  serial_parameters
{
	uint8_t tx_len_check;
	uint8_t rx_len_check;
	uint8_t rx_busy;
	uint8_t tx_busy;
} ;

//metti declar nel conf.h

typedef struct serial_device
{
	rb_t serial_tx_buff;
	rb_t serial_rx_buff;
	//uint8_t tx_buff[]; // but the hal does not have any buffer
	//uint8_t rx_buff[];
	const uart_hal_cfg_t* config;
} ser_dev_st;




// configure tx and rx sizes here ?

//int8_t Ser_init(ser_dev_st* console, uart_hal_cfg_t* config, uint8_t* txb, uint8_t* rxb);
//void Ser_full_duplex_start(long baud);
//void Ser_half_duplex_tx_start(long baud);
//void Ser_trig_tx(ser_dev_st* console);
//void Ser_put_buff(ser_dev_st* console, const char* str);
//void Ser_put_integer(ser_dev_st* console, uint16_t c);
//uint8_t Ser_tx_buffer_space_av(ser_dev_st* console);
//void Ser_half_duplex_rx_start(long baud);
//uint8_t Ser_rx_buffer_space_av(ser_dev_st* console);
//uint8_t Ser_get_char(ser_dev_st* console);


//void Ser_set_console(ser_dev_st* console, uart_hal_channel_t channel);
//void Ser_set_console_1(ser_dev_st* console);
int8_t Ser_init(ser_dev_st* console, const uart_hal_cfg_t* config, uint8_t* txb, base_t tx_sz, uint8_t* rxb, base_t rx_sz);
void Ser_trig_tx(ser_dev_st* console);
void Ser_put_buff(ser_dev_st* console, const char* str);
void Ser_put_integer(ser_dev_st* console, uint16_t c);
void Ser_put_char(ser_dev_st* console, char ch);
uint8_t Ser_tx_buffer_space_av(ser_dev_st* console);
uint8_t Ser_rx_buffer_space_av(ser_dev_st* console);
uint8_t Ser_rx_buffer_elem(ser_dev_st* console);
uint8_t Ser_get_char(ser_dev_st* console);


#endif
