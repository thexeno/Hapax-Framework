#include "utils/sys_types.h"
#include "spi_hal.h"
#include "pin_hal.h"
#include "utils/ring_buffer.h"

#define SPI_SS (PORT_B | PIN_2)

typedef enum spi_enumerations
{
    SPI_OK = 0,
    SPI_INIT_PEND,
    SPI_OVERRUN,
    SPI_BUSY,
	SPI_RX_BURST_DONE,
	SPI_TX_BURST_DONE,
    SPI_ERR
} spi_status_t;

typedef enum spi_sm_states
{
	SPI_SM_START = 0,
	SPI_SM_TX_RX,
	SPI_SM_RX,
	SPI_SM_TX
} spi_sm_st_t;

//metti declar nel conf.h

//base_t rx_fifo[SPI_RX_FIFO];
//base_t tx_fifo[SPI_TX_FIFO];

typedef struct spi_device
{
    rb_t spi_tx_buff;
    rb_t spi_rx_buff;
    spi_status_t status;
    base_t items;
    const spi_hal_cfg_t* config;
} spi_dev_st;

uint8_t Spi_transfer(spi_dev_st* console, base_t rx_items);
void Spi_slave_release(spi_dev_st* console);
void Spi_slave_select(spi_dev_st* console);
void Spi_put_integer(spi_dev_st* console, uint16_t c);
void Spi_put_char(spi_dev_st* console, char ch);
void Spi_put_buff(spi_dev_st* console, const char* str);
uint8_t Spi_get_char(spi_dev_st* console);
uint8_t Spi_rx_buffer_elem(spi_dev_st* console);
uint8_t Spi_tx_buffer_elem(spi_dev_st* console);
spi_status_t Spi_get_status(spi_dev_st* console);
spi_status_t Spi_init_stream_in(spi_dev_st* console, const spi_hal_cfg_t* config, uint8_t* local_buff, base_t rx_sz, uint8_t ack, uint8_t nack);

spi_status_t Spi_init(spi_dev_st* console, const spi_hal_cfg_t* config);
spi_status_t Spi_get_status(spi_dev_st* console);
spi_status_t Spi_tx(spi_dev_st* console, uint8_t* buffer, base_t items);
spi_status_t Spi_rx(spi_dev_st* console, uint8_t* buffer, base_t items);
spi_status_t Spi_master_blocking_rx(spi_dev_st* console, uint8_t* buffer, base_t items);
base_t Spi_transaction_items(spi_dev_st* console);
spi_status_t Spi_rx_blocking(spi_dev_st* console, uint8_t* buffer, base_t items);
spi_status_t Spi_tx_blocking(spi_dev_st* console, uint8_t* buffer, base_t items);