#ifndef CLOCK_HAL_H
#define CLOCK_HAL_H

#include <sys_types.h>
#include <wrappers/hal.h>

typedef enum
{
	CLK_OK = 0,
	CLK_INIT_PEND,
	CLK_ERR
} clk_hal_err_t;

clk_hal_err_t Clock_hal_init(void);

#endif
