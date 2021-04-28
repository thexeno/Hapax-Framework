#include "conf.h"
#include "spi_hal.h"



void i2c_init(i2c_dev_st* dev)
{
	/** Or speed, or address, depending if master or slave, respectively */
	if (dev->mode == I2C_MASTER_INIT_MODE)
	{
		i2cData.baud = (uint16_t)speed_adx;
		TWCR = 0;
		i2c_baudSelector(i2cData.baud);
		TWCR |= (1<<TWIE); /* Int enabled */
		TWCR |= (1<<TWEN); /* enabled */
		/*By writing the TWEA bit to zero, the device can be virtually disconnected from the 2-wire Serial
		Bus temporarily. Address recognition can then be resumed by writing the TWEA bit to one
		again.
		 */	
		i2cData.mode = I2C_MASTER_INIT_MODE;
	}
	else if (mode == I2C_SLAVE_INIT_MODE)
	{
		TWCR = 0;
		TWCR |= (1<<TWIE); /* Int enabled */
		TWCR |= (1<<TWEA); /* ack if selected */
		TWAR =	(uint8_t)speed_adx;
		TWCR |= (1<<TWEN); /* enabled */
		i2cData.mode = I2C_SLAVE_MODE;
		i2cData.index = 0;
		i2cData.buffLenRx = I2C_BUFF_LEN;
		i2cData.buffLenTx = I2C_BUFF_LEN;
	}
	else
	{
		
	}

	i2cData.busy = 0;
}

uint8_t I2CHal_init_master(spi_hal_conf_bord_e data_order, spi_hal_conf_cpol_e clk_pol, spi_hal_conf_cpha_e clk_phase, long speed)
{
	uint8_t conf_var = 0x00;
	uint8_t clk_div = 0;

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
    clk_div = (uint8_t)(CONF_CPU_FREQ/(speed));
	if (clk_div == 0)
			return -1;

	if (clk_div <= 2)
	{
		SPSR |= (1 << SPI2X);
		conf_var &= ~((1 << SPR1) | (1 << SPR0));
	}
    else if (clk_div <= 4)
	{
		SPSR &= ~(1 << SPI2X);
		conf_var &= ~((1 << SPR1) | (1 << SPR0));
	}
    else if (clk_div <= 8)
    {
		SPSR |= (1 << SPI2X);
		conf_var &= ~(1 << SPR1);
		conf_var |= (1 << SPR0);
    }	
    else if (clk_div <= 16)
    {
		SPSR &= ~(1 << SPI2X);
		conf_var &= ~(1 << SPR1);
		conf_var |= (1 << SPR0);	    
    }
	else if (clk_div <= 32)
	{
		SPSR |= (1 << SPI2X);
		conf_var |= (SPR1);
		conf_var &= ~(SPR0);
	}
    else if (clk_div <= 64)
    {
		SPSR &= ~(1 << SPI2X);
		conf_var |= (SPR1);
		conf_var &= ~(SPR0);
    }
	else
	{
		SPSR &= ~(1 << SPI2X);
		conf_var |= (SPR1);
		conf_var |= ~(SPR0);
	}
	
	SPCR = conf_var;
	return 0;
}


uint8_t SpiHal_init_slave(spi_hal_conf_bord_e data_order, spi_hal_conf_cpol_e clk_pol, spi_hal_conf_cpha_e clk_phase)
{
	uint8_t conf_var = 0x00;
	uint8_t clk_div = 0;

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
