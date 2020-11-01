#include <wrappers/gpio_hal.h>
#include "conf.h"
#include "gpio.h"
#include "pin.h"

const gpio_hal_cfg_t *gpio_hal_cfg_buff;
base_t total_pins = 0;

//*****************************************************************************
// PIN to PAD matrix replicated here for simpler conf
//*****************************************************************************
static const unsigned long gpio_hal_PinToPadMap[64] =
{
	10,11,12,13,14,15,16,17,255,255,18,
	19,20,21,22,23,24,40,28,29,25,255,
	255,255,255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,255,255,
	31,255,255,255,255,0,255,32,30,255,1,
	255,2,3,4,5,6,7,8,9
};
//****************************************************************************
//                      GLOBAL VARIABLES                (taken form the _IF example from the SDK)                   
//****************************************************************************
static unsigned long gpio_hal_Reg[]=
{
    GPIOA0_BASE,
    GPIOA1_BASE,
    GPIOA2_BASE,
    GPIOA3_BASE,
    GPIOA4_BASE
};

void gpio_hal_cc3200Custom_gpiodata_get(uint8_t gpio_pin_n, unsigned long *gbase, uint8_t *bitpack)
{
    unsigned long gpio = 0;
    //
    // Get the GPIO pin from the external Pin number
    //
    *bitpack = 1 << (gpio_pin_n % 8);

    //
    // Get the GPIO port from the external Pin number
    //
    *gbase = (gpio_pin_n / 8);
    *gbase = gpio_hal_Reg[*gbase];

}


gpio_hal_err_t Gpio_hal_write_value(conf_gpio_e pin_enum, gpio_hal_val_t val) // per il gpio_hal_val_t metti un redefine di quello ufficiale stm32
{
    unsigned long port;
    uint8_t bit_pack;
    unsigned long gpio_pin = 0;
    base_t i = 0;
    
	for(i = 0; i < total_pins; i++)
	{
        if (gpio_hal_cfg_buff[i].pin_enum != CONF_GPIO_ENUM_UNUSED)
        {
            // can be optimized with LOCAL INDICIZATION!, or use the fast, non portable approach
            if (gpio_hal_cfg_buff[i].pin_enum == pin_enum)
            {
                gpio_pin = gpio_hal_PinToPadMap[gpio_hal_cfg_buff[i].pin];
                gpio_hal_cc3200Custom_gpiodata_get(gpio_pin, &port, &bit_pack);
                //val = val << bit_pack;
                MAP_GPIOPinWrite(port, bit_pack, bit_pack & val);  // with bit pack you can select how many bit to retrieve. To be used in the parallel access API.
                                                        // This will return a single pin value.
                return GPIO_OK;
            }
        }
	}
	return GPIO_ERR_PIN;
}

gpio_hal_val_t Gpio_hal_read_value(conf_gpio_e pin_enum)
{
    gpio_hal_port_t port;
    uint8_t bit_pack;
    unsigned long gpio_pin = 0;
	gpio_hal_val_t ret = GPIO_HAL_VAL_FALSE;
    base_t i = 0;

	for(i = 0; i < total_pins; i++)
	{
        if (gpio_hal_cfg_buff[i].pin_enum != CONF_GPIO_ENUM_UNUSED)
        {
            // can be optimized with LOCAL INDICIZATION!, or use the fast, non portable approach
            if (gpio_hal_cfg_buff[i].pin_enum == pin_enum)
            {
                gpio_pin = gpio_hal_PinToPadMap[gpio_hal_cfg_buff[i].pin];
                gpio_hal_cc3200Custom_gpiodata_get(gpio_pin, &port, &bit_pack);
                return (gpio_hal_val_t)MAP_GPIOPinRead(port, bit_pack);
            }
        }
    }
	return (gpio_hal_val_t)GPIO_ERR_PIN;
}

void Gpio_hal_parallel_write_val(gpio_hal_port_t port, gpio_hal_port_t val)
{
//    gpio_hal_cc3200Custom_gpiodata_get(gpio_hal_cfg_buff[i].pin, &port, &bit_pack);
	MAP_GPIOPinWrite(port, 0xFF, val);
}

gpio_hal_port_t Gpio_hal_parallel_read_val(gpio_hal_port_t port, gpio_hal_port_t val)
{
	return (gpio_hal_port_t)MAP_GPIOPinRead(port, 0xFF); // 0xFF enable all the bit of the port to be read
}


// Table 5-14. GPIO Mapping
void Gpio_hal_init(const gpio_hal_cfg_t* handle)
{
	uint8_t i = 0;
    unsigned long gpio_base = 0;
    unsigned long gpio_pin = 0;
    uint8_t byte_bitpack = 0;

	gpio_hal_cfg_buff = handle;
	total_pins = Gpio_hal_conf_get_size();

	for (i = 0; i < total_pins; i++)
	{
		//PRCMPeripheralReset(unsigned long ulPeripheral), TBD
	}

	for (i = 0; i < total_pins ; i++)
	{
        gpio_pin = gpio_hal_PinToPadMap[handle[i].pin];
        gpio_hal_cc3200Custom_gpiodata_get(gpio_pin, &gpio_base, &byte_bitpack);

        if ((gpio_hal_port_t)gpio_base == GPIOA0_BASE)
        {
			MAP_PRCMPeripheralClkEnable(PRCM_GPIOA0, PRCM_RUN_MODE_CLK);
        }
        else if ((gpio_hal_port_t)gpio_base == GPIOA1_BASE)
        {
			MAP_PRCMPeripheralClkEnable(PRCM_GPIOA1, PRCM_RUN_MODE_CLK);
        }
        else if ((gpio_hal_port_t)gpio_base == GPIOA2_BASE)
        {
			MAP_PRCMPeripheralClkEnable(PRCM_GPIOA2, PRCM_RUN_MODE_CLK);
        }
        else if ((gpio_hal_port_t)gpio_base == GPIOA3_BASE)
        {
			MAP_PRCMPeripheralClkEnable(PRCM_GPIOA3, PRCM_RUN_MODE_CLK);
        }
        else 
        {
            /* empty */
        }

        // pin mode è il pinmux Table 16-7. Pin Multiplexing
        MAP_PinTypeGPIO(handle[i].pin, PIN_MODE_0, handle[i].mode_pull);   // PIN_MODE_0 equivalent of pin mode: gpio
		MAP_GPIODirModeSet(gpio_base, byte_bitpack, handle[i].mode_io);
        Gpio_hal_write_value(handle[i].pin_enum, handle[i].val);
	}
}


