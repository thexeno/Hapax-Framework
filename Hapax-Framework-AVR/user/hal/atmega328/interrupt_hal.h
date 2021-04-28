#ifndef INTERRUPT_HAL_H
#define INTERRUPT_HAL_H

#include "utils/sys_types.h"



base_t IntHal_suspend_global_interrupt(void);
void IntHal_restore_global_interrupt(base_t status);
void IntHal_enable_global_interrupt(void);
void IntHal_vector_init(void);
void IntHal_vector_register(void (*f_pt)(void), uint8_t id);
void IntHal_vector_unregister(uint8_t irq_i);

#endif