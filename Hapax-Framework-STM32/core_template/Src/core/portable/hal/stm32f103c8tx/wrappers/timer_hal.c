#include "timer_hal.h"
#include "interrupt_hal.h"
#include "stm32f1xx_ll_tim.h"

// If needed, try with ONe pulse for controlling the WS2812 LEDs
typedef void (*timer_hal_isr_cb)(timer_hal_irq_src_t);
static timer_hal_isr_cb timer_hal_ISR_cb[TIMER_TOTAL_INSTANCE];
static const timer_hal_conf_t *timer_hal_cfg_buff;
static const timer_hal_pwm_conf_t *pwm_hal_cfg_buff;
static const timer_hal_oc_conf_t *oc_hal_cfg_buff;
static timer_hal_err_t timers_ready = TIMER_HAL_ERR_INIT_PEND;
static TIM_ClockConfigTypeDef sClockSourceConfig[TIMER_TOTAL_INSTANCE] = {0};
static TIM_MasterConfigTypeDef sMasterConfig[TIMER_TOTAL_INSTANCE] = {0};
static TIM_OC_InitTypeDef sConfigOC[TIMER_TOTAL_INSTANCE] = {0};
static TIM_IC_InitTypeDef sConfigIC[TIMER_TOTAL_INSTANCE] = {0};
static TIM_HandleTypeDef tim[TIMER_TOTAL_INSTANCE];

enum {
	TIMER_1_HAL = 0,
	TIMER_2_HAL,
	TIMER_3_HAL,
	TIMER_4_HAL
};


static void timer4_isr()
{
    if (__HAL_TIM_GET_IT_SOURCE(&tim[TIMER_4_HAL], TIM_IT_CC1))
    {
        __HAL_TIM_CLEAR_IT(&tim[TIMER_4_HAL], TIM_IT_CC1);
        timer_hal_ISR_cb[TIMER_4_HAL](TIMER_HAL_CH1); // callback which will provide the calling hardware ID (i.e TIMER_4) to the application
    }
    else if (__HAL_TIM_GET_IT_SOURCE(&tim[TIMER_4_HAL], TIM_IT_CC2))
    {
        __HAL_TIM_CLEAR_IT(&tim[TIMER_4_HAL], TIM_IT_CC2);
        timer_hal_ISR_cb[TIMER_4_HAL](TIMER_HAL_CH2);
    }
    else if (__HAL_TIM_GET_IT_SOURCE(&tim[TIMER_4_HAL], TIM_IT_CC3))
    {
        __HAL_TIM_CLEAR_IT(&tim[TIMER_4_HAL], TIM_IT_CC3);
        timer_hal_ISR_cb[TIMER_4_HAL](TIMER_HAL_CH3);
    }
    else if (__HAL_TIM_GET_IT_SOURCE(&tim[TIMER_4_HAL], TIM_IT_CC4))
    {
        __HAL_TIM_CLEAR_IT(&tim[TIMER_4_HAL], TIM_IT_CC4);
        timer_hal_ISR_cb[TIMER_4_HAL](TIMER_HAL_CH4);
    }   
    else
    {
        // nothing and return
    }
}

static void timer3_isr()
{
    // metti cb
    if (__HAL_TIM_GET_IT_SOURCE(&tim[TIMER_3], TIM_IT_CC1))
    {
        __HAL_TIM_CLEAR_IT(&tim[TIMER_3], TIM_IT_CC1);
        timer_hal_ISR_cb[TIMER_3](TIMER_HAL_CH1); // callback which will provide the calling hardware ID (i.e TIMER_4) to the application
    }
    else if (__HAL_TIM_GET_IT_SOURCE(&tim[TIMER_3], TIM_IT_CC2))
    {
        __HAL_TIM_CLEAR_IT(&tim[TIMER_3], TIM_IT_CC2);
        timer_hal_ISR_cb[TIMER_3](TIMER_HAL_CH2);
    }
    else if (__HAL_TIM_GET_IT_SOURCE(&tim[TIMER_3], TIM_IT_CC3))
    {
        __HAL_TIM_CLEAR_IT(&tim[TIMER_3], TIM_IT_CC3);
        timer_hal_ISR_cb[TIMER_3](TIMER_HAL_CH3);
    }
    else if (__HAL_TIM_GET_IT_SOURCE(&tim[TIMER_3], TIM_IT_CC4))
    {
        __HAL_TIM_CLEAR_IT(&tim[TIMER_3], TIM_IT_CC4);
        timer_hal_ISR_cb[TIMER_3](TIMER_HAL_CH4);
    }   
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

static void timer_hal_error_handler(timer_hal_err_t *flag)
{
    *flag = TIMER_HAL_ERR_ERROR;
}

timer_hal_err_t Timer_hal_init(const timer_hal_conf_t *handle)
{
    timer_hal_err_t ret = TIMER_HAL_ERR_OK;
    timer_hal_cfg_buff = handle;

    for (int i = 0; i < TIMER_TOTAL_INSTANCE; i++)
    {
        if (handle[i].tmr != CONF_TIMER_ENUM_UNUSED)
        {
            tim[i].Instance = handle[i].periph; //TIM3;
            tim[i].Init.Prescaler = handle[i].presc;
            tim[i].Init.CounterMode = TIM_COUNTERMODE_UP; // default, change with LL
            tim[i].Init.Period = handle[i].period;
            tim[i].Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;            // default, chenge with LL
            tim[i].Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // default, change with LL if needed

            if (handle[i].periph == TIM1)
            {
                __HAL_RCC_TIM1_CLK_ENABLE();
                HAL_NVIC_SetPriority(TIM1_BRK_IRQn, 0, 0);
                HAL_NVIC_EnableIRQ(TIM1_BRK_IRQn);
                HAL_NVIC_SetPriority(TIM1_UP_IRQn, 0, 0);
                HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
                HAL_NVIC_SetPriority(TIM1_TRG_COM_IRQn, 0, 0);
                HAL_NVIC_EnableIRQ(TIM1_TRG_COM_IRQn);
                HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
                HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
            }
            else if (handle[i].periph == TIM2)
            {
                __HAL_RCC_TIM2_CLK_ENABLE();
                HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
                HAL_NVIC_EnableIRQ(TIM2_IRQn);
            }
            else if (handle[i].periph == TIM3)
            {
                __HAL_RCC_TIM3_CLK_ENABLE();
                HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
                HAL_NVIC_EnableIRQ(TIM3_IRQn);
            }
            else if (handle[i].periph == TIM4)
            {
                __HAL_RCC_TIM4_CLK_ENABLE();
                HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
                HAL_NVIC_EnableIRQ(TIM4_IRQn);
            }
            else
            {
                timer_hal_error_handler(&ret);
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
    int idx = 0;
    for (int i = 0; i < TIMER_TOTAL_INSTANCE; i++)
    {
        if (timer_hal_cfg_buff[i].tmr == tmr)
        {
            if (timer_hal_cfg_buff[i].periph == TIM1)
            {
                idx = TIMER_1_HAL;
            }
            else if (timer_hal_cfg_buff[i].periph == TIM2)
            {
                idx = TIMER_2_HAL;
            }
            else if (timer_hal_cfg_buff[i].periph == TIM3)
            {
                idx = TIMER_3_HAL;
            }
            else if (timer_hal_cfg_buff[i].periph == TIM4)
            {
                idx = TIMER_4_HAL;
            }
            else
            {
                break;
            }            
            timer_hal_ISR_cb[idx] = f_pt; // indexing with timer_hal_cfg_buff[i].tmr to have freedom in defining the order in conf.
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

timer_hal_err_t Timer_hal_OC_init(const timer_hal_oc_conf_t *handle)
{
    timer_hal_err_t ret = TIMER_HAL_ERR_OK;
    oc_hal_cfg_buff = handle;

    if (timers_ready != TIMER_HAL_ERR_OK)
    {
        return TIMER_HAL_ERR_NO_TIMER;
    }

    for (int i = 0; i < OC_TOTAL_INSTANCE; i++)
    {
        if (handle[i].tmr != CONF_TIMER_ENUM_UNUSED)
        {
            int j = 0;
            while (timer_hal_cfg_buff[j].tmr != handle[i].tmr)
            {
                // Find the right index in the timer conf, so can be used to
                // index the tim[] for the OC conf. Works based on the fact oc conf is always a subset of
                // timer conf for hardware reasons.
                if (j > TIMER_TOTAL_INSTANCE)
                    timer_hal_error_handler(&ret);
                j++;
            }
            if (HAL_TIM_OC_Init(&tim[j]) != HAL_OK) // ...hence search with j
            {
                timer_hal_error_handler(&ret);
            }
            // i is used only for the handle[], j for the internal timer structs.
            // handle[i] configures the j-th timer struct.
            sMasterConfig[j].MasterOutputTrigger = TIM_TRGO_RESET;
            sMasterConfig[j].MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
            if (HAL_TIMEx_MasterConfigSynchronization(&tim[j], &sMasterConfig[j]) != HAL_OK)
            {
                timer_hal_error_handler(&ret);
            }

            sConfigOC[j].OCMode = handle[i].mode;

            sConfigOC[j].Pulse = 0;
            sConfigOC[j].OCPolarity = handle[i].pol;

            sConfigOC[j].OCFastMode = TIM_OCFAST_DISABLE;
            if (HAL_TIM_OC_ConfigChannel(&tim[j], &sConfigOC[j], handle[i].channel) != HAL_OK)
            {
                timer_hal_error_handler(&ret);
            }

            if (handle[i].interrupt == TIMER_HAL_INT_ON)
            {
                switch (handle[i].channel)
                {
                case TIM_CHANNEL_1:
                    __HAL_TIM_ENABLE_IT(&tim[j], TIM_IT_CC1);
                    break;
                case TIM_CHANNEL_2:
                    __HAL_TIM_ENABLE_IT(&tim[j], TIM_IT_CC2);
                    break;
                case TIM_CHANNEL_3:
                    __HAL_TIM_ENABLE_IT(&tim[j], TIM_IT_CC3);
                    break;
                case TIM_CHANNEL_4:
                    __HAL_TIM_ENABLE_IT(&tim[j], TIM_IT_CC4);
                    break;
                default:
                    timer_hal_error_handler(&ret);
                    break;
                }

                if (timer_hal_cfg_buff[j].periph == TIM1)
                    IntHal_vector_register(timer1_isr, TIM1_IRQHandler_num);
                else if (timer_hal_cfg_buff[j].periph == TIM2)
                    IntHal_vector_register(timer2_isr, TIM2_IRQHandler_num);
                else if (timer_hal_cfg_buff[j].periph == TIM3)
                    IntHal_vector_register(timer3_isr, TIM3_IRQHandler_num);
                else if (timer_hal_cfg_buff[j].periph == TIM4)
                    IntHal_vector_register(timer4_isr, TIM4_IRQHandler_num);
                else
                {
                }
            }
        }
    }
    return ret;
}

void Timer_hal_OC_start(conf_oc_e oc)
{
    for (int i = 0; i < CONF_OC_ENUM_UNUSED; i++)
    {
        int j = 0;
        while (timer_hal_cfg_buff[j].tmr != oc_hal_cfg_buff[i].tmr)
        {
            if (j > TIMER_TOTAL_INSTANCE)
            {
                timer_hal_err_t *ret;          // for compliance
                timer_hal_error_handler(&ret); // for further error implementation
            }
            j++;
        }
        if (oc_hal_cfg_buff[i].oc_enum == oc)
        {
            HAL_TIM_OC_Start(&tim[j], oc_hal_cfg_buff[i].channel);
        }
    }
}

uint32_t Timer_hal_OC_get(conf_oc_e oc)
{
    int i = 0;
    // search the OC conf
    while (oc_hal_cfg_buff[i].oc_enum != oc)
    {
        if (i > CONF_OC_ENUM_UNUSED)
        {
            timer_hal_err_t *ret;          // for compliance
            timer_hal_error_handler(&ret); // for further error implementation
        }
        i++;
    }

    int j = 0;
    // search the corresponding timer
    while (oc_hal_cfg_buff[i].tmr != timer_hal_cfg_buff[j].tmr)
    {
        if (j > CONF_TIMER_ENUM_UNUSED)
        {
            timer_hal_err_t *ret;          // for compliance
            timer_hal_error_handler(&ret); // for further error implementation
        }
        j++;
    }
    return __HAL_TIM_GET_COMPARE(&tim[j], oc_hal_cfg_buff[i].channel);
}

void Timer_hal_OC_period(conf_oc_e oc, uint32_t val)
{
        int i = 0;
        // search the OC conf
        while (oc_hal_cfg_buff[i].oc_enum != oc)
        {
            if (i > CONF_OC_ENUM_UNUSED)
            {
                timer_hal_err_t *ret;          // for compliance
                timer_hal_error_handler(&ret); // for further error implementation
            }
            i++;
        }
        
        int j = 0;
        // search the corresponding timer
        while (oc_hal_cfg_buff[i].tmr != timer_hal_cfg_buff[j].tmr)
        {
            if (j > CONF_TIMER_ENUM_UNUSED)
            {
                timer_hal_err_t *ret;          // for compliance
                timer_hal_error_handler(&ret); // for further error implementation
            }
            j++;
        }
        __HAL_TIM_SET_COMPARE(&tim[j], oc_hal_cfg_buff[i].channel, val);
}

timer_hal_err_t Timer_hal_PWM_init(const timer_hal_pwm_conf_t *handle)
{
    timer_hal_err_t ret = TIMER_HAL_ERR_OK;
    pwm_hal_cfg_buff = handle;
    if (timers_ready != TIMER_HAL_ERR_OK)
    {
        return TIMER_HAL_ERR_NO_TIMER;
    }
    
    for (int i = 0; i < PWM_TOTAL_INSTANCE; i++)
    {
        
        if (handle[i].tmr != CONF_TIMER_ENUM_UNUSED)
        {
            int j = 0;
            while (timer_hal_cfg_buff[j].tmr != handle[i].tmr)
            {
                // Find the right index in the timer conf, so can be used to
                // index the tim[] for the OC conf. Works based on the fact oc conf is always a subset of
                // timer conf for hardware reasons.
                if (j > TIMER_TOTAL_INSTANCE)
                    timer_hal_error_handler(&ret);
                j++;
            }
            if (HAL_TIM_PWM_Init(&tim[j]) != HAL_OK)
            {
                timer_hal_error_handler(&ret);
            }
            sMasterConfig[j].MasterOutputTrigger = TIM_TRGO_RESET;
            sMasterConfig[j].MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
            if (HAL_TIMEx_MasterConfigSynchronization(&tim[j], &sMasterConfig[j]) != HAL_OK)
            {
                timer_hal_error_handler(&ret);
            }

            sConfigOC[j].OCMode = handle[i].mode; // default, change with LL

            sConfigOC[j].Pulse = 0;
            sConfigOC[j].OCPolarity = handle[i].pol;
            sConfigOC[j].OCFastMode = TIM_OCFAST_DISABLE;
            if (HAL_TIM_PWM_ConfigChannel(&tim[j], &sConfigOC[j], handle[i].channel) != HAL_OK)
            {
                timer_hal_error_handler(&ret);
            }
        }
    }
    return ret;
}

void Timer_hal_PWM_start(conf_pwm_e pwm)
{
    for (int i = 0; i < CONF_PWM_ENUM_UNUSED; i++)
    {
        int j = 0;
        while (timer_hal_cfg_buff[j].tmr != pwm_hal_cfg_buff[i].tmr)
        {
            if (j > TIMER_TOTAL_INSTANCE)
            {
                timer_hal_err_t *ret;          // for compliance
                timer_hal_error_handler(&ret); // for further error implementation
            }
            j++;
        }
        if (pwm_hal_cfg_buff[i].pwm_enum == pwm)
        {
            HAL_TIM_PWM_Start(&tim[j], pwm_hal_cfg_buff[i].channel);
            break;
        }
    }
}

void Timer_hal_PWM_DC(conf_pwm_e oc, uint32_t val)
{
        int i = 0;
        // search the OC conf
        while (pwm_hal_cfg_buff[i].pwm_enum != oc)
        {
            if (i > CONF_PWM_ENUM_UNUSED)
            {
                timer_hal_err_t *ret;          // for compliance
                timer_hal_error_handler(&ret); // for further error implementation
            }
            i++;
        }
        
        int j = 0;
        // search the corresponding timer
        while (pwm_hal_cfg_buff[i].tmr != timer_hal_cfg_buff[j].tmr)
        {
            if (j > CONF_TIMER_ENUM_UNUSED)
            {
                timer_hal_err_t *ret;          // for compliance
                timer_hal_error_handler(&ret); // for further error implementation
            }
            j++;
        }
        __HAL_TIM_SET_COMPARE(&tim[j], pwm_hal_cfg_buff[i].channel, val);
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
