#include "conf.h"
#include "sys_types.h"
#include "gpio.h"
#include "gpio_hal.h"



//#define LSE_VALUE

// CPU configs
#define CONF_CPU_FREQ     16000000UL


/**********************************************************
 *                CONFIGURATION STRUCTURES                *
 **********************************************************
 *
 * Brief intro:
 * 
 * The configuration structs are composed by an enumeration, under which is perfomed the
 * association between logic components in the application and HW components.
 * Such enumeration is followed by the HW characteristics of the HW, coming from 
 * the HAL layers (or their wrappers) in the **portable** section.
 */

// e questo ti obbliga anche a dichiarare  TUTTI i pin
const gpio_hal_cfg_t gpio_hal_conf[GPIO_TOTAL_PIN] =
{
  { DEBUG_LED,  PIN_64, GPIO_DIR_MODE_OUT,   PIN_TYPE_STD, GPIO_HAL_VAL_TRUE}
};

const gpio_hal_cfg_t * const Gpio_hal_conf_get(void)
{
    return (gpio_hal_conf/*[0]*/);
}

base_t Gpio_hal_conf_get_size(void)
{
    return (sizeof(gpio_hal_conf)/sizeof(gpio_hal_conf[0]));
}


