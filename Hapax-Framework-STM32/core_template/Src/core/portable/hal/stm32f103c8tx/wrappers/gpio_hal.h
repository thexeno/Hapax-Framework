#ifndef GPIO_HAL_H
#define GPIO_HAL_H

#include <sys_types.h>
#include "hal.h"
#include "conf.h"

typedef GPIO_TypeDef*  gpio_hal_port_t;
typedef uint16_t      gpio_hal_pin_t;
typedef uint32_t	  gpio_hal_mode_t;
typedef GPIO_PinState gpio_hal_val_t;
typedef uint32_t      gpio_hal_pull_t;

/* Use and redefine the type of the native HAL library used to simplify the access in SET/RESET API */
#define GPIO_HIGH GPIO_PIN_SET     
#define GPIO_LOW  GPIO_PIN_RESET

#define GPIO_TOTAL_PIN 12

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
	gpio_hal_port_t port;
	gpio_hal_pin_t pin;
	gpio_hal_mode_t mode_io;
	gpio_hal_pull_t mode_pull;
	gpio_hal_val_t	val;
	gpio_hal_err_t status;
} gpio_hal_cfg_t;
//#define GPIO_TOTAL_CONF 

const gpio_hal_cfg_t * const Gpio_hal_conf_get(void);
base_t Gpio_hal_conf_get_size(void);

void Gpio_hal_init(const gpio_hal_cfg_t* handle);
gpio_hal_err_t Gpio_hal_set_mode(conf_gpio_e pin_enum, gpio_hal_mode_t mode_io);
gpio_hal_val_t Gpio_hal_get_value(conf_gpio_e pin_enum);
gpio_hal_err_t Gpio_hal_set_value(conf_gpio_e pin_enum, gpio_hal_val_t val);
base_t Gpio_hal_parallel_get_val(gpio_hal_port_t port_id, base_t val);
void Gpio_hal_parallel_set_val(gpio_hal_port_t port_id, base_t val);


#endif
