#include "utils/sys_types.h"
#include "hal/atmega328/pin_hal.h"

#define SPI_SS (PORT_B | PIN_2)

typedef enum
{
    SPI_MODE_MASTER,
    SPI_MODE_SLAVE
} spi_mode_e;

typedef enum
{
    SPI_TR_TRANSMIT,
    SPI_TR_TRANSMIT_RECEIVE,
    SPI_TR_RECEIVE,
    SPI_TR_RECEIVE_TRANSMIT
} spi_transact_e;

typedef enum
{
    SPI_DATA_LSB_F,
    SPI_DATA_MSB_F,
    SPI_SCK_POL_HIGH,
	SPI_SCK_POL_LOW,
    SPI_SCK_PHASE
} spi_data_e;


typedef enum
{
    SPI_FREE = 0,
	SPI_MASTER_BUSY,
	SPI_SLAVE_BUSY
} spi_state_e;

struct spi_parameters
{
    spi_state_e state;
	spi_mode_e active_mode;
};

uint8_t Spi_transfer(spi_mode_e mode);
void Spi_put_integer(uint16_t);
void Spi_init(spi_mode_e mode, long speed);
void spi_ISR(void);

