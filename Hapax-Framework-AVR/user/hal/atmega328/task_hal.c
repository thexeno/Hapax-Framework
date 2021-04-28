#include "task_hal.h"
#include "timer_hal.h"


// Metti un monitor (con la time library) della attivita CPU

/* Edit in order to configure appropriately the third part drivers */
/* Can rely on defines in hal_conf.h for low level and MCU specific values, and on app_conf.h */

void taskHal_ConfCounter(task_hal_base_tick_t* basetick) // metti il parametro generico qui in confCounter
{	
	uint16_t comp = 0;
	comp = ((basetick->sys_cpu/1000 * basetick->tick)/64)-1;
	timer_hal_conf[TIMER_1].compare_value = comp; // usa buff conf
	Timer_hal_init_compare(timer_hal_conf[TIMER_1]);
	Timer_hal_set_compare(timer_hal_conf[TIMER_1], comp);
	Timer_hal_ISR_callback_set(void (*fp_t)(timer_hal_ch_t ), timer_hal_ch_t channel)
	//initCompare1(comp, COMP_A_DISCONNECT, 64, TMR_CLK_RISE, CLKOSC, TMR_INT_ON);
}

void taskHal_StartCounter(void)
{
	//startTimer1();
	 //Timer_A_startCounter( TIMER_A0_BASE,TIMER_A_UP_MODE);
	Timer_hal_start_timer(timer_hal_conf[TIMER_1]);
}

// void taskHal_StartDispatch(void)
// {
// 	timer1_interruptMask();
// }

// void taskHal_EndDispatch(void)
// {
// 	timer1_interruptUnmask();
// }