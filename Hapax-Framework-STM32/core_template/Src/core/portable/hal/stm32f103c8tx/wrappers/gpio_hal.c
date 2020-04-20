#include <wrappers/gpio_hal.h>


gpio_hal_cfg_t *gpio_hal_cfg_buff;
base_t total_pins = 0;



gpio_hal_err_t Gpio_hal_set_mode(base_t pin_enum, gpio_hal_mode_t mode_io)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if (pin_enum < total_pins)
	{
		GPIO_InitStruct.Mode = mode_io;
		GPIO_InitStruct.Pin = gpio_hal_cfg_buff[pin_enum].pin;
		GPIO_InitStruct.Pull = gpio_hal_cfg_buff[pin_enum].mode_pull; 
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // metti API per gestire funzioni particolari
		HAL_GPIO_Init(gpio_hal_cfg_buff[pin_enum].port, &GPIO_InitStruct);
		return GPIO_OK;
	}
	else
		return GPIO_ERR_PIN;
}


gpio_hal_err_t Gpio_hal_set_value(base_t pin_enum, gpio_hal_val_t val) // per il gpio_hal_val_t metti un redefine di quello ufficiale stm32
{
	if (pin_enum < total_pins)
	{
		HAL_GPIO_WritePin(gpio_hal_cfg_buff[pin_enum].port, gpio_hal_cfg_buff[pin_enum].pin, val);
		return GPIO_OK;
	}
	else
		return GPIO_ERR_PIN;
}

gpio_hal_val_t Gpio_hal_get_value(base_t pin_enum)
{
	gpio_hal_val_t ret = 0;
	if (pin_enum < total_pins)
	{
		ret = HAL_GPIO_ReadPin(gpio_hal_cfg_buff[pin_enum].port, gpio_hal_cfg_buff[pin_enum].pin);
	}
	else
	{
		return (gpio_hal_val_t)GPIO_ERR_PIN;
	}
	return ret;
}


void Gpio_hal_parallel_set_val(gpio_hal_port_t port_id, base_t val)
{
	port_id->ODR = (val & (base_t)0x0000ffff); // in file stm32f103x6.h
}

base_t Gpio_hal_parallel_get_val(gpio_hal_port_t port_id, base_t val)
{
	return port_id->IDR; // in file stm32f103x6.h
}


void Gpio_hal_init(gpio_hal_cfg_t* handle, base_t sz)
{
	uint8_t i = 0;
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	gpio_hal_cfg_buff = handle;
	total_pins = sz;

	for (i = 0; i < sz /* CONF_TOTAL_APP_PIN */ ; i++)
	{
		HAL_GPIO_DeInit(handle[i].port, handle[i].pin);
	}

	for (i = 0; i < sz /* CONF_TOTAL_APP_PIN */ ; i++)
	{

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

