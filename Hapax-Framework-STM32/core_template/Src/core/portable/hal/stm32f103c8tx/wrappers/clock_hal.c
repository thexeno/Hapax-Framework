#include "clock_hal.h"
#include "interrupt_hal.h"

static void rcc_isr(void)
{
    while (1)
        ;
}

clock_hal_err_t Clock_hal_init(const clk_hal_conf_t *handle)
{
    clock_hal_err_t st = CLK_INIT_PEND;
    uint32_t clk_check = 0;
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the CPU, AHB and APB busses clocks   */
    RCC_OscInitStruct.OscillatorType = handle->source;
    if (handle->source == CLK_HAL_SOURCE_HSI)
    {
        RCC_OscInitStruct.HSIState = RCC_HSI_ON;
        RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
    }
    else
    {
        RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
        RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    }
    //turn on later if needed
    RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
    RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

    if (handle->source == RCC_OSCILLATORTYPE_HSI)
    {
        RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
        RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
#if ((CONF_HSI_OSC < CONF_AHB_FREQ) || (CONF_HSI_OSC < CONF_APB1_FREQ) || (CONF_HSI_OSC < CONF_APB2_FREQ))
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON; // or none
#if (CONF_AHB_FREQ / CLOCK_HAL_HSI_OSC < 3)
        RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
#elif (CONF_AHB_FREQ / CLOCK_HAL_HSI_OSC < 4)
        RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL3;
#elif (CONF_AHB_FREQ / CLOCK_HAL_HSI_OSC < 5)
        RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
#elif (CONF_AHB_FREQ / CLOCK_HAL_HSI_OSC < 6)
        RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL5;
#elif (CONF_AHB_FREQ / CLOCK_HAL_HSI_OSC < 7)
        RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
#elif (CONF_AHB_FREQ / CLOCK_HAL_HSI_OSC < 8)
        RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL7;
#elif (CONF_AHB_FREQ / CLOCK_HAL_HSI_OSC < 9)
        RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
#elif (CONF_AHB_FREQ / CLOCK_HAL_HSI_OSC < 10)
        RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
#else
        RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL10;
#endif

#else
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF; // or none
#endif
    }
    else
    {
        RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
        RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
#if ((CONF_HSE_OSC < CONF_AHB_FREQ) || (CONF_HSE_OSC < CONF_APB1_FREQ) || (CONF_HSE_OSC < CONF_APB2_FREQ))
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON; // or none
#if (CONF_AHB_FREQ / CONF_HSE_OSC < 3)
        RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
#elif (CONF_AHB_FREQ / CONF_HSE_OSC < 4)
        RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL3;
#elif (CONF_AHB_FREQ / CONF_HSE_OSC < 5)
        RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
#elif (CONF_AHB_FREQ / CONF_HSE_OSC < 6)
        RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL5;
#elif (CONF_AHB_FREQ / CONF_HSE_OSC < 7)
        RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
#elif (CONF_AHB_FREQ / CONF_HSE_OSC < 8)
        RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL7;
#elif (CONF_AHB_FREQ / CONF_HSE_OSC < 9)
        RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
#elif (CONF_AHB_FREQ / CONF_HSE_OSC < 10)
        RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
#else
        RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL10;
#endif

#else
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF; // or none
#endif
    }

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        st = CLK_ERR;
    }
    /** Initializes the CPU, AHB and APB busses clocks 
  */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
#if ((CONF_AHB_FREQ / CONF_APB1_FREQ) < 2)
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
#elif (CONF_AHB_FREQ / CONF_APB1_FREQ < 3)
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
#elif (CONF_AHB_FREQ / CONF_APB1_FREQ < 5)
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
#elif (CONF_AHB_FREQ / CONF_APB1_FREQ < 9)
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV8;
#else
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV16;
#endif

#if (CONF_AHB_FREQ / CONF_APB2_FREQ < 2)
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
#elif (CONF_AHB_FREQ / CONF_APB2_FREQ < 3)
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
#elif (CONF_AHB_FREQ / CONF_APB2_FREQ < 5)
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;
#elif (CONF_AHB_FREQ / CONF_APB2_FREQ < 9)
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV8;
#else
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV16;
#endif

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        st = CLK_ERR;
    }
    //set interrupt
    IntHal_vector_register(rcc_isr, RCC_IRQHandler_num);
    HAL_RCC_EnableCSS();
    SystemCoreClockUpdate();
    clk_check = Clock_hal_get_info(RCC_CLOCKTYPE_HCLK);

    if (clk_check <= 24000000)
    {
        SET_BIT(FLASH->ACR, FLASH_ACR_LATENCY_0);
    }
    else if (clk_check <= 48000000)
    {
        SET_BIT(FLASH->ACR, FLASH_ACR_LATENCY_1);
    }
    else
    {
        SET_BIT(FLASH->ACR, FLASH_ACR_LATENCY_2);
    }
    
    return st;
}

void Clock_hal_clk_out(uint32_t src)
{
    //RCC_MCO1SOURCE_SYSCLK def
    HAL_RCC_MCOConfig(RCC_MCO, src, RCC_MCODIV_1);
}

uint32_t Clock_hal_get_info(uint32_t info)
{
    uint32_t ret = -1;
    switch (info)
    {
    case RCC_CLOCKTYPE_HCLK:
        ret = HAL_RCC_GetHCLKFreq();
        break;

    case RCC_CLOCKTYPE_PCLK1:
        ret = HAL_RCC_GetPCLK1Freq();
        break;

    case RCC_CLOCKTYPE_PCLK2:
        ret = HAL_RCC_GetPCLK2Freq();
        break;

    case RCC_CLOCKTYPE_SYSCLK:
    default:
        ret = HAL_RCC_GetSysClockFreq();
    }
    return ret;
}
