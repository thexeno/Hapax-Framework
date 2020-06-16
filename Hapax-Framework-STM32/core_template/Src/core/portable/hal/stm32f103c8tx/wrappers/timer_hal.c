#include "timer_hal.h"
#include "interrupt_hal.h"
#include "stm32f1xx_ll_tim.h"


// If needed, try with ONe pulse for controlling the WS2812 LEDs
typedef void (*timer_hal_isr_cb)(timer_hal_irq_src_t );
static timer_hal_isr_cb timer_hal_ISR_cb[TIMER_TOTAL_INSTANCE];
static timer_hal_conf_t *timer_hal_cfg_buff;
static pwm_hal_conf_t *pwm_hal_cfg_buff;
static oc_hal_conf_t *oc_hal_cfg_buff;
static timer_hal_err_t timers_ready = TIMER_HAL_ERR_INIT_PEND;

/* Translation tables: 
*  needed because TIMx is a pointer and cannot 
*  be used as index in the timer conf array and in a switch/case
*/

/* according to the HAL and CMSIS */
// TIM_TypeDef* timer_base_translate[TIMER_TOTAL] =
// {
// 	TIM1,
// 	TIM2,
// 	TIM3,
// 	TIM4
// };


static TIM_ClockConfigTypeDef sClockSourceConfig[TIMER_TOTAL_INSTANCE] = {0};
static TIM_MasterConfigTypeDef sMasterConfig[TIMER_TOTAL_INSTANCE] = {0};
static TIM_OC_InitTypeDef sConfigOC[TIMER_TOTAL_INSTANCE] = {0};
static TIM_IC_InitTypeDef sConfigIC[TIMER_TOTAL_INSTANCE] = {0};
static TIM_HandleTypeDef tim[TIMER_TOTAL_INSTANCE];


static void timer4_isr()
{
	// metti cb
    if  (__HAL_TIM_GET_IT_SOURCE(&tim[TIMER_4], TIM_IT_CC1))
    	timer_hal_ISR_cb[TIMER_4](TIMER_HAL_CH1); // callback which will provide the calling hardware ID (i.e TIMER_4) to the application
	
    else if (__HAL_TIM_GET_IT_SOURCE(&tim[TIMER_4], TIM_IT_CC2))
    	timer_hal_ISR_cb[TIMER_4](TIMER_HAL_CH2);
	
    else if (__HAL_TIM_GET_IT_SOURCE(&tim[TIMER_4], TIM_IT_CC3))
    	timer_hal_ISR_cb[TIMER_4](TIMER_HAL_CH3);
	
    else if (__HAL_TIM_GET_IT_SOURCE(&tim[TIMER_4], TIM_IT_CC4))
    	timer_hal_ISR_cb[TIMER_4](TIMER_HAL_CH4);
    
    else
    {
        // nothing and return
    }
}

static void timer3_isr()
{
	// metti cb
    if  (__HAL_TIM_GET_IT_SOURCE(&tim[TIMER_3], TIM_IT_CC1))
    	timer_hal_ISR_cb[TIMER_3](TIMER_HAL_CH1); // callback which will provide the calling hardware ID (i.e TIMER_4) to the application
	
    else if (__HAL_TIM_GET_IT_SOURCE(&tim[TIMER_3], TIM_IT_CC2))
    	timer_hal_ISR_cb[TIMER_3](TIMER_HAL_CH2);
	
    else if (__HAL_TIM_GET_IT_SOURCE(&tim[TIMER_3], TIM_IT_CC3))
    	timer_hal_ISR_cb[TIMER_3](TIMER_HAL_CH3);
	
    else if (__HAL_TIM_GET_IT_SOURCE(&tim[TIMER_3], TIM_IT_CC4))
    	timer_hal_ISR_cb[TIMER_3](TIMER_HAL_CH4);
    
    else
    {
        // nothing and return
    }
}

static void timer2_isr()
{
	// metti cb
//	timer_hal_ISR_cb[TIMER_2](TIMER_2); // callback a i.e. ser_rx_char_ISR(ser_dev_st* console)
	
}

static void timer1_isr()
{
	// metti cb
//	timer_hal_ISR_cb[TIMER_1](TIMER_1); // callback a i.e. ser_rx_char_ISR(ser_dev_st* console)
	
}

static void timer_hal_error_handler(timer_hal_err_t* flag)
{
    *flag = TIMER_HAL_ERR_ERROR;
}

timer_hal_err_t Timer_hal_init(timer_hal_conf_t *handle)
{
    timer_hal_err_t ret = TIMER_HAL_ERR_OK;
    timer_hal_cfg_buff = handle;

    for (int i = 0; i < TIMER_TOTAL_INSTANCE; i++)
    {
        if (handle[i].tmr != CONF_TIMER_ENUM_UNUSED)
        {
           tim[i].Instance = handle[i].periph;//TIM3;
           tim[i].Init.Prescaler = handle[i].presc;
           tim[i].Init.CounterMode = TIM_COUNTERMODE_UP;  // default, change with LL
           tim[i].Init.Period = handle[i].period;
           tim[i].Init.ClockDivision = TIM_CLOCKDIVISION_DIV2; // default, chenge with LL
           tim[i].Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // default, change with LL if needed

            switch (handle[i].periph)
                {

                case TIM1:
                    __HAL_RCC_TIM1_CLK_ENABLE();
                    HAL_NVIC_SetPriority(TIM1_BRK_IRQn, 0, 0);
                    HAL_NVIC_EnableIRQ(TIM1_BRK_IRQn);
                    HAL_NVIC_SetPriority(TIM1_UP_IRQn, 0, 0);
                    HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
                    HAL_NVIC_SetPriority(TIM1_TRG_COM_IRQn, 0, 0);
                    HAL_NVIC_EnableIRQ(TIM1_TRG_COM_IRQn);
                    HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
                    HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
                    break;

                case TIM2:
                    __HAL_RCC_TIM2_CLK_ENABLE();
                    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
                    HAL_NVIC_EnableIRQ(TIM2_IRQn);
                    break;

                case TIM3:
                    __HAL_RCC_TIM3_CLK_ENABLE();
                    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
                    HAL_NVIC_EnableIRQ(TIM3_IRQn);
                    break;

                case TIM4:
                    __HAL_RCC_TIM4_CLK_ENABLE();
                    HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
                    HAL_NVIC_EnableIRQ(TIM4_IRQn);
                    break;

                default:
                    timer_hal_error_handler(&ret);
                    break;
                }


            if (HAL_TIM_Base_Init(&tim[i]) != HAL_OK)
            {
                timer_hal_error_handler(&ret);
            }
            sClockSourceConfig[i].ClockSource = TIM_CLOCKSOURCE_INTERNAL; // default, change with LL
            if (HAL_TIM_ConfigClockSource(&tim[i], &sClockSourceConfig[i]) != HAL_OK)
            {
                timer_hal_error_handler(&ret);
            }
                
            if (HAL_TIM_OnePulse_Init((&tim[i]), TIM_OPMODE_REPETITIVE) != HAL_OK)
            {
                timer_hal_error_handler(&ret);
            }
        }
    }
    if (ret == TIMER_HAL_ERR_OK)
    {
        timers_ready = ret;
    }
    return ret;
}

/* With timer_hal_channel_t, the user function CB will be called from the ISR with the hardware channel 
 * responsible of the IRQ as parameter
 */ 
void Timer_hal_set_ISR_cb(conf_timer_e tmr, void (*f_pt)(timer_hal_irq_src_t))
{
    for (int i = 0; i < TIMER_TOTAL_INSTANCE; i++)
    {
        if (timer_hal_cfg_buff[i].tmr == tmr)
        {
            timer_hal_ISR_cb[i] = f_pt;
            break;
        }
    }
}

#if 0

void Timer_NotStd_hal_pulse_enable(timer_hal_conf_t handle)
{
    &tim[handle.periph]->Instance->CR1 |= TIM_OPMODE_SINGLE;
    //HAL_TIM_OnePulse_Start(&tim[handle.periph], timer_ch_translate[handle.channel]);
}

void Timer_NotStd_hal_pulse_disable(timer_hal_conf_t handle)
{
    &tim[handle.periph]->Instance->CR1 |= TIM_OPMODE_REPETITIVE;
    //HAL_TIM_OnePulse_Start(&tim[handle.periph], timer_ch_translate[handle.channel]);
}

#endif


// TIM_OCMODE_TIMING             
// TIM_OCMODE_ACTIVE             
// TIM_OCMODE_INACTIVE           
// TIM_OCMODE_TOGGLE             
// TIM_OCMODE_PWM1               
// TIM_OCMODE_PWM2               
// TIM_OCMODE_FORCED_ACTIVE      
// TIM_OCMODE_FORCED_INACTIVE    

timer_hal_err_t Timer_hal_OC_init(timer_hal_oc_t *handle)
{
    timer_hal_err_t ret = TIMER_HAL_ERR_OK;
    oc_hal_cfg_buff = handle;

    if (timers_ready != TIMER_HAL_ERR_OK)
    {
        return TIMER_HAL_ERR_NO_TIMER;
    }
    
    for (int i = 0; i < OC_TOTAL_ISTANCE; i++)
    {
        if (handle[i].tmr != CONF_OC_ENUM_UNUSED)
        {
            if (HAL_TIM_OC_Init(&tim[i]) != HAL_OK)
            {
                timer_hal_error_handler(&ret);
            }

            sMasterConfig[i].MasterOutputTrigger = TIM_TRGO_RESET;
            sMasterConfig[i].MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
            if (HAL_TIMEx_MasterConfigSynchronization(&tim[i], &sMasterConfig[i]) != HAL_OK)
            {
                timer_hal_error_handler(&ret);
            }

            sConfigOC[i].OCMode = handle[i].mode; 
                        
            sConfigOC[i].Pulse = 0;
            sConfigOC[i].OCPolarity = handle[i].pol;
            
            sConfigOC[i].OCFastMode = TIM_OCFAST_DISABLE;
            if (HAL_TIM_OC_ConfigChannel(&tim[i], &sConfigOC[i,  handle[i].channel) != HAL_OK)
            {
                timer_hal_error_handler(&ret);
            }

            if (handle[i].interrupt == TIMER_INT_ON)
            {
                switch (handle[i].channel)
                {
                    case TIM_CHANNEL_1:
                    __HAL_TIM_ENABLE_IT(&tim[i], TIM_IT_CC1);
                    break;
                    case TIM_CHANNEL_2:
                    __HAL_TIM_ENABLE_IT(&tim[i], TIM_IT_CC2);
                    break;
                    case TIM_CHANNEL_3:
                    __HAL_TIM_ENABLE_IT(&tim[i], TIM_IT_CC3);
                    break;
                    case TIM_CHANNEL_4:
                    __HAL_TIM_ENABLE_IT(&tim[i], TIM_IT_CC4);
                    break;
                    default:
                    timer_hal_error_handler(&ret);
                    break;
                }

                if (handle[i].periph == TIM1)
                    IntHal_vector_register(timer1_isr, TIM1_IRQHandler_num);
                else if (handle[i].periph == TIM2)
                    IntHal_vector_register(timer2_isr, TIM2_IRQHandler_num);                
                else if (handle[i].periph == TIM3)
                    IntHal_vector_register(timer3_isr, TIM3_IRQHandler_num);                
                else if (handle[i].periph == TIM4)
                    IntHal_vector_register(timer4_isr, TIM4_IRQHandler_num);                
                else {}
            }
        }
    }
    return ret;
}

void Timer_hal_OC_start(conf_oc_e oc)
{
    for (int i = 0; i<CONF_OC_ENUM_UNUSED; i++)
    {
        if (oc_hal_cfg_buff[i].oc_enum == oc)
        {
            HAL_TIM_OC_Start(&tim[i], oc_hal_cfg_buff[i].channel);
        }
    }
}

uint32_t Timer_hal_OC_get(conf_oc_e oc)
{
    for (int i = 0; i<CONF_OC_ENUM_UNUSED; i++)
    {
        if (oc_hal_cfg_buff[i].oc_enum == oc)
        {
            // If kept internal teh user might be unaware of the implication of other channels used, leadin to not synch
            return __HAL_TIM_GET_COMPARE(&tim[i], oc_hal_cfg_buff[i].channel);
        }
    }
}

void Timer_hal_OC_period(conf_oc_e oc)
{
    for (int i = 0; i<CONF_OC_ENUM_UNUSED; i++)
    {
        if (oc_hal_cfg_buff[i].oc_enum == oc)
        {
            // If kept internal teh user might be unaware of the implication of other channels used, leadin to not synch
            return __HAL_TIM_GET_COMPARE(&tim[i], oc_hal_cfg_buff[i].channel);
        }
    }
}


void Timer_hal_OC_period(timer_hal_oc_t *handle, conf_oc_e oc, uint32_t val)
{
    for (int i = 0; i<CONF_OC_ENUM_UNUSED; i++)
    {
        if (oc_hal_cfg_buff[i].oc_enum == oc)
        {
            __HAL_TIM_SET_COMPARE(&tim[i], oc_hal_cfg_buff[i].channel, val);
             //                     (__HAL_TIM_GET_COMPARE(&tim[handle->periph], timer_ch_translate[handle->channel])+val)
               //                   );
            //__HAL_TIM_DISABLE(&tim[handle->periph]);
            //__HAL_TIM_SET_COMPARE(&tim[handle->periph], timer_ch_translate[handle->channel],
              //                   (__HAL_TIM_GET_COUNTER(&tim[handle->periph])+val));
            //__HAL_TIM_ENABLE(&tim[handle->periph]);     
        }
    }             
}           


timer_hal_err_t Timer_hal_PWM_init(timer_hal_pwm_t *handle)
{
    timer_hal_err_t ret = TIMER_HAL_ERR_OK;
    pwm_hal_cfg_buff = handle;
    if (timers_ready != TIMER_HAL_ERR_OK)
    {
        return TIMER_HAL_ERR_NO_TIMER;
    }
    for (int i = 0; i < TIMER_TOTAL_INSTANCE; i++)
    {
        if (handle[i].tmr != CONF_TIMER_ENUM_UNUSED)
        {
           if (HAL_TIM_PWM_Init(&tim[i]) != HAL_OK)
           {
               timer_hal_error_handler(&ret);
           }
           sMasterConfig[i].MasterOutputTrigger = TIM_TRGO_RESET;
           sMasterConfig[i].MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
           if (HAL_TIMEx_MasterConfigSynchronization(&tim[i], &sMasterConfig[i]) != HAL_OK)
           {
               timer_hal_error_handler(&ret);
           }
           if (handle->mode == TIMER_PWM)
               sConfigOC[i].OCMode = TIM_OCMODE_PWM1; // default, change with LL
           else if (handle->mode == TIMER_PWM_INV)
               sConfigOC[i].OCMode = TIM_OCMODE_PWM2; // default, change with LL
           else
           {
               timer_hal_error_handler(&ret);
           }
           sConfigOC[i].Pulse = 0;
           if (handle->pol == TIMER_POL_HIGH)
               sConfigOC[i].OCPolarity = TIM_OCPOLARITY_HIGH;
           if (handle->pol == TIMER_POL_LOW)
               sConfigOC[i].OCPolarity = TIM_OCPOLARITY_LOW;
           sConfigOC[i].OCFastMode = TIM_OCFAST_DISABLE;
           if (HAL_TIM_PWM_ConfigChannel(&tim[i], &sConfigOC[i], handle[i].channel) != HAL_OK)
           {
               timer_hal_error_handler(&ret);
           }
        }
    }
    return ret;
}





void Timer_hal_PWM_start(conf_pwm_e pwm)
{
    for (int i = 0; i<CONF_PWM_ENUM_UNUSED; i++)
    {
        if (pwm_hal_cfg_buff[i].pwm_enum == pwm)
        {
            HAL_TIM_PWM_Start(&tim[i], pwm_hal_cfg_buff[i].channel);
            break;
        }
    }
}


void Timer_hal_PWM_DC(timer_hal_pwm_t *handle, conf_pwm_e pwm, uint32_t val)
{
    #if 0
    if (handle->pwm_enum == pwm) // in this way can also passed the pointer to the right handler only for faster assignment
    {
        switch (handle->channel)
        {
            case TIM_CHANNEL_1:
            LL_TIM_OC_SetCompareCH1(tim[i].Instance, val);
            break;
            case TIM_CHANNEL_2:
            LL_TIM_OC_SetCompareCH2(tim[i].Instance, val);
            break;
            case TIM_CHANNEL_3:
            LL_TIM_OC_SetCompareCH3(tim[i].Instance, val);
            break;
            case TIM_CHANNEL_4:
            LL_TIM_OC_SetCompareCH4(tim[i].Instance, val);
            break;   
            default:
            break;
        }       
    }
    else // the argument passed was not the exact handle, and must be searched instead
    {
    #endif
        for (int i = 0; i<CONF_PWM_ENUM_UNUSED; i++)
        {
            if (pwm_hal_cfg_buff[i].pwm_enum == pwm)
            {
                switch (pwm_hal_cfg_buff[i].channel)
                {
                    case TIM_CHANNEL_1:
                    LL_TIM_OC_SetCompareCH1(tim[i].Instance, val);
                    break;
                    case TIM_CHANNEL_2:
                    LL_TIM_OC_SetCompareCH2(tim[i].Instance, val);
                    break;
                    case TIM_CHANNEL_3:
                    LL_TIM_OC_SetCompareCH3(tim[i].Instance, val);
                    break;
                    case TIM_CHANNEL_4:
                    LL_TIM_OC_SetCompareCH4(tim[i].Instance, val);
                    break;   
                    default:
                    //timer_hal_error_handler((timer_hal_err_t*)void);
                    break;
                }
            }
        }
    #if 0
    }
    #endif
}


// perc*ARR / 100 = dc-val
//void Timer_NotStd_hal_pulse_duty(timer_hal_func_t *handle, uint32_t duty)
//{
//    __HAL_TIM_SET_COMPARE(&tim[handle->periph], timer_ch_translate[handle->channel], duty);
// }

// timer_hal_err_t Timer_NotStd_hal_pulse_init(timer_hal_func_t *handle)
// {
//     timer_hal_err_t ret = TIMER_HAL_ERR_OK;
//     if (handle->channel == TIMER_CH3 || handle->channel == TIMER_CH4)
//     {
//         timer_hal_error_handler(&ret);
//     }
//     if (Timer_hal_init(timer_hal_conf_t *handle) != TIMER_HAL_ERR_OK) // because affects the whole timer
//     {
//         timer_hal_error_handler(&ret);
//     }
//     if (HAL_TIM_OnePulse_Init((&tim[handle->periph]), TIM_OPMODE_SINGLE) != HAL_OK)
//     {
//         timer_hal_error_handler(&ret);
//     }
//     return ret;
// }



//     if (handle->mode == TIMER_INPUT_CAPT)
//     {
//         if (HAL_TIM_IC_Init(&tim[handle->periph]) != HAL_OK)
//         {
//             timer_hal_error_handler();
//         }
//         sMasterConfig[handle->periph].MasterOutputTrigger = TIM_TRGO_RESET;       // default, change with LL
//         sMasterConfig[handle->periph].MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE; // default, change with LL
//         if (HAL_TIMEx_MasterConfigSynchronization(&tim[handle->periph], &sMasterConfig[handle->periph]) != HAL_OK)
//         {
//             timer_hal_error_handler();
//         }
//         sConfigIC[handle->periph].ICPolarity = handle->polarity;//TIM_INPUTCHANNELPOLARITY_RISING;
//         sConfigIC[handle->periph].ICSelection = TIM_ICSELECTION_DIRECTTI;
//         sConfigIC[handle->periph].ICPrescaler = TIM_ICPSC_DIV1; // default, change with LL
//         sConfigIC[handle->periph].ICFilter = 0; // default, change with LL
//         if (HAL_TIM_IC_ConfigChannel(&tim[handle->periph], &sConfigIC[handle->periph], handle->channel/*TIM_CHANNEL_1*/) != HAL_OK)
//         {
//             timer_hal_error_handler();
//         }
//     }
//     else if (handle->mode == TIMER_PWM)
//     {
 
//     }
//     else if (handle->mode == TIMER_OUTPUT_COMP)
//     {
//         sMasterConfig[handle->periph].MasterOutputTrigger = TIM_TRGO_RESET;
//         sMasterConfig[handle->periph].MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//         if (HAL_TIMEx_MasterConfigSynchronization(&tim[handle->periph], &sMasterConfig[handle->periph]) != HAL_OK)
//         {
//             timer_hal_error_handler();
//         }
//         sConfigOC[handle->periph].OCMode = TIM_OCMODE_TIMING; // default, change with LL
//         sConfigOC[handle->periph].Pulse = 0;
//         sConfigOC[handle->periph].OCPolarity = handle->polarity;//TIM_OCPOLARITY_HIGH;
//         sConfigOC[handle->periph].OCFastMode = TIM_OCFAST_DISABLE;
//         if (HAL_TIM_OC_ConfigChannel(&tim[handle->periph], &sConfigOC[handle->periph], handle->channel) != HAL_OK)
//         {
//             timer_hal_error_handler();
//         }
//     }
//     else
//     {
//         timer_hal_error_handler();
//     }
// }
