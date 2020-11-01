
#ifndef TIMER_HAL_H
#define TIMER_HAL_H

#include "sys_types.h"
#include "conf.h"
#include "hal.h"

//typedef uint32_t      spi_hal_presc_t;

typedef SPI_TypeDef* spi_hal_periph_t;

#define SPI_TOTAL_INSTANCE 4
//#define PWM_TOTAL_INSTANCE CONF_PWM_ENUM_UNUSED // situation can get very custom and numerous, so might be better to allocate only the really used ones
//#define OC_TOTAL_INSTANCE CONF_OC_ENUM_UNUSED // situation can get very custom and numerous, so might be better to allocate only the really used ones


// To keep separation from hardware in application code which uses the callback
typedef enum
{
	SPI_HAL_TX = 0,
	SPI_HAL_RX,
    SPI_HAL_ERR
} spi_hal_irq_src_t;

typedef enum
{
	SPI_HAL_ERR_OK = 0,
	SPI_HAL_ERR_INIT_PEND,
    SPI_HAL_ERR_NO_DEV,
	SPI_HAL_ERR_ERROR
} spi_hal_err_t;


typedef enum
{
	SPI_HAL_INT_ON,
	SPI_HAL_INT_OFF
} spi_hal_int_t;


// Basic timer configuration structure
typedef struct
{
	conf_spi_e spi; // Defined in the application specific enumeration header (conf.h). Will not compile if not properly configured
    spi_hal_periph_t periph;
    spi_hal_mode_t mode;
    spi_hal_word_t dsize;
    spi_hal_cpha_t cpha;
    spi_hal_cpol_t cpol;
} spi_hal_conf_t;

typedef struct
{
    conf_spi_e spi; //for indexing the already partially configurated SPI module
    spi_hal_baud_t baud;
    spi_hal_pha_t pha;
    spi_hal_pol_t pol;
    spi_hal_ss_cb_t ss_cb;
} spi_hal_xfer_t;

// Initialisation functions
spi_hal_err_t Spi_hal_init(const spi_hal_conf_t *handle);
spi_hal_err_t Spi_hal_xfer_start(spi_hal_xfer_t )


void Spi_hal_set_ISR_cb(conf_spi_e tmr, void (*f_pt)(timer_hal_irq_src_t));


#endif
