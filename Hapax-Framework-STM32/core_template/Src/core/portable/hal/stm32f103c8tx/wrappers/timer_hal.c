#include <wrappers/timer_hal.h>
#include "stm32f1xx_ll_tim.h"

#define TIMER_TOTAL 4U

  TIM_TypeDef* timer_base_translate[TIMER_TOTAL] =
  {
		  TIM1,
		  TIM2,
		  TIM3,
		  TIM4
  };

TIM_ClockConfigTypeDef sClockSourceConfig[TIMER_TOTAL] = {0};
TIM_MasterConfigTypeDef sMasterConfig[TIMER_TOTAL] = {0};
TIM_OC_InitTypeDef sConfigOC[TIMER_TOTAL] = {0};
TIM_IC_InitTypeDef sConfigIC[TIMER_TOTAL] = {0};
TIM_HandleTypeDef tim[TIMER_TOTAL];

static void timer_hal_error_handler(timer_hal_err_t* flag)
{
    *flag = TMR_HAL_ERR;
}

static timer_hal_err_t timer_hal_init(timer_hal_cfg_t *handle)
{
    timer_hal_err_t ret = TMR_HAL_OK;
    tim[handle->periph].Instance = timer_base_translate[handle->periph];//TIM3;
    tim[handle->periph].Init.Prescaler = handle->presc;
    tim[handle->periph].Init.CounterMode = TIM_COUNTERMODE_UP;  // default, change with LL
    tim[handle->periph].Init.Period = handle->period;
    tim[handle->periph].Init.ClockDivision = TIM_CLOCKDIVISION_DIV2; // default, chenge with LL
    tim[handle->periph].Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // default, change with LL if needed

#warning "Interrupt are enabled my default"
#warning "TBD Interrupt selection"

    switch (handle->periph)
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
        timer_hal_error_handler(&ret);
        break;
    }

    if (HAL_TIM_Base_Init(&tim[handle->periph]) != HAL_OK)
    {
        timer_hal_error_handler(&ret);
    }
    sClockSourceConfig[handle->periph].ClockSource = TIM_CLOCKSOURCE_INTERNAL; // default, change with LL
    if (HAL_TIM_ConfigClockSource(&tim[handle->periph], &sClockSourceConfig[handle->periph]) != HAL_OK)
    {
        timer_hal_error_handler(&ret);
    }
    return ret;
}

timer_hal_err_t Timer_hal_PWM_init(timer_hal_cfg_t *handle)
{
    timer_hal_err_t ret = TMR_HAL_OK;
    if (timer_hal_init(handle) != TMR_HAL_OK)
    {
        return TMR_HAL_ERR;
    }

    if (HAL_TIM_PWM_Init(&tim[handle->periph]) != HAL_OK)
    {
        timer_hal_error_handler(&ret);
    }

    sMasterConfig[handle->periph].MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig[handle->periph].MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&tim[handle->periph], &sMasterConfig[handle->periph]) != HAL_OK)
    {
        timer_hal_error_handler(&ret);
    }

    if (handle->mode == TMR_PWM)
        sConfigOC[handle->periph].OCMode = TIM_OCMODE_PWM1; // default, change with LL
    else if (handle->mode == TMR_PWM_INV)
        sConfigOC[handle->periph].OCMode = TIM_OCMODE_PWM2; // default, change with LL
    else
    {
        timer_hal_error_handler(&ret);
    }
    
    sConfigOC[handle->periph].Pulse = 0;
    sConfigOC[handle->periph].OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC[handle->periph].OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&tim[handle->periph], &sConfigOC[handle->periph], handle->channel) != HAL_OK)
    {
        timer_hal_error_handler(&ret);
    }
    return ret;
}


//     if (handle->mode == TMR_INPUT_CAPT)
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
//     else if (handle->mode == TMR_PWM)
//     {
 
//     }
//     else if (handle->mode == TMR_OUTPUT_COMP)
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


void Timer_hal_PWM_start(timer_hal_cfg_t *handle)
{
    HAL_TIM_PWM_Start(&tim[handle->periph], handle->channel);
}


void Timer_hal_PWM_DC(timer_hal_cfg_t *handle, uint32_t val)
{
    switch (handle->channel)
    {
    case TIM_CHANNEL_1:
    LL_TIM_OC_SetCompareCH1(tim[handle->periph].Instance, val);
    break;
    case TIM_CHANNEL_2:
    LL_TIM_OC_SetCompareCH2(tim[handle->periph].Instance, val);
    break;
    case TIM_CHANNEL_3:
    LL_TIM_OC_SetCompareCH3(tim[handle->periph].Instance, val);
    break;
    case TIM_CHANNEL_4:
    LL_TIM_OC_SetCompareCH4(tim[handle->periph].Instance, val);
    break;   

    default:
    break;
    }
}
