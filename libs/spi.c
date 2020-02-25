#include "spi.h"
#include "interrupt_hal.h"




typedef struct spi_dev_buffered
{
	spi_dev_st* spi_dev_pt;
	uint8_t* buffer;
	base_t max_items;
} spi_dev_buffered_t;


//static void spi_fast_rx_byte_handler(spi_hal_ch_t channel);
//static void spi_fast_rx_byte_handler_0(void);
//static void spi_byte_slave_handler(spi_hal_ch_t channel);


uint8_t spi_ack;
uint8_t spi_nack;

static spi_dev_buffered_t spi_dev_internal_pt[NUM_SPI_CHANNELS];
static void spi_rx_byte_handler(spi_hal_ch_t channel);
static void spi_tx_byte_handler(spi_hal_ch_t channel);
static void spi_rx_byte_handler0(void);

spi_status_t Spi_init(spi_dev_st* console, const spi_hal_cfg_t* config)
{
	spi_status_t ret = SPI_OK;
	console->config = config;
	console->status = SPI_INIT_PEND;

	spi_dev_internal_pt[config->channel].spi_dev_pt = console;

	// Set teh ISR callback in the given TX or RX functions

	if (SpiHal_init(console->config) != SPI_HAL_OK)
	{
		return SPI_INIT_PEND;
	}
	else
	{
		console->status = SPI_OK;
		return console->status;
	}

	return SPI_ERR;
}



uint8_t Spi_get_char(spi_dev_st* console)
{
  return ((uint8_t)Rb_retrieve_char(&console->spi_rx_buff));
}


/**
 * @brief      Add a whole string  to the TX ring buffer.
 *
 * @param[in]  str   The string pointer
 */
void Spi_put_buff(spi_dev_st* console, const char* str)
{
	Rb_put_buff(str, &console->spi_tx_buff);
}

void Spi_put_char(spi_dev_st* console, char ch)
{
  Rb_put_char(&console->spi_tx_buff, ch);
}

/**
 * @brief      Add a 16bit integer to the TX ring buffer
 *
 * @param[in]  c     integer
 */
void Spi_put_integer(spi_dev_st* console, uint16_t c)
{
  Rb_put_integer("%d", c, &console->spi_tx_buff);
}

void Spi_slave_select(spi_dev_st* console)
{
  SpiHal_Start(console->config);
}

void Spi_slave_release(spi_dev_st* console)
{
	
  SpiHal_Stop(console->config);
}


/**
 * @brief      The elements in the RX ring buffer
 *
 * @return     Number of elements
 */
base_t Spi_transaction_items(spi_dev_st* console)
{
    return spi_dev_internal_pt[console->config->channel].max_items;
}

/**
 * @brief      The elements in the TX ring buffer
 *
 * @return     Number of elements
 */
uint8_t Spi_tx_buffer_elem(spi_dev_st* console)
{
    uint16_t elem = 0;
    elem = (uint16_t)Rb_used_size(&console->spi_tx_buff);
    return elem;
}

#warning "fai blocking tx"

spi_status_t Spi_rx_blocking(spi_dev_st* console, uint8_t* buffer, base_t items)
{
	uint8_t buffer_size = 0;
	unsigned long time_delta = 60000;
	base_t int_status;
	spi_status_t ret = SPI_BUSY;
	SpiHal_ISR_callback_reset(console->config->channel);
	SpiHal_interrupt_disable(console->config);
	int_status = IntHal_suspend_global_interrupt();
	
	if (console->config->mode == SPI_MODE_MASTER)
	{
		if (console->status == SPI_OK)
		{
			console->status = SPI_BUSY;
			//IntHal_restore_global_interrupt(int_status);
			spi_dev_internal_pt[console->config->channel].spi_dev_pt = console;
			spi_dev_internal_pt[console->config->channel].max_items = items;
			console->items = items;
			SpiHal_Start(console->config);
			while (console->items > 0)
			{
				SpiHal_transmit(console->config, 0x00); // shift in
				while (!SpiHal_transfer_done(console->config));
				SpiHal_Clear(console->config);
				buffer[spi_dev_internal_pt[console->config->channel].max_items - console->items--] = SpiHal_read(console->config);
			}
			console->status = SPI_RX_BURST_DONE;
			SpiHal_Stop(console->config); // includes the IFlag clear
			//SpiHal_interrupt_enable(console->config);
			ret = SPI_OK;
		}
		else
		{
			ret = console->status;
		}
	}
	else
	{
		if (console->status == SPI_OK)
		{
			console->status = SPI_BUSY;
			IntHal_restore_global_interrupt(int_status);
			spi_dev_internal_pt[console->config->channel].spi_dev_pt = console;
			spi_dev_internal_pt[console->config->channel].max_items = items;
			console->items = items;
			while (console->items > 0 && time_delta > 0)
			{
				while (!SpiHal_transfer_done(console->config) && time_delta > 0)
				{
					time_delta--;
				}

				SpiHal_Clear(console->config);
				buffer[spi_dev_internal_pt[console->config->channel].max_items - console->items--] = SpiHal_read(console->config);
				time_delta--;
			}
			SpiHal_Stop(console->config); // includes the IFlag clear
			console->status = SPI_RX_BURST_DONE;
			//SpiHal_interrupt_enable(console->config);
			ret = SPI_OK;
		}
		else
		{
			ret = console->status;
		}
	}
	//IntHal_restore_global_interrupt(int_status);
	return ret;
}

spi_status_t Spi_rx(spi_dev_st* console, uint8_t* buffer, base_t items)
{
	uint8_t buffer_size = 0;
	base_t int_status = IntHal_suspend_global_interrupt();
	spi_status_t ret = SPI_BUSY;
	spi_dev_internal_pt[console->config->channel].spi_dev_pt = console;
	spi_dev_internal_pt[console->config->channel].buffer = buffer;
	spi_dev_internal_pt[console->config->channel].max_items = items;
	  
	SpiHal_ISR_callback_set(spi_rx_byte_handler, console->config->channel); // change default isr
	SpiHal_interrupt_enable(console->config);
	if (console->config->mode == SPI_MODE_MASTER)
	{	
		if (console->status == SPI_OK)
		{
			console->status = SPI_BUSY;
			console->items = items;
			SpiHal_Start(console->config);
			if (console->items > 0)
			{
				SpiHal_transmit(console->config, 0x00); // shift in the first byte 
			}
			else
			{
				SpiHal_Stop(console->config);
			}
			ret = SPI_OK;
		}
		else
		{
			ret = SPI_BUSY;
		}
	}
	else
	{
		if (console->status == SPI_OK)
		{
			console->items = items;
		}
		ret = console->status;
	}
	IntHal_restore_global_interrupt(int_status);
	return ret;
}

spi_status_t Spi_tx_blocking(spi_dev_st* console, uint8_t* buffer, base_t items)
{
	uint8_t buffer_size = 0;
	unsigned long time_delta = 60000;
	base_t int_status;
	spi_status_t ret = SPI_BUSY;
	SpiHal_ISR_callback_reset(console->config->channel);
	SpiHal_interrupt_disable(console->config);
	int_status = IntHal_suspend_global_interrupt();
	
	if (console->config->mode == SPI_MODE_MASTER)
	{
		if (console->status == SPI_OK)
		{
			console->status = SPI_BUSY;
			//IntHal_restore_global_interrupt(int_status);
			spi_dev_internal_pt[console->config->channel].spi_dev_pt = console;
			spi_dev_internal_pt[console->config->channel].max_items = items;
			console->items = items;
			SpiHal_Start(console->config);
			while (console->items > 0)
			{
				SpiHal_transmit(console->config, (uint8_t)buffer[spi_dev_internal_pt[console->config->channel].max_items - console->items--]); // shift in
				while (!SpiHal_transfer_done(console->config));
				SpiHal_Clear(console->config);
			}
			console->status = SPI_TX_BURST_DONE;
			SpiHal_Stop(console->config); // includes the IFlag clear
			//SpiHal_interrupt_enable(console->config);
			ret = SPI_OK;
		}
		else
		{
			ret = console->status;
		}
	}
	else
	{
		if (console->status == SPI_OK)
		{
			console->status = SPI_BUSY;
			IntHal_restore_global_interrupt(int_status);
			spi_dev_internal_pt[console->config->channel].spi_dev_pt = console;
			spi_dev_internal_pt[console->config->channel].max_items = items;
			console->items = items;
			while (console->items > 0 && time_delta > 0)
			{
				while (!SpiHal_transfer_done(console->config) && time_delta > 0)
				{
					time_delta--;
				}

				SpiHal_Clear(console->config);
				SpiHal_transmit(console->config, buffer[spi_dev_internal_pt[console->config->channel].max_items - console->items--]);
				time_delta--;
			}
			SpiHal_Stop(console->config); // includes the IFlag clear
			console->status = SPI_TX_BURST_DONE;
			//SpiHal_interrupt_enable(console->config);
			ret = SPI_OK;
		}
		else
		{
			ret = console->status;
		}
	}
	//IntHal_restore_global_interrupt(int_status);
	return ret;
}

spi_status_t Spi_tx(spi_dev_st* console, uint8_t* buffer, base_t items)
{
	uint8_t buffer_size = 0;
	base_t int_status = IntHal_suspend_global_interrupt();
	spi_status_t ret = SPI_BUSY;
	spi_dev_internal_pt[console->config->channel].spi_dev_pt = console;
	spi_dev_internal_pt[console->config->channel].buffer = buffer;
	spi_dev_internal_pt[console->config->channel].max_items = items;
	
	SpiHal_ISR_callback_set(spi_tx_byte_handler, console->config->channel); // change default isr
	SpiHal_interrupt_enable(console->config);
	if (console->config->mode == SPI_MODE_MASTER)
	{
		if (console->status == SPI_OK)
		{
			console->status = SPI_BUSY;
			console->items = items;
			SpiHal_Start(console->config);
			if (console->items > 0)
			{
				SpiHal_transmit(console->config, spi_dev_internal_pt[console->config->channel].buffer[0]); // shift out the first byte
			}
			else
			{
				SpiHal_Stop(console->config);
			}
			ret = SPI_OK;
		}
		else
		{
			ret = SPI_BUSY;
		}
	}
	else
	{
		if (console->status == SPI_OK)
		{
			console->items = items;
		}
		ret = console->status;
	}
	IntHal_restore_global_interrupt(int_status);
	return ret;
}


spi_status_t Spi_get_status(spi_dev_st* console)
{
	spi_status_t ret = SPI_ERR;
	ret = console->status;
	if (console->status == SPI_RX_BURST_DONE || console->status == SPI_TX_BURST_DONE)
	{
		ret = console->status;
		console->status = SPI_OK;
	}
	return ret;
}


static void spi_rx_byte_handler(spi_hal_ch_t channel)
{
	if (spi_dev_internal_pt[channel].spi_dev_pt->items > 0)
	{
		(spi_dev_internal_pt[channel].buffer[spi_dev_internal_pt[channel].max_items - spi_dev_internal_pt[channel].spi_dev_pt->items--]) = *spdr[spi_dev_internal_pt[channel].spi_dev_pt->config->channel];
	}
	else
	{
		spi_dev_internal_pt[channel].spi_dev_pt->status = SPI_RX_BURST_DONE;
	}
}

static void spi_rx_byte_handler0(void)
{
	if (spi_dev_internal_pt[0].spi_dev_pt->items > 0)
	{
		(spi_dev_internal_pt[0].buffer[spi_dev_internal_pt[0].max_items - spi_dev_internal_pt[0].spi_dev_pt->items--]) = *spdr[0];
	}
	else
	{
		spi_dev_internal_pt[0].spi_dev_pt->status = SPI_RX_BURST_DONE;
	}
}

static void spi_tx_byte_handler(spi_hal_ch_t channel)
{
	if (spi_dev_internal_pt[channel].spi_dev_pt->items > 0)
	{
		*spdr[spi_dev_internal_pt[channel].spi_dev_pt->config->channel] = (spi_dev_internal_pt[channel].buffer[spi_dev_internal_pt[channel].max_items - spi_dev_internal_pt[channel].spi_dev_pt->items--]);
	}
	else
	{
		spi_dev_internal_pt[channel].spi_dev_pt->status = SPI_TX_BURST_DONE;
	}
}
