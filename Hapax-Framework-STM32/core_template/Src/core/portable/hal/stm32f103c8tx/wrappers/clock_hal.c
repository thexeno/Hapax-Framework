#include <wrappers/clock_hal.h>

clk_hal_err_t Clock_hal_init(void)
{
  clk_hal_err_t st = CLK_INIT_PEND;
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
  //RCC_OscInitStruct.HSEPredivValue = 0;
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF; // or none
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL10;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    st = CLK_ERR;
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    st = CLK_ERR;
  }
  HAL_RCC_EnableCSS();
  SystemCoreClockUpdate();
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
    HAL_RCC_GetHCLKFreq();
    break;

    case RCC_CLOCKTYPE_PCLK1:
    HAL_RCC_GetPCLK1Freq();
    break;

    case RCC_CLOCKTYPE_PCLK2:
    HAL_RCC_GetPCLK2Freq();
    break;

    case RCC_CLOCKTYPE_SYSCLK:
    default:
    ret = HAL_RCC_GetSysClockFreq();
  }
  return ret;
}
