#include "spi_hal.h"
#include "interrupt_hal.h"



uint8_t SpiHal_init_master(spi_hal_conf_bord_e data_order, spi_hal_conf_cpol_e clk_pol, spi_hal_conf_cpha_e clk_phase, long speed)
{
	uint8_t conf_var = 0x00;
	uint8_t clk_div = 0;

    SPCR = ((1<< SPIE) | (1 << SPE) | (1 << MSTR));
    switch (data_order)
    {
		case SPI_HAL_MSB_FIRST:
		SPCR &= ~(1 << DORD);
		break;

		case SPI_HAL_LSB_FIRST:
		SPCR |= (1 << DORD);
		default:
		break;
    }
	switch (clk_phase)
    {
      case SPI_HAL_SCK_LEAD_EDGE_SAMP:
      SPCR &= ~(1 << CPHA);
      break;

      case SPI_HAL_SCK_TRAIL_EDGE_SAMP:
	  SPCR |= (1 << CPHA);
      default:
      break;
    }
    switch (clk_pol)
    {
      case SPI_HAL_SCK_LEAD_RISE:
	  SPCR &= ~(1 << CPOL);
      break;

      case SPI_HAL_SCK_LEAD_FALL:
	  SPCR |= (1 << CPOL);
      default:
      break;
    }
	#warning "put cpu speed in a truct for the config"
    clk_div = (uint8_t)(16000000UL/(speed));
	if (clk_div == 0)
			return -1;

	if (clk_div <= 3)
	{
		SPSR |= (1 << SPI2X);
		SPCR &= ~((1 << SPR1) | (1 << SPR0));
	}
    else if (clk_div <= 4)
	{
		SPSR &= ~(1 << SPI2X);
		SPCR &= ~((1 << SPR1) | (1 << SPR0));
	}
    else if (clk_div <= 8)
    {
		SPSR |= (1 << SPI2X);
		SPCR &= ~(1 << SPR1);
		SPCR |= (1 << SPR0);
    }	
    else if (clk_div <= 16)
    {
		SPSR &= ~(1 << SPI2X);
		SPCR &= ~(1 << SPR1);
		SPCR |= (1 << SPR0);	    
    }
	else if (clk_div <= 32)
	{
		SPSR |= (1 << SPI2X);
		SPCR |= (SPR1);
		SPCR &= ~(SPR0);
	}
    else if (clk_div <= 64)
    {
		SPSR &= ~(1 << SPI2X);
		SPCR |= (SPR1);
		SPCR &= ~(SPR0);
    }
	else
	{
		SPSR &= ~(1 << SPI2X);
		SPCR |= (SPR1);
		SPCR |= ~(SPR0);
	}
	
	//SPCR = conf_var;
	return 0;
}


uint8_t SpiHal_init_slave(spi_hal_conf_bord_e data_order, spi_hal_conf_cpol_e clk_pol, spi_hal_conf_cpha_e clk_phase)
{
	uint8_t conf_var = 0x00;


	conf_var = ((1<< SPIE) | (1 << SPE)); 
	switch (data_order)
	{
		case SPI_HAL_MSB_FIRST:
		conf_var &= ~(1 << DORD);
		break;

		case SPI_HAL_LSB_FIRST:
		conf_var |= (1 << DORD);
		default:
		break;
	}
	switch (clk_phase)
	{
		case SPI_HAL_SCK_LEAD_EDGE_SAMP:
		conf_var &= ~(1 << CPHA);
		break;

		case SPI_HAL_SCK_TRAIL_EDGE_SAMP:
		conf_var |= (1 << CPHA);
		default:
		break;
	}
	switch (clk_pol)
	{
		case SPI_HAL_SCK_LEAD_RISE:
		conf_var &= ~(1 << CPOL);
		break;

		case SPI_HAL_SCK_LEAD_FALL:
		conf_var |= (1 << CPOL);
		default:
		break;
	}
	
	SPCR = conf_var;
	return 0;
}


void SpiHal_transmit(uint8_t TXData)
{
   SPDR = TXData;
}

uint8_t SpiHal_read(void)
{
    return SPDR;
}
//
//uint8_t spiHal_getHwStat(uint8_t st)
//{
    //uint8_t ret = 0;
    //if (st == SPI_TXFLAG_CHECK)
    //{
      ////ret = EUSCI_A_SPI_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_SPI_TRANSMIT_INTERRUPT);
    //}
    //return ret;
//}
