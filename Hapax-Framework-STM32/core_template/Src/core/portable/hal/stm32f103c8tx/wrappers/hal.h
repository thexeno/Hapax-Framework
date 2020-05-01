#ifndef HAL_H
#define HAL_H

#include "sys_types.h"
#include "stm32f1xx_hal.h"
//#include "stm32f1xx_hal_conf.h"
#include "system_stm32f1xx.h"

//typedef uint32_t hwreg_t;
typedef uint32_t cpu_dw_t;
typedef uint32_t base_t;


void Core_hal_init(void);

#endif /* HAL_H_ */
