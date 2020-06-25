#include <wrappers/gpio_hal.h>
#include "conf.h"


const gpio_hal_cfg_t *gpio_hal_cfg_buff;
base_t total_pins = 0;

gpio_hal_err_t Gpio_hal_set_mode(conf_gpio_e pin_enum, gpio_hal_mode_t mode_io)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
    base_t i = 0;
	for(i = 0; i < total_pins; i++)
	{
        if (gpio_hal_cfg_buff[i].pin_enum != CONF_GPIO_ENUM_UNUSED)
        {
            // can be optimized with LOCAL INDICIZATION!, or use the fast, non portable approach
            if (gpio_hal_cfg_buff[i].pin_enum == pin_enum)
            {
                GPIO_InitStruct.Mode = mode_io;
                GPIO_InitStruct.Pin = gpio_hal_cfg_buff[i].pin;
                GPIO_InitStruct.Pull = gpio_hal_cfg_buff[i].mode_pull; 
                GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // metti API per gestire funzioni particolari
                HAL_GPIO_Init(gpio_hal_cfg_buff[i].port, &GPIO_InitStruct);
                return GPIO_OK;
            }
	    }
    }
	return GPIO_ERR_PIN;
}


gpio_hal_err_t Gpio_hal_set_value(conf_gpio_e pin_enum, gpio_hal_val_t val) // per il gpio_hal_val_t metti un redefine di quello ufficiale stm32
{
    base_t i = 0;
	for(i = 0; i < total_pins; i++)
	{
        if (gpio_hal_cfg_buff[i].pin_enum != CONF_GPIO_ENUM_UNUSED)
        {
            // can be optimized with LOCAL INDICIZATION!, or use the fast, non portable approach
            if (gpio_hal_cfg_buff[i].pin_enum == pin_enum)
            {
                HAL_GPIO_WritePin(gpio_hal_cfg_buff[i].port, gpio_hal_cfg_buff[i].pin, val);
                return GPIO_OK;
            }
        }
	}
	return GPIO_ERR_PIN;
}

gpio_hal_val_t Gpio_hal_get_value(conf_gpio_e pin_enum)
{
	gpio_hal_val_t ret = 0;
    base_t i = 0;

	for(i = 0; i < total_pins; i++)
	{
        if (gpio_hal_cfg_buff[i].pin_enum != CONF_GPIO_ENUM_UNUSED)
        {
            // can be optimized with LOCAL INDICIZATION!, or use the fast, non portable approach
            if (gpio_hal_cfg_buff[i].pin_enum == pin_enum)
            {
                return HAL_GPIO_ReadPin(gpio_hal_cfg_buff[i].port, gpio_hal_cfg_buff[i].pin);
            }
        }
    }
	return (gpio_hal_val_t)GPIO_ERR_PIN;
}


void Gpio_hal_parallel_set_val(gpio_hal_port_t port_id, base_t val)
{
	port_id->ODR = (val & (base_t)0x0000ffff); // in file stm32f103x6.h
}

base_t Gpio_hal_parallel_get_val(gpio_hal_port_t port_id, base_t val)
{
	return port_id->IDR; // in file stm32f103x6.h
}


void Gpio_hal_init(const gpio_hal_cfg_t* handle)
{
	uint8_t i = 0;
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	gpio_hal_cfg_buff = handle;
	total_pins = Gpio_hal_conf_get_size();

	for (i = 0; i < total_pins; i++)
	{
		HAL_GPIO_DeInit(handle[i].port, handle[i].pin);
	}

	for (i = 0; i < total_pins ; i++)
	{
        /* Don't exclude the CONF_GPIO_ENUM_UNUSED on purpose */
		if ((gpio_hal_port_t)handle[i].port == GPIOA)
			__HAL_RCC_GPIOA_CLK_ENABLE();
		if ((gpio_hal_port_t)handle[i].port == GPIOB)
			__HAL_RCC_GPIOB_CLK_ENABLE();
		if ((gpio_hal_port_t)handle[i].port == GPIOC)
			__HAL_RCC_GPIOC_CLK_ENABLE();
		if ((gpio_hal_port_t)handle[i].port == GPIOD)
			__HAL_RCC_GPIOD_CLK_ENABLE();

		HAL_GPIO_WritePin(handle[i].port, handle[i].pin, handle[i].val); // reason: was initialized first time in the CubeMX generated code example
		GPIO_InitStruct.Mode = handle[i].mode_io;
		GPIO_InitStruct.Pin = handle[i].pin;
		GPIO_InitStruct.Pull = handle[i].mode_pull; 
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // metti API per gestire funzioni particolari
		HAL_GPIO_Init(handle[i].port, &GPIO_InitStruct);
	}
}

