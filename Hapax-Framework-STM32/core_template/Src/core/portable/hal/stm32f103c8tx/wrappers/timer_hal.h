#ifndef TIMER_HAL_H
#define TIMER_HAL_H

#include <sys_types.h>
#include <wrappers/hal.h>

//typedef uint32_t  	  ;
typedef uint32_t      timer_hal_presc_t;
typedef uint32_t      timer_hal_channel_t;
typedef uint32_t      timer_hal_period_t;
typedef uint32_t      timer_hal_pol_t;
typedef uint32_t      gpio_hal_pull_t;

typedef enum
{
	TIMER_1 = 0,
	TIMER_2,
	TIMER_3,
	TIMER_4
} timer_hal_base_t;


typedef enum
{
	TMR_HAL_OK = 0,
	TMR_HAL_INIT_PEND,
	TMR_HAL_ERR
} timer_hal_err_t;

typedef enum
{
	TMR_INPUT_CAPT = 0,
	TMR_OUTPUT_COMP,
	TMR_PWM,
	TMR_PWM_INV,
	TMR_ONE_PULSE
} timer_hal_mode_t;

typedef struct
{
	timer_hal_base_t periph;
	timer_hal_presc_t presc;
	timer_hal_period_t period;
	timer_hal_mode_t mode;
	timer_hal_channel_t channel;
} timer_hal_cfg_t;


void Timer_hal_PWM_start(timer_hal_cfg_t *handle);
timer_hal_err_t Timer_hal_PWM_init(timer_hal_cfg_t *handle);
void Timer_hal_PWM_DC(timer_hal_cfg_t *handle, uint32_t val);

#endif
