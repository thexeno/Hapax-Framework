
#ifndef SPI_HAL_H
#define SPI_HAL_H

#include "sys_types.h"
#include "conf.h"


/* Definition scheme to bring total elements from the configuration to inside the HAL */
#define SPI_TOTAL_INSTANCE CONF_SPI_ENUM_UNUSED

/* Typedef to locally define peripheral address from the CMSIS available for the ARM part */
typedef SPI_TypeDef* spi_hal_periph_t;

typedef uint32_t spi_hal_baud_t;

/* Typedef to locally define modes from the CMSIS available for the ARM part, allowing not change in the configuration when changing part */
typedef enum  
{
    SPI_HAL_MODE_MASTER = SPI_MODE_SLAVE,
    SPI_HAL_MODE_SLAVE = SPI_MODE_MASTER
} spi_hal_mode_t;

/* Definition scheme to bring peripheral address from the CMSIS to the HAL of the ARM part, allowing not change in the configuration when changing part */
#define SPI_HAL_1 ((spi_hal_periph_t)SPI1)
#define SPI_HAL_2 ((spi_hal_periph_t)SPI2)

/* Type of handle used to refer to a particular transaction from the application level */
typedef int spi_hal_handle_t;

/* define the lenght of a single transaction, default 8bit */
typedef enum  
{
    SPI_HAL_WORD_8BIT = SPI_DATASIZE_8BIT,
    SPI_HAL_WORD_16BIT = SPI_DATASIZE_16BIT
} spi_hal_word_t;

/* Types of error states for internal usage and application check */
typedef enum
{
	SPI_HAL_ERR_OK = 0,
	SPI_HAL_ERR_INIT_PEND,
    SPI_HAL_ERR_NO_DEV,
    SPI_HAL_ERR_LOCK,
	SPI_HAL_ERR_ERROR
} spi_hal_err_t;

/* Interrupt flag options */
typedef enum
{
	SPI_HAL_INT_OFF = 0,
	SPI_HAL_INT_ON
} spi_hal_int_t;

/* Phase options, typedef for allowing no change in the configuration when changing part */
typedef enum
{
	SPI_HAL_PHA_1EDGE = SPI_PHASE_1EDGE,
	SPI_HAL_PHA_2EDGE = SPI_PHASE_2EDGE
} spi_hal_cpha_t;

/* Polarity options, typedef for allowing no change in the configuration when changing part */
typedef enum
{
	SPI_HAL_POL_IDLE_LOW = SPI_POLARITY_LOW,
	SPI_HAL_POL_IDLE_HIGH = SPI_POLARITY_HIGH
} spi_hal_cpol_t;

/* Types of possible IRQ sources from the SPI to be used in the callback definition at the application level */
typedef enum {
    SPI_HAL_ISR_T_ERR = 0,
    SPI_HAL_ISR_T_TXE,
    SPI_HAL_ISR_T_RX
} spi_hal_isr_t;

/* ANSI C99 compatible main SPI data structure to be used per each physical instantiation required.
 * Not all data members have to be initialized. To C99, what is not initialized explicitly at declaration,
 * will be assigned 0, and all the types of the structure are compatible with this default enumeration.
 */ 
typedef struct
{
	conf_spi_e spi; // Defined in the application specific enumeration header (conf.h). Will not compile if not properly configured
    spi_hal_periph_t periph;  // Peripheral redeclared here to shell the part specific HW
    spi_hal_err_t status; // init at OK
    spi_hal_mode_t mode; 
    spi_hal_baud_t baud;
    spi_hal_word_t dsize;
    spi_hal_cpha_t cpha;
    spi_hal_cpol_t cpol;
} spi_hal_conf_t;

// Initialisation functions
spi_hal_err_t Spi_hal_init(const spi_hal_conf_t *handle);
spi_hal_err_t Spi_hal_init_transfer(conf_spi_e spi, uint32_t txd, spi_hal_handle_t* handle);


void Spi_hal_set_ISR_cb(conf_spi_e tmr, void (*f_pt)(timer_hal_irq_src_t));


#endif
