#include "spi.h"

#define  SPI_RB_SIZE 32U


static void spi_byte_handler(spi_hal_ch_t channel);

typedef struct spi_dev_buffered
{
	spi_dev_st* spi_dev_pt;
} spi_dev_buffered_t;

static spi_dev_buffered_t spi_dev_internal_pt[NUM_SPI_CHANNELS];

static uint8_t tx_buff[SPI_RB_SIZE];
static uint8_t rx_buff[SPI_RB_SIZE];

static long baudrate = 0;

static struct ring_buff spi_tx_buff;
static struct ring_buff spi_rx_buff;

spi_status_t Spi_init(spi_dev_st* console, const spi_hal_cfg_t* config, uint8_t* txb, base_t tx_sz, uint8_t* rxb, base_t rx_sz)
{
  spi_status_t ret = SPI_OK;
  console->config = config;
  console->status = SPI_INIT_PEND;

  spi_dev_internal_pt[config->channel].spi_dev_pt = console;

  Rb_init(&console->spi_tx_buff, txb, (rb_sz_t)tx_sz);
  Rb_init(&console->spi_rx_buff, rxb, (rb_sz_t)rx_sz);

  SpiHal_ISR_callback_set(spi_byte_handler, console->config->channel);

  if (SpiHal_init(console->config) != SPI_HAL_OK)
  {
    return SPI_INIT_PEND;
  }
  else
  {
    console->status = SPI_OK;
    return SPI_OK;
  }

  return SPI_ERR;
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
  SpiHal_Start(&console->config);
}

void Spi_slave_release(spi_dev_st* console)
{
  SpiHal_Stop(&console->config);
}

uint8_t Spi_transfer(spi_dev_st* console, base_t rx_items)
{
  uint8_t buffer_size = 0;
  console->rx_items = rx_items;
  if (&console->config->mode == SPI_MODE_MASTER)
  {
    if ((Rb_used_size(&console->spi_tx_buff) > 0) || (console->rx_items > 0))
    {
      SpiHal_transmit(&console->config, Rb_retrieve_char(&console->spi_tx_buff));  
    }
  }
}

void spi_byte_handler(spi_hal_ch_t channel)
{
  spi_dev_st* console = spi_dev_internal_pt[channel].spi_dev_pt; // buffered for debug

  if ((Rb_used_size(&console->spi_tx_buff) > 0) && (console->rx_items > 0))
  {
    // MASTER 
    // there is data to send from buffer, keep doing it
    // but also has to be received rx_items
    Rb_put_char(&console->spi_rx_buff, SpiHal_read(&console->config));  // store what was shifted in
    console->rx_items--;
    SpiHal_transmit(&console->config, Rb_retrieve_char(&console->spi_tx_buff)); //keep transmit 
  }
  else if ((Rb_used_size(&console->spi_tx_buff) > 0) && (console->rx_items == 0))
  {
    // no more data to be received, more to be sent
    SpiHal_transmit(&console->config, Rb_retrieve_char(&console->spi_tx_buff)); //keep transmit 
  }
  else if ((Rb_used_size(&console->spi_tx_buff) == 0) && (console->rx_items > 0))
  {
    // just receive
    Rb_put_char(&console->spi_rx_buff, SpiHal_read(&console->config));  // store what was shifted in
    console->rx_items--; // c'è il > 0 che scazza
    if (console->rx_items > 0)
    {
      SpiHal_transmit(&console->config, 0x00); //keep receving with dummy
    }
  }
  else
  {
    // stop
  }
}