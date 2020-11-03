#ifndef GPIO_HAL_H
#define GPIO_HAL_H

#include <sys_types.h>
#include "hal.h"
#include "conf.h"




typedef unsigned char   gpio_hal_pin_t;
typedef unsigned long   gpio_hal_port_t;
typedef unsigned long	gpio_hal_mode_t;
typedef enum {
    GPIO_HAL_VAL_FALSE = 0,
    GPIO_HAL_VAL_TRUE = 0xFF,
}   gpio_hal_val_t;
typedef unsigned long   gpio_hal_pull_t;

/* Use and redefine the type of the native HAL library used to simplify the access in SET/RESET API */
#define GPIO_HIGH GPIO_PIN_SET     
#define GPIO_LOW  GPIO_PIN_RESET

#define GPIO_TOTAL_PIN CONF_GPIO_ENUM_UNUSED

typedef enum
{
	GPIO_OK = 0,
	GPIO_INIT_PEND,
	GPIO_ERR_PIN,
	GPIO_ERR_PORT
} gpio_hal_err_t;

typedef struct
{
	conf_gpio_e pin_enum;
	gpio_hal_pin_t pin;
	gpio_hal_mode_t mode_io;
	gpio_hal_pull_t mode_pull;
	gpio_hal_val_t	val;
	gpio_hal_err_t status;
} gpio_hal_cfg_t;
//#define GPIO_TOTAL_CONF 

const gpio_hal_cfg_t*  Gpio_hal_conf_get(void);
base_t Gpio_hal_conf_get_size(void);

void Gpio_hal_init(const gpio_hal_cfg_t* handle);
gpio_hal_val_t Gpio_hal_read_value(conf_gpio_e pin_enum);
gpio_hal_err_t Gpio_hal_write_value(conf_gpio_e pin_enum, gpio_hal_val_t val);
gpio_hal_port_t Gpio_hal_parallel_read_val(gpio_hal_port_t port_id, gpio_hal_port_t val);
void Gpio_hal_parallel_write_val(gpio_hal_port_t port_id, gpio_hal_port_t val);

#endif
