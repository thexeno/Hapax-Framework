#include "spi_hal.h"
#include "interrupt_hal.h"

static const spi_hal_conf_t *spi_hal_conf_buff;
static SPI_HandleTypeDef spi[SPI_TOTAL_INSTANCE];

typedef void (*spi_hal_isr_cb)(conf_spi_e );


static void uart_hal_cb_default(conf_spi_e uart)
{
    (void)uart;
    while(1);
}


spi_hal_isr_cb spi_hal_ISR_cb_uart[SPI_TOTAL_INSTANCE];


 void spi_hal_spi2_isr()
{
  
        uart_hal_ISR_ERR_cb_uart[UART_HAL_UART0](uart_hal_cfg_buff[UART_HAL_UART0].uart_enum); 
 
    MAP_UARTIntClear(base_enum_to_base[UART_HAL_UART0], int_type);
}
  

 void spi_hal_spi1_isr()
{
  
        uart_hal_ISR_ERR_cb_uart[UART_HAL_UART0](uart_hal_cfg_buff[UART_HAL_UART0].uart_enum); 
 
    MAP_UARTIntClear(base_enum_to_base[UART_HAL_UART0], int_type);
}

void Spi_hal_ISR_TX_callback_set(conf_spi_e spi, void (*fp_t)(conf_spi_e ))
{
    int i = 0;
    for (i = 0; i < SPI_TOTAL_INSTANCE; i++)
    {
        if (spi_hal_conf_buff[i].spi == spi)
        {
            spi_hal_ISR_cb_uart[spi_hal_cfg_buff[i].base_enum] = fp_t;
        }
    }
}

void Spi_hal_init(const spi_hal_conf_t *handle)
{
    spi_hal_err_t ret = SPI_HAL_ERR_OK;
    spi_hal_conf_buff = handle;
    int clk_freq = 0;
    uint32_t baud_presc = 0;

    for (int i = 0; i < SPI_TOTAL_INSTANCE; i++)
    {
        if (handle[i].spi != CONF_SPI_ENUM_UNUSED)
        {
            spi[i].Instance = handle[i].periph;
            spi2
                spi[i]
                    .Init.Mode = handle[i].mode;
            SPI_MODE_MASTER
            spi[i].Init.Direction = SPI_DIRECTION_2LINES; // just use default
            spi[i].Init.DataSize = handle[i].dsize;
            SPI_DATASIZE_16BIT
            spi[i].Init.CLKPolarity = handle[i].cpol;
            SPI_POLARITY_LOW
            spi[i].Init.CLKPhase = handle[i].cpha;
            SPI_PHASE_1EDGE
            if (handle[i].mode == SPI_MODE_MASTER)
            {
                // set as HW output
                spi[i].Init.NSS = SPI_NSS_HARD_OUTPUT;
            }
            else
            {
                // hw input
                spi[i].Init.NSS = SPI_NSS_HARD_INPUT;
            }
            if (spi[i].Instance == SPI1)
            {
                clk_freq = Clock_hal_get_info(RCC_CLOCKTYPE_PCLK2);
            }
            else
            {
                // SPI2
                clk_freq = Clock_hal_get_info(RCC_CLOCKTYPE_PCLK1);
            }
            baud_presc = clk_freq / handle[i].baud;
            // this is a  Bit Twiddling Hacks, will Round up to the next highest power of 2
            baud_presc--;
            baud_presc |= baud_presc >> 1;
            baud_presc |= baud_presc >> 2;
            baud_presc |= baud_presc >> 4;
            baud_presc |= baud_presc >> 8;
            baud_presc |= baud_presc >> 16; // for a 32bit number
            baud_presc++;

            switch (baud_presc)
            {
            case 2:
                spi[i].Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
                break;
            case 4:
                spi[i].Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
                break;
            case 8:
                spi[i].Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
                break;
            case 16:
                spi[i].Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
                break;
            case 32:
                spi[i].Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
                break;
            case 64:
                spi[i].Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
                break;
            case 128:
                spi[i].Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
                break;
            case 256:
                spi[i].Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
                break;
            default:
                // the slowest
                spi[i].Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
                break;
            }
            spi[i].Init.FirstBit = SPI_FIRSTBIT_MSB;
            spi[i].Init.TIMode = SPI_TIMODE_DISABLE; // not supported in this device
            spi[i].Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
            spi[i].Init.CRCPolynomial = 10 if (HAL_SPI_Init(&hspi1) != HAL_OK)
            {
                spi_hal_error_handler(&ret);
            }

            if (HAL_SPI_Init(&hspi1) != HAL_OK)
            {
                Error_Handler();
            }
        }

        void Spi_hal_send(conf_spi_e spi_dev, uint32_t val)
        {
        }

        void Spi_hal_transfer(conf_spi_e spi, uint32_t val)
        {
            int i = 0;
            // search the OC conf
            while (spi_hal_conf_buff[i].spi != spi)
            {
                if (i > SPI_TOTAL_INSTANCE)
                {
                    return;
                }
                i++;
            }
        HAL_StatusTypeDef HAL_SPI_TransmitReceive_IT(spi_hal_conf_buff[i].periph, 
        uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)

        }
