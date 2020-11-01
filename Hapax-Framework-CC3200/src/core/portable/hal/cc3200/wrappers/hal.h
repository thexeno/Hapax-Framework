#ifndef HAL_H
#define HAL_H

#include "sys_types.h"
// Driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "interrupt.h"
#include "hw_apps_rcm.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
#include "gpio.h"
#include "pin.h"
#include "interrupt.h"

//typedef uint32_t hwreg_t;
typedef uint32_t cpu_dw_t;
typedef uint32_t base_t;



void Core_hal_init(void);

#endif /* HAL_H_ */
