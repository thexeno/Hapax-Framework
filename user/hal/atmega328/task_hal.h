#ifndef TASK_HAL_H_
#define TASK_HAL_H_

#include "hal.h"
#include "utils/sys_types.h"

//#define TASK_CPU_FREQ ACONF_CPU_FREQ
typedef struct 
{
	uint16_t tick;
	uint8_t isr;
	long sys_cpu;
} task_hal_base_tick_t;

extern const task_hal_base_tick_t task_hal_conf;

void taskHal_ConfCounter(task_hal_base_tick_t* st);
void taskHal_StartCounter(void);
void taskHal_StartDispatch(void);
void taskHal_EndDispatch(void);


#endif