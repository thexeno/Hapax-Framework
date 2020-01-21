#include "spi.h"
#include "utils/ring_buffer.h"
#include "hal/atmega328/hal.h" // TI stuff included in hal.h
#include "hal/atmega328/spi_hal.h"
#include "hal/atmega328/interrupt_hal.h"

#define  SPI_RB_SIZE 32U

static uint8_t tx_buff[SPI_RB_SIZE];
static uint8_t rx_buff[SPI_RB_SIZE];

static struct spi_parameters spi;
static long baudrate = 0;

static struct ring_buff spi_tx_buff;
static struct ring_buff spi_rx_buff;

void Spi_init(spi_mode_e mode, long speed)
{
    Rb_init(&spi_rx_buff, rx_buff);
    Rb_init(&spi_tx_buff, tx_buff);

	spi.state = SPI_FREE;
    spi.active_mode = mode;
    IntHal_vector_register(spi_ISR, HAL_VECT_SPI_ID);
    if (mode == SPI_MODE_MASTER)
    {
		Pin_mode(SPI_SS, PIN_OUTPUT, PULLUP_DISABLED);
		Pin_wr_pin(SPI_SS, (!0));
        SpiHal_init_master(
                           SPI_HAL_MSB_FIRST, 
                           SPI_HAL_SCK_LEAD_RISE, 
                           SPI_HAL_SCK_LEAD_EDGE_SAMP, 
                           speed
                           );
    }
    else if (mode == SPI_MODE_SLAVE)
    {
		Pin_mode(SPI_SS, PIN_INPUT, PULLUP_ENABLED);
        SpiHal_init_slave(
                           SPI_HAL_MSB_FIRST, 
                           SPI_HAL_SCK_LEAD_RISE, 
                           SPI_HAL_SCK_LEAD_EDGE_SAMP
                           );
    }
}


/**
 * @brief      Add a whole string  to the TX ring buffer.
 *
 * @param[in]  str   The string pointer
 */
void Spi_put_buff(const char* str)
{
	Rb_put_buff(str, &spi_tx_buff, (rb_sz_t)SPI_RB_SIZE);
}

/**
 * @brief      Add a 16bit integer to the TX ring buffer
 *
 * @param[in]  c     integer
 */
void Spi_put_integer(uint16_t c)
{
	Rb_put_integer("%d", c, &spi_tx_buff, (rb_sz_t)SPI_RB_SIZE);
}


uint8_t Spi_transfer(spi_mode_e mode)
{
    uint8_t dummy = 0x00;

/**
 * Warn if SPI is not free, otherwise initialize accordingly to the mode
 */

    if (spi.state != SPI_FREE)
    {
        return -1;
    }
    else if (spi.active_mode != mode)
    {
        return -2;
    }
    else if (spi.active_mode == SPI_MODE_MASTER)
    {
        Pin_wr_pin(SPI_SS, (0));
        spi.state = SPI_MASTER_BUSY;
    }
    else
    {
        spi.state = SPI_SLAVE_BUSY;
    }

    if (!Rb_is_empty(&spi_tx_buff))
    {
        SpiHal_transmit((uint8_t)Rb_retrieve_char(&spi_tx_buff, (rb_sz_t)SPI_RB_SIZE));    
    }
    else
    {
        SpiHal_transmit(dummy);
    }
    

    // switch (mode)
    // {
    //     case SPI_SEND:
    //     SpiHal_transmit(Rb_retrieve_byte(&spi_tx_buff));
    //     break;

    //     case SPI_RECEIVE:
    //     SpiHal_transmit(dummy);
    //     break;

    //     case SPI_SEND_RECEIVE:
    //     SpiHal_transmit()
    // }
    // SpiHal_transmit(Rb_retrieve_byte(&spi_tx_buff));
    // // wait for isrrx
}



void spi_ISR(void)
{
  uint8_t c = 0;
  /* Corect byte intepretation done at higher level software (SPI device specific) */
  /* Get data shifted in */
  Rb_put_char(&spi_rx_buff, SpiHal_read(), (rb_sz_t)SPI_RB_SIZE);
  /* put data to shift if any */
  if (!Rb_is_empty(&spi_tx_buff) && (Rb_used_size(&spi_tx_buff, (rb_sz_t)SPI_RB_SIZE)>=1))
  {
      SpiHal_transmit((uint8_t)Rb_retrieve_char(&spi_tx_buff, (rb_sz_t)SPI_RB_SIZE));
  }
  else 
  {
      //Last data
      spi.state = SPI_FREE;
  }
    
}