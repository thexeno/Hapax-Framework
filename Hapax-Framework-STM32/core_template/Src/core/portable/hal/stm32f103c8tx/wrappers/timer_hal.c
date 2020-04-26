#include <wrappers/timer_hal.h>

  TIM_TypeDef* timer_base_translate[4] =
  {
		  TIM1,
		  TIM2,
		  TIM3,
		  TIM4
  };

static void timer_hal_error_handler(void)
{
    for(;;);
}

timer_hal_err_t Timer_hal_init(timer_hal_cfg_t *handle)
{
    //timer_hal_err_t ret = TMR_HAL_INIT_PEND;
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};
    TIM_IC_InitTypeDef sConfigIC = {0};
    TIM_HandleTypeDef tim;
    
    //timer_hal_cfg_buff[???] = handle;

    tim.Instance = timer_base_translate[handle->base];//TIM3;
    tim.Init.Prescaler = handle->presc;
    tim.Init.CounterMode = TIM_COUNTERMODE_UP;  // default, change with LL
    tim.Init.Period = handle->period;
    tim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2; // default, chenge with LL
    tim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // default, change with LL if needed

#warning "Interrupt are enabled my default"
#warning "TBD Interrupt selection"

    switch (handle->base)
    {

    case TIMER_1:
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

    case TIMER_2:
        __HAL_RCC_TIM2_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM2_IRQn);
        break;

    case TIMER_3:
        __HAL_RCC_TIM3_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
        break;

    case TIMER_4:
        __HAL_RCC_TIM4_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM4_IRQn);
        break;

    default:
        timer_hal_error_handler();
        break;
    }

    if (HAL_TIM_Base_Init(&tim) != HAL_OK)
    {
        timer_hal_error_handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL; // default, change with LL
    if (HAL_TIM_ConfigClockSource(&tim, &sClockSourceConfig) != HAL_OK)
    {
        timer_hal_error_handler();
    }

    if (handle->mode == TMR_INPUT_CAPT)
    {
        if (HAL_TIM_IC_Init(&tim) != HAL_OK)
        {
            timer_hal_error_handler();
        }
        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;       // default, change with LL
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE; // default, change with LL
        if (HAL_TIMEx_MasterConfigSynchronization(&tim, &sMasterConfig) != HAL_OK)
        {
            timer_hal_error_handler();
        }
        sConfigIC.ICPolarity = handle->polarity;//TIM_INPUTCHANNELPOLARITY_RISING;
        sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
        sConfigIC.ICPrescaler = TIM_ICPSC_DIV1; // default, change with LL
        sConfigIC.ICFilter = 0; // default, change with LL
        if (HAL_TIM_IC_ConfigChannel(&tim, &sConfigIC, handle->channel/*TIM_CHANNEL_1*/) != HAL_OK)
        {
            timer_hal_error_handler();
        }
    }
    else if (handle->mode == TMR_PWM)
    {
        if (HAL_TIM_PWM_Init(&tim) != HAL_OK)
        {
            timer_hal_error_handler();
        }
        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        if (HAL_TIMEx_MasterConfigSynchronization(&tim, &sMasterConfig) != HAL_OK)
        {
            timer_hal_error_handler();
        }
        sConfigOC.OCMode = TIM_OCMODE_PWM1; // default, change with LL
        sConfigOC.Pulse = 0;
        sConfigOC.OCPolarity = handle->polarity;//TIM_OCPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
        if (HAL_TIM_PWM_ConfigChannel(&tim, &sConfigOC, handle->channel) != HAL_OK)
        {
            timer_hal_error_handler();
        }
    }
    else if (handle->mode == TMR_OUTPUT_COMP)
    {
        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        if (HAL_TIMEx_MasterConfigSynchronization(&tim, &sMasterConfig) != HAL_OK)
        {
            timer_hal_error_handler();
        }
        sConfigOC.OCMode = TIM_OCMODE_TIMING; // default, change with LL
        sConfigOC.Pulse = 0;
        sConfigOC.OCPolarity = handle->polarity;//TIM_OCPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
        if (HAL_TIM_OC_ConfigChannel(&tim, &sConfigOC, handle->channel) != HAL_OK)
        {
            timer_hal_error_handler();
        }
    }
    else
    {
        timer_hal_error_handler();
    }
}
