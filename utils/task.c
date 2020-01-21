/*
 * task.c
 *
 * Created: 16/10/2015 23:33:50
 *  Author: TheXeno
 */ 
#include "task.h"
#include "hal/atmega328/timer_hal.h"

typedef struct {
	int16_t intervalTick;
	int16_t elapsed;
	long lastTick;
	void (*taskFunct)(void);
	uint8_t state;
} task_t;

enum
{
	TASK_IDLE = 0U,
	TASK_READY = 1U,
	TASK_RUN = 2U,
	TASK_QUEUE = 3U
};
/* Task */

static void _task_SysTickIncr(void);

static task_t tasks[TASK_MAX_SUPPORTED];

static signed long taskSystemTick = 0;
static signed long taskSystemTickDispatch = 0;
static uint8_t taskTimeSlotCounter = 0;
static uint8_t taskCreateLock = 0;

static uint8_t taskNum = 0;


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
 * @brief      Retrieve the effective number of the supported  tasks in the task array. Does not consider which task is creted or not, but only the maximum number of supported, at startup.
 *
 * @return     Number of total supported tasks.
 */
uint8_t task_GetTaskNum(void)
{
 	uint8_t a = sizeof(tasks);
 	uint8_t b = sizeof(*tasks);
 	return a/b;
}

/**
 * @brief      Run the task in the array if it is ready for the execution.
 */
void task_Dispatch(void) // tbd misura tempo fuori delle funzioni (task) chiamatiper capire IDLE
                          // metti monitore anche prima di una ISR. l'esecuzione resetta IDLE timer, incrementa il proprio e al next idle timer start, si calcola la percentuale di CPU usata
{
    uint8_t i = 0;
	//base_t st = 0;
	//Gpio_hal_set_value(0, 1);
	
	taskSystemTickDispatch = task_SysTickGet();
    
	for (i=0; i<taskNum; i++)
    {
		if ((taskSystemTickDispatch - tasks[i].lastTick) >= 0)
		{ 
			tasks[i].elapsed = (int16_t)(taskSystemTickDispatch - tasks[i].lastTick);
		}
		else if ((taskSystemTickDispatch - tasks[i].lastTick) < 0)
		{
			tasks[i].elapsed = (int16_t)((TASK_TICK_MAX-tasks[i].lastTick)+taskSystemTickDispatch);
		}
		else
		{
			;
		}	
		
		if (tasks[i].elapsed >= tasks[i].intervalTick)
		{
			tasks[i].state = TASK_RUN;
			// All the instructions before the (*tasks[i].taskFunct)() will introduce jutter in the taske execution time. But sequentiality will be guranteed by reading taskSystemTickDispatch in the task_Dispatch()
			(*tasks[i].taskFunct)();
			tasks[i].lastTick = taskSystemTickDispatch;  // the last tick which the task was executed. Not having the real last one, the period if the scheduling will take into consideration the task execution time.
			tasks[i].state = TASK_IDLE;
		}

	}
	//Gpio_hal_set_value(0, 0);
}


/**
 * @brief      ISR scheduled at a CONF_SYS_TICK, usually in ms. The speed is decided at configuration time, when calling task_ScheduleInit()
 */

void task_SchedulerISR(void)
{

	taskCreateLock = 1;
	
	//Pin_wr_pin(0x10, 1);
	//Pin_wr_pin(0x10, 0);
	//_task_SysTickIncr();
	taskSystemTick++;
}


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
uint8_t task_Create(uint16_t p, void (*func)(void))
{
	if (taskCreateLock == 0 && taskNum < TASK_MAX_SUPPORTED)
	{
		tasks[taskNum].lastTick = 0;
		tasks[taskNum].intervalTick = p; // ms
		tasks[taskNum].taskFunct = func;
		tasks[taskNum].state = TASK_IDLE;
		taskNum++;
	}
	else
	{
		taskCreateLock = 1;
	}
	return taskCreateLock;	
}

/**
 * @brief      Initialze the scheduler with the system tick selected as a quanta.
 *
 * @param[in]  st  the system tick, usually in ms
 */
void task_ScheduleInit(const task_hal_base_tick_t* basetick)
{
	uint16_t comp = 0;
	taskTimeSlotCounter = 0;
	taskCreateLock = 0;
	taskNum = 0;
	taskSystemTick = 0;
	comp = ((basetick->sys_cpu/1000 * basetick->tick)/64)-1;
	//timer_hal_conf[TIMER_1].compare_value = comp;
	Timer_hal_init_compare(&timer_hal_conf[TIMER_1]);
	Timer_hal_set_compare(&timer_hal_conf[TIMER_1], comp);
	Timer_hal_ISR_callback_set(task_SchedulerISR, TIMER_1);
}


/**
 * @brief      Increment the software tick timer. This is called from within the task ISR.
 */
static void _task_SysTickIncr(void)
{
	taskSystemTick++;
}

#warning "define se privata o pubblica"

long task_SysTickGet(void)
{
	long ret = 0;
	base_t int_st = 0;
    int_st = IntHal_suspend_global_interrupt();
	ret = taskSystemTick;
	IntHal_restore_global_interrupt(int_st);
	return ret;
}



/**
 * @brief      Start the scheduler with the initialized tasks
 */
void task_Run(void)
{
	Timer_hal_start_timer(&timer_hal_conf[TIMER_1]);
}

/* EOF */