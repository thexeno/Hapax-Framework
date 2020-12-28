#include "gpio_hal.h"
#include "stm32f1xx.h"

const gpio_hal_cfg_t *gpio_hal_cfg_buff;
base_t total_pins = 0;

gpio_hal_err_t Gpio_hal_write_value(conf_gpio_e pin_enum, gpio_hal_val_t val, gpio_hal_mask_t mask) // per il gpio_hal_val_t metti un redefine di quello ufficiale stm32
{
    // This is NOT atomic.
    base_t i = 0;
    for (i = 0; i < total_pins; i++)
    {
        if (gpio_hal_cfg_buff[i].pin_enum != CONF_GPIO_ENUM_UNUSED)
        {
            // can be optimized with LOCAL INDICIZATION!, or use the fast, non portable approach
            if (gpio_hal_cfg_buff[i].pin_enum == pin_enum)
            {
                if (val == GPIO_HAL_VAL_FALSE)
                {
                    gpio_hal_cfg_buff[i].port->ODR &= ~((uint32_t)gpio_hal_cfg_buff[i].pin & mask);
                }
                else
                {
                    gpio_hal_cfg_buff[i].port->ODR |= ((uint32_t)gpio_hal_cfg_buff[i].pin & mask);
                }
                return GPIO_OK;
            }
        }
    }
    return GPIO_ERR_PIN;
}

gpio_hal_val_t Gpio_hal_read_value(conf_gpio_e pin_enum, gpio_hal_mask_t mask)
{
    base_t i = 0;

    for (i = 0; i < total_pins; i++)
    {
        if (gpio_hal_cfg_buff[i].pin_enum != CONF_GPIO_ENUM_UNUSED)
        {
            // can be optimized with LOCAL INDICIZATION!, or use the fast, non portable approach
            if (gpio_hal_cfg_buff[i].pin_enum == pin_enum)
            {
                if (mask == GPIO_HAL_MASK_AND_UNUSED)
                {
                    if ((gpio_hal_cfg_buff[i].port)->IDR & gpio_hal_cfg_buff[i].pin)
                    {
                        return GPIO_HAL_VAL_TRUE;
                    }
                    else
                    {
                        return GPIO_HAL_VAL_FALSE;
                    }
                }
                else
                {
                    if (((gpio_hal_cfg_buff[i].port)->IDR & gpio_hal_cfg_buff[i].pin & mask) == mask)
                    {
                        return GPIO_HAL_VAL_TRUE;
                    }
                    else
                    {
                        return GPIO_HAL_VAL_FALSE;
                    }
                }
            }
        }
    }
    return GPIO_HAL_VAL_UNDEF;
}

void Gpio_hal_init(const gpio_hal_cfg_t *handle)
{
    uint8_t i = 0;
    uint32_t shift = 0;
    uint32_t *port_pt = 0;
    //GPIO_InitTypeDef GPIO_InitStruct = {0};

    gpio_hal_cfg_buff = handle;
    total_pins = Gpio_hal_conf_get_size();

    for (i = 0; i < total_pins; i++)
    {
        /* Don't exclude the CONF_GPIO_ENUM_UNUSED on purpose */
        if ((gpio_hal_port_t)handle[i].port == GPIOA)
            SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN);
        if ((gpio_hal_port_t)handle[i].port == GPIOB)
            SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN);
        if ((gpio_hal_port_t)handle[i].port == GPIOC)
            SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPCEN);
        if ((gpio_hal_port_t)handle[i].port == GPIOD)
            SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPDEN);

        // cannot use yet the encapsulated GPIO manipulation function
        if (handle[i].val == GPIO_HAL_VAL_FALSE)
        {
            handle[i].port->BSRR = (uint32_t)handle[i].pin << 16;
        }
        else
        {
            handle[i].port->BSRR = (uint32_t)handle[i].pin;
        }

        while (((handle[i].pin >> shift) & (uint32_t)1) == 0 || (shift < 15))
        {
            shift++;
        }

        if (shift < 8)
        {
            // metti switch in out, e tipo di drive secondo datasheet
            port_pt = (uint32_t *)handle[i].port->CRL;
        }
        else
        {
            port_pt = (uint32_t *)handle[i].port->CRH;
        }

        switch (handle[i].mode_io)
        {
        case GPIO_HAL_MODE_OUT:                                                                                                       // poi prova a farlo in un solo modify_reg
            MODIFY_REG(*port_pt, GPIO_CRL_MODE0_Msk << (2 * (shift + 1)), (GPIO_CRL_MODE & 3UL) << (2 * (shift + 1))); // 3UL max speed, 50 MHz
            MODIFY_REG(*port_pt, GPIO_CRL_CNF0_Msk << (shift + 1), (GPIO_CRL_CNF & 0UL) << (2 * (shift + 1)));                        // 0 push-pull
            break;
        case GPIO_HAL_MODE_MUX:
                MODIFY_REG(*port_pt, GPIO_CRL_MODE0_Msk << (2 * (shift + 1)), (GPIO_CRL_MODE & 3UL) << (2 * (shift + 1))); // 3UL max speed, 50 MHz
            MODIFY_REG(*port_pt, GPIO_CRL_CNF0_Msk << (shift + 1), (GPIO_CRL_CNF & 2UL) << (2 * (shift + 1)));                        // 2 AF push-pull
        break;
        case GPIO_HAL_MODE_IN:
        default:
            MODIFY_REG(*port_pt, GPIO_CRL_MODE0_Msk << (2 * (shift + 1)), (GPIO_CRL_MODE & 0UL) << (2 * (shift + 1))); // 0 input
            MODIFY_REG(*port_pt, GPIO_CRL_CNF0_Msk << (shift + 1), (GPIO_CRL_CNF & 1UL) << (2 * (shift + 1)));         // 1UL float input
            break;
        }

        //HAL_GPIO_Init(handle[i].port, &GPIO_InitStruct);
    }
}
