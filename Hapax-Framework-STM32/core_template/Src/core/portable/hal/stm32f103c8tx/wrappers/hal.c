#include <stm32f1xx_hal.h>
#include <wrappers/hal.h>

void Core_hal_init(void)
{
	  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	  HAL_Init();

	  __HAL_RCC_AFIO_CLK_ENABLE();
	  __HAL_RCC_PWR_CLK_ENABLE();

	  /* System interrupt init*/

	  /** NOJTAG: JTAG-DP Disabled and SW-DP Enabled
	  */
	  __HAL_AFIO_REMAP_SWJ_NOJTAG();

}
