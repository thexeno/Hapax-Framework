#include "spi_hal.h"
#include "interrupt_hal.h"

static const spi_hal_conf_t *spi_hal_conf_buff;
static SPI_HandleTypeDef spi[SPI_TOTAL_INSTANCE];
static int local_i = 0;

enum {
    SPI_HAL_ENUM_SPI1 = 0,
    SPI_HAL_ENUM_SPI2,
    SPI_HAL_ENUM_TOTAL
};

typedef void (*spi_hal_isr_cb)(conf_spi_e);
spi_hal_isr_cb spi_hal_ISR_rxc_cb[SPI_TOTAL_INSTANCE];
spi_hal_isr_cb spi_hal_ISR_txe_cb[SPI_TOTAL_INSTANCE];
spi_hal_isr_cb spi_hal_ISR_err_cb[SPI_TOTAL_INSTANCE];

static void spi_hal_cb_default(conf_spi_e spi)
{
    (void)spi;
    while (1)
        ;
}



void spi_hal_spi2_isr()
{
   if (uart_hal_cfg_buff[SPI_HAL_ENUM_SPI2].Instance->CR2 & SPI_CR2_TXEIE)
   {
        spi_hal_ISR_txe_cb[SPI_HAL_ENUM_SPI2](uart_hal_cfg_buff[SPI_HAL_ENUM_SPI2].uart_enum);
   }
   else if (uart_hal_cfg_buff[SPI_HAL_ENUM_SPI2].Instance->CR2 & SPI_CR2_RXNEIE)
   {
        spi_hal_ISR_rxc_cb[SPI_HAL_ENUM_SPI2](uart_hal_cfg_buff[SPI_HAL_ENUM_SPI2].uart_enum);
   }
   else if (uart_hal_cfg_buff[SPI_HAL_ENUM_SPI2].Instance->CR2 & SPI_CR2_ERRIE)
   {
        spi_hal_ISR_err_cb[SPI_HAL_ENUM_SPI2](uart_hal_cfg_buff[SPI_HAL_ENUM_SPI2].uart_enum);
   }
   else
   {
       /* code */
   }
}

void spi_hal_spi2_isr()
{
   if (uart_hal_cfg_buff[SPI_HAL_ENUM_SPI1].Instance->CR2 & SPI_CR2_TXEIE)
   {
        spi_hal_ISR_txe_cb[SPI_HAL_ENUM_SPI1](uart_hal_cfg_buff[SPI_HAL_ENUM_SPI1].uart_enum);
   }
   else if (uart_hal_cfg_buff[SPI_HAL_ENUM_SPI1].Instance->CR2 & SPI_CR2_RXNEIE)
   {
        spi_hal_ISR_rxc_cb[SPI_HAL_ENUM_SPI1](uart_hal_cfg_buff[SPI_HAL_ENUM_SPI1].uart_enum);
   }
   else if (uart_hal_cfg_buff[SPI_HAL_ENUM_SPI1].Instance->CR2 & SPI_CR2_ERRIE)
   {
        spi_hal_ISR_err_cb[SPI_HAL_ENUM_SPI1](uart_hal_cfg_buff[SPI_HAL_ENUM_SPI1].uart_enum);
   }
   else
   {
       /* code */
   }
}

spi_hal_err_t Spi_hal_ISR_callback_set(conf_spi_e spi, spi_hal_isr_t isr_type, void (*fp_t)(conf_spi_e))
{
    int i = 0;
    spi_hal_err_t ret = SPI_HAL_ERR_OK;
    for (i = 0; i < SPI_TOTAL_INSTANCE; i++)
    {
        if (spi_hal_conf_buff[i].spi == spi)
        {
            if (isr_type == SPI_HAL_ISR_T_ERR)
            {
                spi_hal_ISR_err_cb[spi_hal_cfg_buff[i].base_enum] = fp_t;
            }
            else if (isr_type == SPI_HAL_ISR_T_TXE)
            {
                spi_hal_ISR_txe_cb[spi_hal_cfg_buff[i].base_enum] = fp_t;
            }
            else if (isr_type == SPI_HAL_ISR_T_RX)
            {
                spi_hal_ISR_rxc_cb[spi_hal_cfg_buff[i].base_enum] = fp_t;
            }
            else
            {
                ret = SPI_HAL_ERR_ERROR;
            }
        }
    }
    if (i >= SPI_TOTAL_INSTANCE)
    {
        ret = SPI_HAL_ERR_NO_DEV;
    }
    return ret;
}

spi_hal_err_t Spi_hal_init(const spi_hal_conf_t *handle)
{
    spi_hal_conf_buff = handle;
    int clk_freq = 0;
    uint32_t baud_presc = 0;
    uint32_t nss = 0;
    for (int i = 0; i < SPI_TOTAL_INSTANCE; i++)
    {
        spi_hal_ISR_err_cb[spi_hal_cfg_buff[i].base_enum] = spi_hal_cb_default;
        spi_hal_ISR_txe_cb[spi_hal_cfg_buff[i].base_enum] = spi_hal_cb_default;
        spi_hal_ISR_rxc_cb[spi_hal_cfg_buff[i].base_enum] = spi_hal_cb_default;
        if (handle[i].spi != CONF_SPI_ENUM_UNUSED)
        {
            handle[i].status = SPI_HAL_ERR_LOCK;
            CLEAR_BIT(handle[i].periph->CR1, SPI_CR1_SPE);


// SPI_MODE_MASTER
// just use default
// SPI_DATASIZE_16BIT
// SPI_POLARITY_LOW
//             SPI_PHASE_1EDGE

            if (handle[i].mode == SPI_MODE_MASTER)
            {
                // set as HW output
                nss = SPI_NSS_HARD_OUTPUT;
            }
            else if (handle[i].mode == SPI_MODE_SLAVE)
            {
                // hw input
                nss = SPI_NSS_HARD_INPUT;
            }
            else
            {
                return SPI_HAL_ERR_ERROR;
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
                baud_presc = SPI_BAUDRATEPRESCALER_2;
                break;
            case 4:
                baud_presc = SPI_BAUDRATEPRESCALER_4;
                break;
            case 8:
                baud_presc = SPI_BAUDRATEPRESCALER_8;
                break;
            case 16:
                baud_presc = SPI_BAUDRATEPRESCALER_16;
                break;
            case 32:
                baud_presc = SPI_BAUDRATEPRESCALER_32;
                break;
            case 64:
                baud_presc = SPI_BAUDRATEPRESCALER_64;
                break;
            case 128:
                baud_presc = SPI_BAUDRATEPRESCALER_128;
                break;
            case 256:
                baud_presc = SPI_BAUDRATEPRESCALER_256;
                break;
            default:
                // the slowest
                baud_presc = SPI_BAUDRATEPRESCALER_256;
                break;
            }
            WRITE_REG(handle[i].periph->CR1, (handle[i].mode | SPI_DIRECTION_2LINES | SPI_DATASIZE_8BIT |
                                              handle[i].cpol | handle[i].cpha | (nss & SPI_CR1_SSM) |
                                              baud_presc | SPI_FIRSTBIT_MSB | SPI_CRCCALCULATION_DISABLE));
            SET_BIT(handle[i].periph->CR2, (SPI_IT_TXE | SPI_IT_RXNE | SPI_IT_ERR));
            SET_BIT(handle[i].periph->CR1, SPI_CR1_SPE);
        }
        handle[i].status = SPI_HAL_ERR_OK;
    }
}

spi_hal_err_t Spi_hal_init_transfer(conf_spi_e spi, uint32_t txd, spi_hal_handle_t* handle)
{
    spi_hal_handle_t i = 0;
    // search the OC conf
    while (spi_hal_conf_buff[i].spi != spi)
    {
        if (i > SPI_TOTAL_INSTANCE)
        {
            return SPI_HAL_ERR;
        }
        i++;
    }
    if (spi_hal_conf_buff[i].status == SPI_HAL_ERR_LOCK)
    {
        return SPI_HAL_ERR_LOCK; 
    }
    else
    {
        spi_hal_conf_buff[i].status = SPI_HAL_ERR_LOCK;
    }
    
    *handle = i;
     *((volatile uint8_t *)&spi_hal_conf_buff[i].periph->DR = (uint8_t txd);
    return SPI_HAL_ERR_OK;
}


spi_hal_err_t Spi_hal_transfer(spi_hal_handle_t handle, uint32_t txd, uint32_t* rxd)
{
    if (handle > SPI_HAL_ENUM_TOTAL)
    {
        return SPI_HAL_ERR;
    }
    else if (spi_hal_conf_buff[i].status == SPI_HAL_ERR_LOCK)
    {
        return SPI_HAL_ERR_LOCK;
    }
    else
    {
        *((volatile uint8_t *)&spi_hal_conf_buff[handle].periph->DR = (uint8_t txd);
    }
    return SPI_HAL_ERR_OK;
}