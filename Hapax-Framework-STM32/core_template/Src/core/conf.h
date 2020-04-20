#ifndef CONF_H
#define CONF_H

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
	CONF_TOTAL_APP_PIN
};
gpio_hal_cfg_t gpio_hal_conf[CONF_TOTAL_APP_PIN] =
{
	{DEBUG_LED,	GPIOC, GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_PIN_SET, GPIO_NOPULL}, // metti l-init pend interno all'hal
};


#endif /* HAL_CONF_H_ */
