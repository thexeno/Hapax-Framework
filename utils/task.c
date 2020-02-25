/*
 * task.c
 *
 * Created: 16/10/2015 23:33:50
 *  Author: TheXeno
 */ 
#include "task.h"
#include "timer_hal.h"
#include "interrupt_hal.h"
#include "pin_hal.h"





static task_tick_t task_system_tick = 0;

/* Task */
static void task_systick_increment_ISR(void);



static task_tick_t task_system_tick_schedule = 0;
static task_lock_t task_create_lock = TASK_LOCK_ENABLE; // so that withoput init, no task will be assigned
static uint8_t task_num = 0;


/**
 * @brief      Get the pointer of the task table structure, from which the task array structure can be used.
 *
 * @return     The base pointer of the structure.
 */
//task_t* task_GetConfPtr(void)
//{
	//return (&tasks);
//}



/**
 * @brief      Run the task in the array if it is ready for the execution.
 */
void Task_idle(const task_conf_t* conf) // tbd misura tempo fuori delle funzioni (task) chiamatiper capire IDLE
                          // metti monitore anche prima di una ISR. l'esecuzione resetta IDLE timer, incrementa il proprio e al next idle timer start, si calcola la percentuale di CPU usata
{
    uint8_t i = 0;
	//base_t st = 0;
	//Gpio_hal_set_value(0, 1);
	
	task_system_tick_schedule = Task_systick_get();
    
	for (i=0; i<task_num; i++)
    {
		if ((task_system_tick_schedule - tasks[i].last_tick) >= 0)
		{ 
			tasks[i].elapsed = (task_tick_t)(task_system_tick_schedule - tasks[i].last_tick);
		}
		else if ((task_system_tick_schedule - tasks[i].last_tick) < 0)
		{
			tasks[i].elapsed = (task_tick_t)((conf->max_tick_no_wrap - tasks[i].last_tick)+task_system_tick_schedule);
		}
		else
		{
			;
		}	
		
		if (tasks[i].elapsed >= tasks[i].interval_tick)
		{
			tasks[i].state = TASK_RUN;
			// All the instructions before the (*tasks[i].taskFunct)() will introduce jutter in the taske execution time. But sequentiality will be guranteed by reading task_system_tick_schedule in the task_Dispatch()
			(*tasks[i].task_fp)();
			tasks[i].last_tick = task_system_tick_schedule;  // the last tick which the task was executed. Not having the real last one, the period if the scheduling will take into consideration the task execution time.
			tasks[i].state = TASK_IDLE;
		}

	}
	//Gpio_hal_set_value(0, 0);
}


/**
 * @brief      ISR scheduled at a CONF_SYS_TICK, usually in ms. The speed is decided at configuration time, when calling task_ScheduleInit()
 */


/**
 * @brief      Create a schedulable task for the system, registering it in one slot of the task array available.
 *
 * @param[in]  p     is the execution period of the given task
 * @param[in]  func  The task function desired
 *
 * @return     The task lock status. If ZERO, no tasks are in execution and a new one has been added. IF NOT ZERO, as a protection mechanism from accidental creations,
 * 			   something went wrong as the scheduler already started. At this point the scheduler must be interrupted, 
 * 			   the task_ScheduleInit() called again and tryagain to create the task.
 */

//todo metti messaggi errore, non 1 e 0
task_lock_t Task_create(const task_conf_t* conf, task_tick_t p, void (*func_p)(void))
{
	if (task_create_lock == TASK_LOCK_DISABLE && task_num < conf->total_tasks)
	{
		tasks[task_num].last_tick = 0;
		tasks[task_num].interval_tick = p; // ms
		tasks[task_num].task_fp = func_p;
		tasks[task_num].state = TASK_IDLE;
		task_num++;
	}
	else
	{
		task_create_lock = TASK_LOCK_ENABLE;
	}
	return task_create_lock;	
}

/**
 * @brief      Initialze the scheduler with the system tick selected as a quanta.
 *
 * @param[in]  st  the system tick, usually in ms
 */
void Task_scheduler_init(const task_conf_t* conf)
{
	timer_hal_comp_t comp = 0;
	task_create_lock = TASK_LOCK_DISABLE;
	task_num = 0;
	task_system_tick = 0;
	comp = ((conf->sys_clock/1000000UL * conf->tick_quanta)/conf->timer->prescaler)-1;
	Timer_hal_init_compare(conf->timer);
	Timer_hal_set_compare(conf->timer, comp);
	Timer_hal_ISR_callback_set(conf->timer, (void*)task_systick_increment_ISR);
}


/**
 * @brief      Increment the software tick timer. This is called from within the task ISR.
 */
void task_systick_increment_ISR(void)
{
	task_system_tick+=1000;
}



/**
 * @brief      Start the scheduler with the initialized tasks
 */
void Task_scheduler_start(const task_conf_t* conf)
{
	task_create_lock = TASK_LOCK_ENABLE;
	Timer_hal_write_timer(conf->timer, 258);
	Timer_hal_start_timer(conf->timer);
}

void Task_scheduler_stop(const task_conf_t* conf)
{
	// tbd
}

task_tick_t Task_systick_get(void)
{
	task_tick_t ret = 0;
	base_t int_st = 0;
    int_st = IntHal_suspend_global_interrupt();
	ret = task_system_tick;
	IntHal_restore_global_interrupt(int_st);
	return ret;
}


/* EOF */