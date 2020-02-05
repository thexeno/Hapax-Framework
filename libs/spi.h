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
    SPI_ERR
} spi_status_t;

//metti declar nel conf.h

typedef struct spi_device
{
    rb_t spi_tx_buff;
    rb_t spi_rx_buff;
    spi_status_t status;
    base_t rx_items;
    const spi_hal_cfg_t* config;
} spi_dev_st;

uint8_t Spi_transfer(spi_dev_st* console, base_t rx_items);
void Spi_slave_release(spi_dev_st* console);
void Spi_slave_select(spi_dev_st* console);
void Ser_put_integer(spi_dev_st* console, uint16_t c);
void Ser_put_char(spi_dev_st* console, char ch);
void Spi_put_buff(spi_dev_st* console, const char* str);
spi_status_t Spi_init(spi_dev_st* console, const spi_hal_cfg_t* config, uint8_t* txb, base_t tx_sz, uint8_t* rxb, base_t rx_sz);
