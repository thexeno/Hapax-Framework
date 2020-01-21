#ifndef UART_HAL_H
#define	UART_HAL_H

#include "hal.h"
#include "utils/sys_types.h"



typedef enum
{
	UART_HALF_DUPLEX_TX,
	UART_HALF_DUPLEX_RX,
	UART_FULL_DUPLEX
} uart_hal_mode_t;


typedef enum 
{
	UART_0,
	//UART_1,
	NUM_UART_CHANNELS
} uart_hal_ch_t;


typedef struct
{
	uart_hal_ch_t channel;  //UARTA0_BASE
	uart_hal_mode_t mode;
	long	baudrate;
	long	sys_clock;
	// combina i pin su pin_hal
} uart_hal_cgf_t;


//nel conf.h
extern const uart_hal_cgf_t uart_hal_cfg[];

 

uint8_t UartHal_get_byte(const uart_hal_cgf_t* handle);
void UartHal_put_byte(const uart_hal_cgf_t* handle, uint8_t d);
void UartHal_init(const uart_hal_cgf_t* handle, uint8_t special);
void UartHal_ISR_RX_callback_set(void (*fp_t)(uart_hal_ch_t ), uart_hal_ch_t channel);
void UartHal_ISR_TX_callback_set(void (*fp_t)(uart_hal_ch_t ), uart_hal_ch_t channel);

#endif