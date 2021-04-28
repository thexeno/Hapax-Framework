#ifndef HAL_H_
#define HAL_H_

#include "utils/sys_types.h"

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>

typedef uint8_t cpu_dw_t;
typedef uint8_t base_t;
typedef uint8_t register_t;

/* The interrupt vector array wrapper index - Insert as many enumerations as the interrupt supported by the hardware for safely handle them later */
enum {
	HAL_VECT_TIMER1_ID = 0,
	HAL_VECT_TIMER0_ID,
	HAL_VECT_UART_RX_ID,
	HAL_VECT_UART_TX_BUFF_EMPTY_ID,
	HAL_VECT_UART_TX_DONE_ID,
	HAL_VECT_INT0_ID,
	HAL_VECT_INT1_ID,
	HAL_VECT_INT2_ID,
	HAL_VECT_ADC_ID,
	HAL_VECT_SPI_ID,
	HAL_SUPPORTED_ISR
};




#endif /* HAL_H_ */
