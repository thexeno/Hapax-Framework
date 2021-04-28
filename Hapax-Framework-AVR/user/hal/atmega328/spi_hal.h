#ifndef SPI_HAL_H
#define	SPI_HAL_H


#include "hal.h"
#include "pin_hal.h"
#include "utils/sys_types.h"


typedef enum
{
	SPI_HAL_OK = 0,
	SPI_HAL_ERR
} spi_hal_err_t;

typedef enum
{
	SPI_MODE_MASTER,
	SPI_MODE_SLAVE
} spi_hal_mode_t;

typedef enum 
{
	SPI_0,
	//UART_1,
	NUM_SPI_CHANNELS
} spi_hal_ch_t;


typedef enum
{
	SPI_HAL_MSB_FIRST = 0,
	SPI_HAL_LSB_FIRST
	//SPI_HAL_BORD_UNCONF
} spi_hal_endian_t;

typedef enum
{
	SPI_HAL_SCK_LEAD_EDGE_SAMP,
	SPI_HAL_SCK_TRAIL_EDGE_SAMP
	//SPI_HAL_CPHA_UNCONF
} spi_hal_clk_pha_t;

typedef enum
{
	SPI_HAL_SCK_LEAD_RISE,
	SPI_HAL_SCK_LEAD_FALL
	//SPI_HAL_CPOL_UNCONF
} spi_hal_clk_pol_t;
	

typedef struct
{
	spi_hal_ch_t channel;  //UARTA0_BASE
	spi_hal_mode_t mode;
	spi_hal_endian_t endian;
	spi_hal_clk_pha_t phase;
	spi_hal_clk_pol_t polarity;
	base_t pin_ss;
	base_t pin_mosi;
	base_t pin_miso;
	base_t pin_sck;
	long	baudrate;
	long	sys_clock;
} spi_hal_cfg_t;

extern register_t volatile * const spdr[NUM_SPI_CHANNELS];

spi_hal_err_t SpiHal_init(const spi_hal_cfg_t* handle);
void SpiHal_ISR_callback_set(void (*fp_t)(spi_hal_ch_t ), spi_hal_ch_t channel);
void SpiHal_transmit(const spi_hal_cfg_t* handle, uint8_t TXData);
uint8_t SpiHal_read(const spi_hal_cfg_t* handle);
void SpiHal_Start(const spi_hal_cfg_t* handle);
void SpiHal_Stop(const spi_hal_cfg_t* handle);
gpio_hal_val_t SpiHal_ss_pin(const spi_hal_cfg_t* handle);
base_t SpiHal_transfer_done(const spi_hal_cfg_t* handle);
void SpiHal_interrupt_disable(const spi_hal_cfg_t* handle);
void SpiHal_interrupt_enable(const spi_hal_cfg_t* handle);
void SpiHal_Clear(const spi_hal_cfg_t* handle);
void SpiHal_ISR_callback_reset(spi_hal_ch_t channel);

#endif