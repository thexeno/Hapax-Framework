#include "spi_hal.h"
#include "pin_hal.h"
#include "interrupt_hal.h"

typedef void (*spi_hal_isr_cb)(spi_hal_ch_t );
static void spi0_isr(void);
spi_hal_isr_cb spiHal_ISR_cb[NUM_SPI_CHANNELS];

register_t volatile * const spcr[NUM_SPI_CHANNELS] =
{
	(register_t*)&SPCR
};

register_t volatile * const spsr[NUM_SPI_CHANNELS] =
{
	(register_t*)&SPSR
};

register_t volatile * const spdr[NUM_SPI_CHANNELS] =
{
	(register_t*)&SPDR
};

spi_hal_err_t SpiHal_init(const spi_hal_cfg_t* handle)
{
	base_t conf_var = 0x00;
	base_t clk_div = 0;

    *spcr[handle->channel] = ((1<< SPIE) | (1 << SPE));

    switch (handle->endian)
    {
		case SPI_HAL_MSB_FIRST:
		*spcr[handle->channel] &= ~(1 << DORD);
		break;

		case SPI_HAL_LSB_FIRST:
		*spcr[handle->channel] |= (1 << DORD);
		default:
		break;
    }

	switch (handle->phase)
    {
      case SPI_HAL_SCK_LEAD_EDGE_SAMP:
      *spcr[handle->channel] &= ~(1 << CPHA);
      break;

      case SPI_HAL_SCK_TRAIL_EDGE_SAMP:
	  *spcr[handle->channel] |= (1 << CPHA);
      default:
      break;
    }

    switch (handle->polarity)
    {
      case SPI_HAL_SCK_LEAD_RISE:
	  *spcr[handle->channel] &= ~(1 << CPOL);
      break;

      case SPI_HAL_SCK_LEAD_FALL:
	  *spcr[handle->channel] |= (1 << CPOL);
      default:
      break;
    }

    if (handle->mode == SPI_MODE_MASTER)
    {
    	if (Gpio_hal_set_mode(handle->pin_ss, GPIO_MODE_OUTPUT) != GPIO_OK)
    	{
    		return SPI_HAL_ERR;
    	}
    	else
    	{
    		Gpio_hal_set_value(handle->pin_ss, GPIO_HIGH);

    		*spcr[handle->channel] |= (1 << MSTR);

		    clk_div = (uint8_t)(handle->sys_clock/(handle->baudrate));

			if (clk_div == 0)
					return -1;

			if (clk_div <= 3)
			{
				*spsr[handle->channel] |= (1 << SPI2X);
				*spcr[handle->channel] &= ~((1 << SPR1) | (1 << SPR0));
			}
		    else if (clk_div <= 4)
			{
				*spsr[handle->channel] &= ~(1 << SPI2X);
				*spcr[handle->channel] &= ~((1 << SPR1) | (1 << SPR0));
			}
		    else if (clk_div <= 8)
		    {
				*spsr[handle->channel] |= (1 << SPI2X);
				*spcr[handle->channel] &= ~(1 << SPR1);
				*spcr[handle->channel] |= (1 << SPR0);
		    }	
		    else if (clk_div <= 16)
		    {
				*spsr[handle->channel] &= ~(1 << SPI2X);
				*spcr[handle->channel] &= ~(1 << SPR1);
				*spcr[handle->channel] |= (1 << SPR0);	    
		    }
			else if (clk_div <= 32)
			{
				*spsr[handle->channel] |= (1 << SPI2X);
				*spcr[handle->channel] |= (SPR1);
				*spcr[handle->channel] &= ~(SPR0);
			}
		    else if (clk_div <= 64)
		    {
				*spsr[handle->channel] &= ~(1 << SPI2X);
				*spcr[handle->channel] |= (SPR1);
				*spcr[handle->channel] &= ~(SPR0);
		    }
			else
			{
				*spsr[handle->channel] &= ~(1 << SPI2X);
				*spcr[handle->channel] |= (SPR1);
				*spcr[handle->channel] |= ~(SPR0);
			}
    	}
	}

	IntHal_vector_register(spi0_isr, SPI_STC_vect_num);
	return 0;
}

static void spi0_isr()
{
	spiHal_ISR_cb[SPI_0](SPI_0); // callback a i.e. ser_rx_char_ISR(ser_dev_st* console)
}


void SpiHal_ISR_callback_set(void (*fp_t)(spi_hal_ch_t ), spi_hal_ch_t channel)
{
	spiHal_ISR_cb[channel] = fp_t; 
}

void SpiHal_transmit(const spi_hal_cfg_t* handle, uint8_t TXData)
{
   // nello slave va messo in isr
   *spdr[handle->channel] = TXData;
}

uint8_t SpiHal_read(const spi_hal_cfg_t* handle)
{
    return *spdr[handle->channel];
}

void SpiHal_Start(const spi_hal_cfg_t* handle)
{
	Gpio_hal_set_value(handle->pin_ss, GPIO_LOW);
}

void SpiHal_Stop(const spi_hal_cfg_t* handle)
{
	Gpio_hal_set_value(handle->pin_ss, GPIO_HIGH);
}

