#include "conf.h"
#include "sys_types.h"

#include "gpio_hal.h"
#include "timer_hal.h"
#include "clock_hal.h"
#include "spi_hal.h"


//#define LSE_VALUE


const clk_hal_conf_t clk_hal_conf =
{
    //   hw pin,            pin mode,    hw port set,              after-init state,      additional options i.e.: pull modes
    CONF_AHB_FREQ, CONF_APB1_FREQ, CONF_APB2_FREQ, CLK_HAL_SOURCE_HSI
};

const clk_hal_conf_t *  Clock_hal_conf_get(void)
{
    return (&clk_hal_conf);
}

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
    //   hw pin,            pin mode,    hw port set,              after-init state,      additional options i.e.: pull modes
  { DEBUG_LED,                  GPIO_HAL_PORTC,     GPIO_HAL_PIN_13,          GPIO_HAL_MODE_OUT,        GPIO_HAL_VAL_TRUE,  }, 
  { PWM_PIN,                    GPIO_HAL_PORTA,     GPIO_HAL_PIN_6,           GPIO_HAL_MODE_MUX,        GPIO_HAL_VAL_FALSE},
  { MCO_PIN,                    GPIO_HAL_PORTA,     GPIO_HAL_PIN_8,           GPIO_HAL_MODE_MUX,        GPIO_HAL_VAL_FALSE},
  { CONF_GPIO_ENUM_UNUSED,      GPIO_HAL_PORTB,     GPIO_HAL_PIN_9,           GPIO_HAL_MODE_MUX,        GPIO_HAL_VAL_FALSE},
  { CONF_GPIO_ENUM_UNUSED,      GPIO_HAL_PORTB,     GPIO_HAL_PIN_9,           GPIO_HAL_MODE_MUX,        GPIO_HAL_VAL_FALSE},
}; 

const gpio_hal_cfg_t * const Gpio_hal_conf_get(void)
{
    return (gpio_hal_conf/*[0]*/);
}

base_t Gpio_hal_conf_get_size(void)
{
    return (sizeof(gpio_hal_conf)/sizeof(gpio_hal_conf[0]));
}

const spi_hal_conf_t spi_hal_conf[SPI_TOTAL_INSTANCE] =
{
  { .spi = SPI_TEST_0,  .periph = SPI_HAL_1,     .mode = SPI_HAL_MODE_MASTER,    .baud = 100000, .dsize = SPI_HAL_WORD_8BIT,   
    .cpha = SPI_HAL_PHA_1EDGE, .cpol = SPI_HAL_POL_IDLE_HIGH} 
}; 

const gpio_hal_cfg_t * const Gpio_hal_conf_get(void)
{
    return (gpio_hal_conf/*[0]*/);
}

base_t Gpio_hal_conf_get_size(void)
{
    return (sizeof(gpio_hal_conf)/sizeof(gpio_hal_conf[0]));
}

/*
    // Pin enumerator,  
   DEBUG_LED,         
   TIMER4_TEST_PIN0,  
   MCO_PIN,           
   TIMER4_TEST_PIN1,  
*/

const timer_hal_conf_t timer_test[TIMER_TOTAL_INSTANCE] =
{
    {CONF_TIMER_ENUM_UNUSED,    TIM1,   1,    0xffff},
    {CONF_TIMER_ENUM_UNUSED,    TIM2,   1,    0xffff},
    {TIMER_3,                   TIM3,   250,    0x00ff},
    {TIMER_4,                   TIM4,   1,    0xffff}
};

const timer_hal_pwm_conf_t  pwm_test[PWM_TOTAL_INSTANCE] =
{
    {PWM_CHANNEL_TEST_1, TIMER_3,   TIM_CHANNEL_1, TIM_OCMODE_PWM1,   TIM_OCPOLARITY_HIGH,     TIMER_HAL_INT_OFF}
    //{PWM_CHANNEL_TEST_2, TIMER_4,   TIM_CHANNEL_4, TMR_PWM,    TIM_OCPOLARITY_LOW,      TMR_INT_OFF},
    //{PWM_CHANNEL_TEST_3, TIMER_3,   TIM_CHANNEL_4, TMR_PWM,    TIM_OCPOLARITY_HIGH,     TMR_INT_OFF}
};

const timer_hal_oc_conf_t  oc_test[OC_TOTAL_INSTANCE] =
{
    {OC_CHANNEL_TEST_1, TIMER_4,   TIM_CHANNEL_1, TIM_OCMODE_TIMING,    TIM_OCPOLARITY_HIGH,     TIMER_HAL_INT_ON}
};


const timer_hal_pwm_conf_t * const Timer_hal_PWM_conf_get(void)
{
    return (pwm_test/*[0]*/);
}
base_t Timer_hal_PWM_conf_get_size(void)
{
    return (sizeof(pwm_test)/sizeof(pwm_test[0]));
}

const timer_hal_oc_conf_t * const Timer_hal_OC_conf_get(void)
{
    return (oc_test/*[0]*/);
}
base_t Timer_hal_OC_conf_get_size(void)
{
    return (sizeof(oc_test)/sizeof(oc_test[0]));
}

const timer_hal_conf_t * const Timer_hal_conf_get(void)
{
    return (timer_test/*[0]*/);
}
base_t Timer_hal_conf_get_size(void)
{
    return (sizeof(timer_test)/sizeof(timer_test[0]));
}


/*timer_hal_func_t timer_hal_test_conf[2] =
{
        {TIMER_4,    TMR_PWM,            TMR_POL_HIGH,     TMR_INT_OFF,    TMR_CH1},
        {TIMER_4,    TMR_COMP_TOGGLE,        TMR_POL_HIGH,     TMR_INT_ON,     TMR_CH4},
};
*/
