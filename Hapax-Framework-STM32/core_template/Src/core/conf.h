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
	PWM_CHANNEL_TEST_2,
	PWM_CHANNEL_TEST_3,
	CONF_PWM_ENUM_UNUSED
} conf_pwm_e;


typedef enum
{
	OC_CHANNEL_TEST_1 = 0,
	CONF_OC_ENUM_UNUSED
} conf_oc_e;


#endif
