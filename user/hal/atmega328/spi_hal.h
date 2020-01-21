#ifndef SPI_HAL_H
#define	SPI_HAL_H


#include "hal.h"
#include "utils/sys_types.h"

typedef enum
{
	SPI_HAL_MSB_FIRST = 0,
	SPI_HAL_LSB_FIRST
	//SPI_HAL_BORD_UNCONF
} spi_hal_conf_bord_e;

typedef enum
{
	SPI_HAL_SCK_LEAD_EDGE_SAMP,
	SPI_HAL_SCK_TRAIL_EDGE_SAMP
	//SPI_HAL_CPHA_UNCONF
} spi_hal_conf_cpha_e;

typedef enum
{
	SPI_HAL_SCK_LEAD_RISE,
	SPI_HAL_SCK_LEAD_FALL
	//SPI_HAL_CPOL_UNCONF
} spi_hal_conf_cpol_e;
	


uint8_t SpiHal_init_master(spi_hal_conf_bord_e data_order, spi_hal_conf_cpol_e clk_pol, spi_hal_conf_cpha_e clk_phase, long speed);
uint8_t SpiHal_init_slave(spi_hal_conf_bord_e data_order, spi_hal_conf_cpol_e clk_pol, spi_hal_conf_cpha_e clk_phase);
void SpiHal_transmit(uint8_t TXData);
uint8_t SpiHal_read(void);


#endif