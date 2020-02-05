#include "pin_hal.h"

// beningo
// static TYPE volatile * const DataIn[NUM_PORTS] =
// {
// 	(TYPE*)&REGISTER1, (TYPE*)&REGISTER2, 
// };

#define GPIO_CHANNELS 3U

// same order as port
register_t volatile * const ddr[GPIO_CHANNELS] =
{
	(register_t*)&DDRB,
	(register_t*)&DDRC,
	(register_t*)&DDRD
};

register_t volatile * const port[GPIO_CHANNELS] =
{
	(register_t*)&PORTB,
	(register_t*)&PORTC,
	(register_t*)&PORTD
};

// same order as port
register_t volatile * const pin[GPIO_CHANNELS] =
{
	(register_t*)&PINB,
	(register_t*)&PINC,
	(register_t*)&PIND
};

/*const base_t pins[GPIO_PIN_WIDE] =
{
	(1U << 0), (1U << 1), (1U << 2), 
	(1U << 3), (1U << 4), (1U << 5),
	(1U << 6), (1U << 7) 
};*/

gpio_hal_cfg_t *gpio_hal_cfg_buff;
base_t total_pins = 0;



gpio_hal_err_t Gpio_hal_set_mode(base_t pin_enum, gpio_hal_mode_t mode_io)
{
	if (gpio_hal_cfg_buff[pin_enum].status != GPIO_OK)
	{
		return GPIO_ERR_PIN; 
	}
	if (pin_enum < total_pins)
	{
		if (mode_io == GPIO_MODE_OUTPUT)
		{
			*ddr[gpio_hal_cfg_buff[pin_enum].port] |= (1 << gpio_hal_cfg_buff[pin_enum].pin);
		}
		else
		{
			*ddr[gpio_hal_cfg_buff[pin_enum].port] &= ~(1 << gpio_hal_cfg_buff[pin_enum].pin);
		}
		return GPIO_OK;
	}
	return GPIO_ERR_PIN;
}

gpio_hal_err_t Gpio_hal_set_value(base_t pin_enum, gpio_hal_val_t val)
{
	if (pin_enum < total_pins)
	{
		if (val == GPIO_HIGH)
		{
			*port[gpio_hal_cfg_buff[pin_enum].port] |= (1 << gpio_hal_cfg_buff[pin_enum].pin);
		}
		else
		{
			*port[gpio_hal_cfg_buff[pin_enum].port] &= ~(1 << gpio_hal_cfg_buff[pin_enum].pin);
		}
		return GPIO_OK;
	}
	return GPIO_ERR_PIN;
}

base_t Gpio_hal_get_value(base_t pin_enum, gpio_hal_val_t val)
{
	base_t ret = 0;
	if (pin_enum < total_pins)
	{
		ret = (*pin[gpio_hal_cfg_buff[pin_enum].port]) & (1 << gpio_hal_cfg_buff[pin_enum].pin);
	}
	return ret;
}


gpio_hal_err_t Gpio_hal_parallel_set_val(gpio_hal_port_t port_id, base_t val)
{
	if (port_id < GPIO_PORT_CHANNELS)
	{
		*port[port_id] = val;
		return GPIO_OK;
	}
	return GPIO_ERR_PORT;
}

void Gpio_hal_reset(void)
{
	// Init default config
	base_t i = 0;
	for (i=0; i<GPIO_PORT_CHANNELS; i++)
	{
		*ddr[i] = (GPIO_MODE_INPUT & (GPIO_PORT_WIDE-1)); // extedn value to the whole port
		*port[i] = 0;
		/* Disable pullup on all ports */
		MCUCR |= (1 << PUD);
	}
}

void Gpio_hal_init(gpio_hal_cfg_t* handle, base_t sz)
{
	uint8_t temp = 0;
	uint8_t temp1 = 0;
	uint8_t i = 0;

	Gpio_hal_reset();

	gpio_hal_cfg_buff = handle;
	total_pins = sz;

	for (i = 0; i < sz /* CONF_TOTAL_APP_PIN */ ; i++)
	{
		temp = handle[i].port;
		temp1 = (1U << handle[i].pin);

		if (handle[i].mode_io == GPIO_MODE_OUTPUT)
		{
			*ddr[temp] |= temp1;
			if (handle[i].val == GPIO_HIGH)
			{
				*port[temp] |= temp1;
			}
			else
			{
				*port[temp] &= ~(temp1);
			}
		}
		else if (handle[i].mode_io == GPIO_MODE_INPUT_PULL_HI)
		{
			/* Enable pullup on all ports */
			MCUCR &= ~(1 << PUD);
			*ddr[temp] &= ~(temp1);
			*port[temp] |= temp1;
		}
		else
		{
			// no pull
			*ddr[temp] &= ~(temp1);
			*port[temp] &= ~(temp1);
		}
		handle[i].status = GPIO_OK; 
	}
}


//  void Gpio_hal_pu_handle(const gpio_hal_cfg_t* handle, uint8_t pull_mode)
//  {
//  	uint8_t temp = handle->port_pin / GPIO_PIN_WIDE;
//  	uint8_t temp1 = handle->port_pin % GPIO_PIN_WIDE;
//  	if (pull_mode == GPIO_PULL_HI)
//  	{
//  		// table 18.1 datasheet to enable pullup
//  		ddr[temp] &= ~(temp1);
//  		port[temp] |= temp1;
//  		/* Enable pullup on all ports */
//  		MCUCR &= ~(1 << PUD);
//  	}
//  	else
//  	{
//  		ddr[temp] &= ~(temp1);
//  		port[temp] &= ~(temp1);
//  	}
//  }
//  
//  	uint8_t pins_conf = sizeof(handle) / sizeof((handle)[0]);
//  	// Init the configured pins
//  	for (i = 0; i < pins_conf; i++)
//  	{
//  		temp = handle[i]->port_pin / GPIO_PIN_WIDE;
//  		ddr[temp] = handle
//  	}
//  }


// static uint8_t _pin_GetMdPin(uint8_t port_pin)
// {
// 	uint8_t ret = 0;
	
// 	switch (port_pin & 0xf0)
// 	{
// 		case PORT_C:
// 		if (!(DDRC & (1<<(port_pin & 0x0f))))
// 		{
// 			ret = PIN_INPUT;
// 		}
// 		else
// 		{
// 			ret = PIN_OUTPUT;
// 		}						
// 		break;			
		
// 		case PORT_D:
// 		if (!(DDRD & (1<<(port_pin & 0x0f))))
// 		{
// 			ret = PIN_INPUT;
// 		}
// 		else
// 		{
// 			ret = PIN_OUTPUT;
// 		}
// 		break;
		
// 		case PORT_B:
// 		if (!(DDRB & (1<<(port_pin & 0x0f))))
// 		{
// 			ret = PIN_INPUT;
// 		}
// 		else
// 		{
// 			ret = PIN_OUTPUT;
// 		}
// 		break;
// 	}
// 	return ret;
// }

// void Pin_pull_disable(void)
// {
// 	/* Disable pullup on all ports */
// 	MCUCR |= (1 << PUD);
// }


// void Pin_mode(uint16_t port_pin, uint8_t inOut, uint8_t pull)
// {
// 	if (pull == PULLUP_ENABLED)
// 	{
// 		/* Enable pullup on all ports */
// 		MCUCR &= ~(1 << PUD);
// 	}
// 	else
// 	{
// 		/* If only one port require pullup, the other calls must keep that enabled */
// 	}
	
// 	switch (port_pin & 0xf0)
// 	{
// 		case PORT_C:
// 		if (inOut == PIN_INPUT)
// 		{
// 			DDRC &= ~(1<<(port_pin & 0x0f));
// 			if (pull == PULLUP_ENABLED)
// 			{
// 				PORTC |= (1<<(port_pin & 0x0f));
// 			}
// 			else
// 			{
// 				PORTC &= ~(1<<(port_pin & 0x0f));
// 			}
// 		}
// 		else
// 		{
// 			DDRC |= (1<<(port_pin & 0x0f));
// 		}	
// 		break;
		
// 		case PORT_D:
// 		if (inOut == PIN_INPUT)
// 		{
// 			DDRD &= ~(1<<(port_pin & 0x0f));
// 			if (pull == PULLUP_ENABLED)
// 			{
// 				PORTD |= (1<<(port_pin & 0x0f));
// 			}
// 			else
// 			{
// 				PORTD &= ~(1<<(port_pin & 0x0f));
// 			}
// 		}
// 		else
// 		{
// 			DDRD |= (1<<(port_pin & 0x0f));
// 		}
// 		break;
		
// 		case PORT_B:
// 		if (inOut == PIN_INPUT)
// 		{
// 			DDRB &= ~(1<<(port_pin & 0x0f));
// 			if (pull == PULLUP_ENABLED)
// 			{
// 				PORTB |= (1<<(port_pin & 0x0f));
// 			}
// 			else
// 			{
// 				PORTB &= ~(1<<(port_pin & 0x0f));
// 			}
// 		}
// 		else
// 		{
// 			DDRB |= (1<<(port_pin & 0x0f));
// 		}
// 		break;
		
// 	}
// }




// void Pin_wr_pin(gpio_hal_ch_t ch, uint8_t val)
// {
// 	if (_pin_GetMdPin(port_pin) == PIN_INPUT)
// 	{
// 		/* Do not modify pull values */
// 	//	return;
// 	}
	

// 	port[ch] 

// 	switch (port_pin & 0xf0)
// 	{
// 		case PORT_C:
// 		if (val != 0)
// 		{
// 			PORTC |= (1<<(port_pin & 0x0f));
// 		}
// 		else
// 		{
// 			PORTC &= ~(1<<(port_pin & 0x0f));
// 		}	
// 		break;		
		
// 		case PORT_D:
// 		if (val != 0)
// 		{
// 			PORTD |= (1<<(port_pin & 0x0f));
// 		}
// 		else
// 		{
// 			PORTD &= ~(1<<(port_pin & 0x0f));
// 		}
// 		break;
		
// 		case PORT_B:
// 		if (val != 0)
// 		{
// 			PORTB |= (1<<(port_pin & 0x0f));
// 		}
// 		else
// 		{
// 			PORTB &= ~(1<<(port_pin & 0x0f));
// 		}
// 		break;
		
// 	}
// }

// /*
// Return the phisical pin reading of the button
// */

// uint8_t pin_RdPin(uint16_t port_pin)
// {
// 	uint8_t ret = 0;
	
// 	switch (port_pin & 0xf0)
// 	{
// 		case PORT_C:
// 		if (PINC & (1<<(port_pin & 0x0f)))
// 		{
// 			ret = 1;
// 		}
// 		else
// 		{
// 			ret = 0;
// 		}
// 		break;
		
// 		case PORT_D:
// 		if (PIND & (1<<(port_pin & 0x0f)))
// 		{
// 			ret = 1;
// 		}
// 		else
// 		{
// 			ret = 0;
// 		}
// 		break;
		
// 		case PORT_B:
// 		if (PINB & (1<<(port_pin & 0x0f)))
// 		{
// 			ret = 1;
// 		}
// 		else
// 		{
// 			ret = 0;
// 		}
// 		break;
		
// 	}
// 	return ret;
// }



// void pin_SetSpecialFunct(uint16_t sel, uint8_t val)
// {
	
// }

