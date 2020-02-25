#ifndef TIMER_HAL_H_
#define	TIMER_HAL_H_

#include "hal.h"
#include "utils/sys_types.h"


typedef enum 
{
	TIMER_HAL_NORMAL = 0,
	TIMER_HAL_PWM,
	TIMER_HAL_COMPARE_MATCH,
	TIMER_HAL_FAST_PWM_CUSTOM,
	TIMER_HAL_FAST_PWM_FIXED
} timer_hal_mode_t;

typedef enum 
{
	TIMER_HAL_CLKIN_RISE = 0,
	TIMER_HAL_CLKIN_FALL,
	TIMER_HAL_CLK_OSC
} timer_hal_clk_mode_t;

typedef enum 
{
	TIMER_HAL_INT_ON = 0,
	TIMER_HAL_INT_OFF
} timer_hal_int_mode_t;

typedef enum 
{
	TIMER_HAL_COMP_A_TOGGLE = 0,
	TIMER_HAL_COMP_A_CLEAR,
	TIMER_HAL_COMP_A_SET,
	TIMER_HAL_COMP_A_DISCONNECT,

	TIMER_HAL_COMP_B_TOGGLE = 128,
	TIMER_HAL_COMP_B_CLEAR,
	TIMER_HAL_COMP_B_SET,
	TIMER_HAL_COMP_B_DISCONNECT
} timer_hal_comp_mode_t;

typedef uint16_t timer_hal_presc_t;
typedef uint16_t timer_hal_comp_t;

typedef enum 
{
	TIMER_HAL_PWM_A = 0,
	TIMER_HAL_PWM_B,
	TIMER_HAL_PWM_UNUSED
} timer_hal_pwm_ch_t;

typedef enum 
{
	TIMER_HAL_PWM_POL_NORMAL = 0,
	TIMER_HAL_PWM_POL_INVERT,
	TIMER_HAL_PWM_POL_UNUSED
} timer_hal_pwm_pol_t;

typedef enum 
{
	TIMER_0 = 0,
	TIMER_1,
	TIMER_2,
	NUM_TIMER_CHANNELS
} timer_hal_ch_t;

typedef struct
{
	timer_hal_ch_t channel;
	timer_hal_mode_t mode;
	timer_hal_clk_mode_t clock_mode;
	timer_hal_int_mode_t int_mode;
	timer_hal_comp_mode_t compare_mode;
	timer_hal_comp_t compare_value;
	timer_hal_presc_t prescaler;
	timer_hal_pwm_ch_t pwm_channel;
	timer_hal_pwm_pol_t pwm_polarity;
} timer_hal_cfg_t;

extern const timer_hal_cfg_t timer_hal_conf[];

void Timer_hal_init_compare(const timer_hal_cfg_t* handle);
void Timer_hal_set_compare(const timer_hal_cfg_t* handle, timer_hal_comp_t comp);
void Timer_hal_write_timer(const timer_hal_cfg_t* handle, uint16_t val);
void Timer_hal_start_timer(const timer_hal_cfg_t* handle);
void Timer_hal_stop_timer(const timer_hal_cfg_t* handle);
void Timer_hal_init_PWM(const timer_hal_cfg_t* handle);
void Timer_hal_PWM_DC(const timer_hal_cfg_t* handle, 
							timer_hal_pwm_ch_t ch, 
							timer_hal_comp_t dc);
void Timer_hal_PWM_period(const timer_hal_cfg_t* handle, timer_hal_comp_t dc);
void Timer_hal_PWM_DC_direct_B(timer_hal_ch_t ch, timer_hal_comp_t dc);
void Timer_hal_PWM_DC_direct_A(timer_hal_ch_t ch, timer_hal_comp_t dc);
void Timer_hal_clear_pwm_interrupt(const timer_hal_cfg_t* handle);

void Timer_hal_ISR_callback_set(const timer_hal_cfg_t* handle, void (*fp_t)(timer_hal_ch_t ));

#endif