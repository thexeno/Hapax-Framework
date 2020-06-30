#include "conf.h"
#include "sys_types.h"

#include "gpio_hal.h"
#include "timer_hal.h"


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
    //   hw pin,            pin mode,    hw port set,              after-init state,      additional options i.e.: pull modes
  { DEBUG_LED,                  GPIOC,     GPIO_PIN_13,          GPIO_MODE_OUTPUT_PP,    GPIO_PIN_SET,         GPIO_NOPULL}, 
  { PWM_PIN,                    GPIOB,     GPIO_PIN_6,           GPIO_MODE_OUTPUT_PP,        GPIO_PIN_RESET,       GPIO_NOPULL},
  { MCO_PIN,                    GPIOA,     GPIO_PIN_8,           GPIO_MODE_AF_PP,        GPIO_PIN_RESET,       GPIO_NOPULL},
  { CONF_GPIO_ENUM_UNUSED,      GPIOB,     GPIO_PIN_9,           GPIO_MODE_AF_PP,        GPIO_PIN_RESET,       GPIO_NOPULL},
  { CONF_GPIO_ENUM_UNUSED,      GPIOB,     GPIO_PIN_9,           GPIO_MODE_AF_PP,        GPIO_PIN_RESET,       GPIO_NOPULL},
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
    {TIMER_3,                   TIM3,   250,    0xffff},
    {TIMER_4,                   TIM4,   1,    0xffff}
};

const timer_hal_pwm_conf_t  pwm_test[PWM_TOTAL_ISTANCE] = 
{
    {PWM_CHANNEL_TEST_1, TIMER_3,   TIM_CHANNEL_1, TIM_OCMODE_PWM1,   TIM_OCPOLARITY_HIGH,     TIMER_HAL_INT_OFF}
    //{PWM_CHANNEL_TEST_2, TIMER_4,   TIM_CHANNEL_4, TMR_PWM,    TIM_OCPOLARITY_LOW,      TMR_INT_OFF},
    //{PWM_CHANNEL_TEST_3, TIMER_3,   TIM_CHANNEL_4, TMR_PWM,    TIM_OCPOLARITY_HIGH,     TMR_INT_OFF}
};

const timer_hal_oc_conf_t  oc_test[OC_TOTAL_ISTANCE] = 
{
    {OC_CHANNEL_TEST_1, TIMER_4,   TIM_CHANNEL_1, TIM_OCMODE_TIMING,    TIM_OCPOLARITY_HIGH,     TIMER_HAL_INT_ON}
};


const timer_hal_pwm_conf_t * const Timer_hal_PWM_conf_get(void)
{
    return (pwm_test/*[0]*/);
}

const timer_hal_oc_conf_t * const Timer_hal_OC_conf_get(void)
{
    return (oc_test/*[0]*/);
}

const timer_hal_conf_t * const Timer_hal_conf_get(void)
{
    return (timer_test/*[0]*/);
}



/*timer_hal_func_t timer_hal_test_conf[2] =
{
        {TIMER_4,    TMR_PWM,            TMR_POL_HIGH,     TMR_INT_OFF,    TMR_CH1},
        {TIMER_4,    TMR_COMP_TOGGLE,        TMR_POL_HIGH,     TMR_INT_ON,     TMR_CH4},
};
*/
