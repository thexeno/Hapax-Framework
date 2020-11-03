#ifndef CONF_H
#define CONF_H

#include "hal.h"
/*
#include "sys_types.h"
//#include "gpio_hal.h"
//#include "timer_hal.h"
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "interrupt.h"
#include "hw_apps_rcm.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
// cosi sei obbligato a dichiarare per cosa hai incluso
 *
 *
 */



typedef enum
{
	DEBUG_LED = 0,
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

#endif
