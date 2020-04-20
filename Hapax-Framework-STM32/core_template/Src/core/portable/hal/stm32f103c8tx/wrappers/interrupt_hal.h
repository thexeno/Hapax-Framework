#ifndef INTERRUPT_HAL_H
#define INTERRUPT_HAL_H

#include <sys_types.h>

typedef enum
{
	I2C1_EV_IRQHandler_num = 0,
	I2C1_ER_IRQHandler_num,
	TOTAL_ISR_USABLE
} int_hal_vect_num_t;

base_t IntHal_suspend_global_interrupt(void);
void IntHal_restore_global_interrupt(base_t status);
void IntHal_enable_global_interrupt(void);
void IntHal_vector_init(void);
void IntHal_vector_register(void (*f_pt)(void), int_hal_vect_num_t id);
void IntHal_vector_unregister(int_hal_vect_num_t irq_i);

#endif
