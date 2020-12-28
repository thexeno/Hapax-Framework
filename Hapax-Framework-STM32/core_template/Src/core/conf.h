#ifndef CONF_H
#define CONF_H

#include "sys_types.h"
//#include "gpio_hal.h"
//#include "timer_hal.h"

// cosi sei obbligato a dichiarare per cosa hai incluso
typedef enum
{
	DEBUG_LED = 0,
	TIMER4_TEST_PIN0,
	MCO_PIN,
	TIMER4_TEST_PIN1,
    PWM_PIN,
    CONF_GPIO_ENUM_UNUSED
} conf_gpio_e;


typedef enum
{
	TIMER_4 = 0,
	TIMER_3,
	CONF_TIMER_ENUM_UNUSED
} conf_timer_e;


typedef enum
{
	PWM_CHANNEL_TEST_1 = 0,
	CONF_PWM_ENUM_UNUSED
} conf_pwm_e;


typedef enum
{
	OC_CHANNEL_TEST_1 = 0,
	CONF_OC_ENUM_UNUSED
} conf_oc_e;


typedef enum
{
	SPI_TEST_0 = 0,
	CONF_SPI_ENUM_UNUSED
} conf_spi_e;

// CPU configs
#define CONF_HSE_OSC    24000000UL
#define CONF_LSE_OSC    32768UL
#define CONF_AHB_FREQ      8000000UL
#define CONF_APB1_FREQ     4000000UL
#define CONF_APB2_FREQ     4000000UL

#endif
