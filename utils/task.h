/*
 * task.h
 *
 * Created: 16/10/2015 23:34:05
 *  Author: TheXeno
 */ 


#ifndef TASK_H_
#define TASK_H_


#include "sys_types.h"
#include "hal/atmega328/task_hal.h"
#include "hal/atmega328/interrupt_hal.h"
#include "hal/atmega328/pin_hal.h"


#define TASK_TICK_MAX 40000UL
#define TASK_MAX_SUPPORTED 4U


void task_Dispatch(void);
uint8_t task_GetTaskNum(void);
void task_SchedulerISR(void);
uint8_t task_Create(uint16_t p, void (*func)(void));
void task_ScheduleInit(const task_hal_base_tick_t* st);
long task_SysTickGet(void);
long task_SysTickToMs(void);
void task_Run(void);

/* fine task */




#endif /* TASK_H_ */


