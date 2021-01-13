#ifndef INTERRUPT_HAL_H
#define INTERRUPT_HAL_H

#include "sys_types.h"
#include "conf.h"

typedef enum
{
	I2C1_EV_IRQHandler_num = 0,
	I2C1_ER_IRQHandler_num,
	TIM4_IRQHandler_num,
    TIM3_IRQHandler_num,
    TIM2_IRQHandler_num,
    TIM1_IRQHandler_num,
    RCC_IRQHandler_num,
	SPI1_IRQHandler_num,
	SPI2_IRQHandler_num,
	INT_HAL_TOTAL_ISR
} int_hal_vect_num_t;

base_t IntHal_suspend_global_interrupt(void);
void IntHal_restore_global_interrupt(base_t status);
void IntHal_enable_global_interrupt(void);
void IntHal_vector_init(void);
void IntHal_vector_register(void (*f_pt)(void), int_hal_vect_num_t id);
void IntHal_vector_unregister(int_hal_vect_num_t irq_i);

#endif
