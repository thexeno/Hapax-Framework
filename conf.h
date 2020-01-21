#ifndef CONF_H_
#define CONF_H_

#include "hal/atmega328/hal.h"
#include "hal/atmega328/pin_hal.h"
#include "hal/atmega328/spi_hal.h"
#include "hal/atmega328/uart_hal.h"
#include "hal/atmega328/interrupt_hal.h"
#include "hal/atmega328/timer_hal.h"


// CPU configs
#define CONF_CPU_FREQ	 16000000UL

// UART defines and config
#define CONF_SERIAL_BAUD 9600U
const uart_hal_cgf_t uart_hal_cfg[] =
{
	{UART_0, UART_HALF_DUPLEX_TX, 800000U, CONF_CPU_FREQ},
	//{UART_0, UART_FULL_DUPLEX, CONF_SERIAL_BAUD, CONF_CPU_FREQ}
};

// Utils config
typedef uint8_t rb_sz_t;

/* Pin assignement LED*/
//#define DEBUG_LED	(GPIO_B5)
//#define DEBUG_LED_1	(PORT_B | PIN_3)
//#define DEBUG_LED_2	(PORT_B | PIN_0)

/* Pin assignement buttons */
//#define MAIN_BUTTON (PORT_D | PIN_6)
//#define MAIN_BUTTON_INIT_V 0x01

enum 
{
	DEBUG_LED = 0,
	DEBUG_LED_OC_T2A,
	MOSI_PIN,
	DEBUG_LED_2_SS,
	DEBUG_LED_PWM_T0B,
	SYNC_CLK_PIN,
	CONF_TOTAL_APP_PIN
};

#define LCD_PARALLEL GPIO_PORT_B

const gpio_hal_cfg_t gpio_hal_conf[CONF_TOTAL_APP_PIN] =
{
	{DEBUG_LED, GPIO_PORT_B, GPIO_PIN_5, GPIO_MODE_OUTPUT, GPIO_LOW},
	{DEBUG_LED_OC_T2A, GPIO_PORT_B, GPIO_PIN_3, GPIO_MODE_OUTPUT, GPIO_LOW},
	{MOSI_PIN, GPIO_PORT_B, GPIO_PIN_3, GPIO_MODE_OUTPUT, GPIO_LOW},
	{DEBUG_LED_2_SS, GPIO_PORT_B, GPIO_PIN_2, GPIO_MODE_OUTPUT, GPIO_HIGH},
	{DEBUG_LED_PWM_T0B, GPIO_PORT_D, GPIO_PIN_5, GPIO_MODE_OUTPUT, GPIO_HIGH},
	{SYNC_CLK_PIN, GPIO_PORT_D, GPIO_PIN_4, GPIO_MODE_OUTPUT, GPIO_HIGH}
};




#endif /* HAL_CONF_H_ */
