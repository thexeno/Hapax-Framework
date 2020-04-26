#ifndef CONF_H
#define CONF_H

#include <wrappers/timer_hal.h>
#include <wrappers/clock_hal.h>
#include <wrappers/gpio_hal.h>
#include <wrappers/hal.h>


//#define LSE_VALUE

// CPU configs
#define CONF_CPU_FREQ	 16000000UL

// Utils config
//typedef uint8_t rb_sz_t;



enum
{
	DEBUG_LED,
	TIMER4_PIN,
	MCO_PIN,
	CONF_TOTAL_APP_PIN
};
gpio_hal_cfg_t gpio_hal_conf[CONF_TOTAL_APP_PIN] =
{
	{DEBUG_LED,		GPIOC, GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_PIN_SET,   GPIO_NOPULL}, // metti l-init pend interno all'hal
	{TIMER4_PIN,	GPIOB, GPIO_PIN_6,  GPIO_MODE_AF_PP, 	 GPIO_PIN_RESET, GPIO_NOPULL},
	{MCO_PIN, 		GPIOA, GPIO_PIN_8,	GPIO_MODE_AF_PP,	 GPIO_PIN_RESET, GPIO_NOPULL}
};


timer_hal_cfg_t timer_hal_conf[1] =
{
	// TMW_PWM for example is a custom enum, as there are more than one OC type, so customisaztion can be implemented with an dditional custo
	// name
	{TIMER_4,	256,	3125,	TMR_PWM,	TIM_OCPOLARITY_HIGH,	TIM_CHANNEL_1}
};

#endif /* HAL_CONF_H_ */
