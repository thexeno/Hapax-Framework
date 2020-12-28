#include "stm32f1xx_hal.h"
#include "hal.h"

void Core_hal_init(void)
{
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    SET_BIT(FLASH->ACR, FLASH_ACR_PRFTBE); 
    SET_BIT(FLASH->ACR, FLASH_ACR_LATENCY_0);// at the moment clock is default to int 8MHz
    NVIC_SetPriorityGrouping(4);
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN);
    SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);

    // NOJTAG: JTAG-DP Disabled and SW-DP Enabled
    SET_BIT(AFIO->MAPR, AFIO_MAPR_SWJ_CFG_JTAGDISABLE);
}
