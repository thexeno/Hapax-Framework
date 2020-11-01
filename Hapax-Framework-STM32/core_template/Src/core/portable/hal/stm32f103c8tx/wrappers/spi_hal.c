#include "spi_hal.h"
#include "interrupt_hal.h"

static const spi_hal_conf_t *spi_hal_conf_buff;
static SPI_HandleTypeDef spi[SPI_TOTAL_INSTANCE];

void Spi_hal_init(const spi_hal_conf_t *handle)
{
    spi_hal_err_t ret = SPI_HAL_ERR_OK;
    spi_hal_conf_buff = handle;
    
    for (int i = 0; i < SPI_TOTAL_INSTANCE; i++)
    {
        if (handle[i].spi != CONF_SPI_ENUM_UNUSED)
        {
            spi[i].Instance = handle[i].periph; //TIM3;
            spi[i].Init.Mode = 
            spi[i].Init.Direction = SPI_DIRECTION_2LINES; // just use default
            spi[i].Init.DataSize = handle[i].dsize;
            spi[i].Init.CLKPolarity  = handle[i].cpol;
            spi[i].Init.CLKPhase = handle[i].cpha;
            spi[i].Init.NSS 
            spi[i].Init.BaudRatePrescaler
            spi[i].Init.FirstBit
            spi[i].Init.TIMode
            spi[i].Init.CRCCalculation
            spi[i].Init.CRCPolynomial
            if (HAL_SPI_Init(&hspi1) != HAL_OK)
            {
                spi_hal_error_handler(&ret);
            }

hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
}


void Spi_hal_send(conf_spi_e spi_dev, uint32_t val)
{
    
}

void Spi_hal_transfer(conf_pwm_e oc, uint32_t val)
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


