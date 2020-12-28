#ifndef CLOCK_HAL_H
#define CLOCK_HAL_H

#include "sys_types.h"
#include "hal.h"


#define CLOCK_HAL_HSI_OSC 8000000UL

typedef enum
{
	CLK_OK = 0,
	CLK_INIT_PEND,
	CLK_ERR
} clock_hal_err_t;

typedef struct
{
	unsigned long ahb_freq;
    unsigned long apb1_freq;
    unsigned long apb2_freq;
    unsigned long source;
} clk_hal_conf_t;

clock_hal_err_t Clock_hal_init(const clk_hal_conf_t *handle);
void Clock_hal_clk_out(uint32_t src);
uint32_t Clock_hal_get_info(uint32_t info);
const clk_hal_conf_t * Clock_hal_conf_get(void);


#endif
