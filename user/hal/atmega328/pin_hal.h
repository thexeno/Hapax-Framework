#ifndef PIN_HAL_H
#define PIN_HAL_H

#include "hal.h"
#include "utils/sys_types.h"

typedef enum 
{
	//GPIO_B0 = 0U, GPIO_B1, GPIO_B2, GPIO_B3, GPIO_B4, GPIO_B5, GPIO_B6, GPIO_B7,
	//GPIO_C0 = 8U, GPIO_C1, GPIO_C2, GPIO_C3, GPIO_C4, GPIO_C5, GPIO_C6, GPIO_C7,
	//GPIO_TOTAL
	GPIO_PORT_B = 0,
	GPIO_PORT_C,
	GPIO_PORT_D,
	GPIO_PORT_CHANNELS
} gpio_hal_port_t;

typedef enum 
{
	GPIO_PIN_0 = 0,
	GPIO_PIN_1,
	GPIO_PIN_2,
	GPIO_PIN_3,
	GPIO_PIN_4,
	GPIO_PIN_5,
	GPIO_PIN_6,
	GPIO_PIN_7,
	GPIO_PORT_WIDE
} gpio_hal_pin_t;

typedef enum 
{
	GPIO_MODE_INPUT = 0,
	GPIO_MODE_INPUT_PULL_HI,
	GPIO_MODE_INPUT_PULL_LOW,
	GPIO_MODE_OUTPUT	
} gpio_hal_mode_t;

typedef enum 
{
	GPIO_LOW = 0,
	GPIO_HIGH	
} gpio_hal_val_t;


typedef enum
{
	GPIO_OK = 0,
	GPIO_INIT_PEND,
	GPIO_ERR_PIN,
	GPIO_ERR_PORT
} gpio_hal_err_t;

typedef struct
{
	base_t pin_enum;
	gpio_hal_port_t port;
	gpio_hal_pin_t pin;
	gpio_hal_mode_t mode_io;
	gpio_hal_val_t	val;
	gpio_hal_err_t status;
} gpio_hal_cfg_t;
//#define GPIO_TOTAL_CONF 

extern gpio_hal_cfg_t gpio_hal_conf[];

void Gpio_hal_init(gpio_hal_cfg_t* handle, base_t sz);
gpio_hal_err_t Gpio_hal_set_mode(base_t pin_enum, gpio_hal_mode_t mode_io);
gpio_hal_val_t Gpio_hal_get_value(base_t pin_enum);
gpio_hal_err_t Gpio_hal_set_value(base_t pin_enum, gpio_hal_val_t val);
gpio_hal_err_t Gpio_hal_parallel_set_val(gpio_hal_port_t port_id, base_t val);
void Gpio_hal_reset(void);

#endif
