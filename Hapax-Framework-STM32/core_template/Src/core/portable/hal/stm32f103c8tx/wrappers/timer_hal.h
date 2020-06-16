#ifndef TIMER_HAL_H
#define TIMER_HAL_H

#include "sys_types.h"
#include "conf.h"
#include "hal.h"

typedef uint32_t      timer_hal_presc_t;
typedef uint32_t      timer_hal_oc_mode_t;
typedef uint32_t      timer_hal_pwm_mode_t;
typedef uint32_t      timer_hal_pwm_pol_t;
typedef uint32_t      timer_hal_oc_pol_t;
typedef uint32_t      timer_hal_period_t;
typedef uint32_t      timer_hal_channel_t

typedef TIM_TypeDef*  timer_hal_periph_t;

#define TIMER_TOTAL_INSTANCE 4
#define PWM_TOTAL_ISTANCE CONF_PWM_ENUM_UNUSED // situation can get very custom and numerous, so might be better to allocate only the really used ones
#define OC_TOTAL_ISTANCE CONF_OC_ENUM_UNUSED // situation can get very custom and numerous, so might be better to allocate only the really used ones



typedef enum
{
	TIMER_HAL_CH1 = 0,
	TIMER_HAL_CH2,
	TIMER_HAL_CH3,
	TIMER_HAL_CH4
} timer_hal_irq_src_t;

typedef enum
{
	TIMER_HAL_ERR_OK = 0,
	TIMER_HAL_ERR_INIT_PEND,
    TIMER_HAL_ERR_NO_TIMER,
	TIMER_HAL_ERR_ERROR
} timer_hal_err_t;


typedef enum
{
	TIMER_HAL_INT_ON,
	TIMER_HAL_INT_OFF
} timer_hal_int_t;


// Basic timer configuration structure
typedef struct
{
	conf_timer_e tmr; // Defined in the application specific enumeration header (conf.h). Will not compile if not properly configured
    timer_hal_periph_t periph;
	timer_hal_presc_t presc;
	timer_hal_period_t period;
} timer_hal_conf_t;

// PWM configuration structure. The <conf_timer_e> indicates it relies on the basic timer, hence must be configured first.
typedef struct
{
	conf_pwm_e pwm_enum;    // Defined in the application specific enumeration header (conf.h). Will not compile if not properly configured
	conf_timer_e tmr;    // Defined in the application specific enumeration header (conf.h). Will not compile if not properly configured
	timer_hal_channel_t channel;
    timer_hal_pwm_mode_t mode;
	timer_hal_pol_t pol;
	timer_hal_int_t interrupt;
} timer_hal_pwm_conf_t;

// OC configuration structure. The <conf_timer_e> indicates it relies on the basic timer, hence must be configured first.
typedef struct
{
	conf_oc_e pwm_enum;    // Defined in the application specific enumeration header (conf.h). Will not compile if not properly configured
	conf_timer_e tmr;      // Defined in the application specific enumeration header (conf.h). Will not compile if not properly configured
	timer_hal_channel_t channel;
    timer_hal_oc_mode_t mode;
	timer_hal_pol_t pol;
	timer_hal_int_t interrupt;
} timer_hal_oc_conf_t;

/* xxx_conf_t:
 * Declared in a conf.c file according to the configuration desired,
 * enumerated in in a designated application defines conf.h header.
 */
extern const timer_hal_conf_t * const Timer_hal_conf_get(void);
extern const timer_hal_pwm_conf_t * const Timer_hal_PWM_conf_get(void);

// Initialisation functions
timer_hal_err_t Timer_hal_init(timer_hal_conf_t *handle);
timer_hal_err_t Timer_hal_PWM_init(timer_hal_pwm_t *handle);
timer_hal_err_t Timer_hal_OC_init(timer_hal_conf_t *handle);

// Timer control functions
void Timer_hal_PWM_start(conf_pwm_e pwm);
void Timer_hal_OC_start(timer_hal_conf_t *handle);

// Timing functions
void Timer_hal_OC_period(timer_hal_conf_t *handle, uint32_t val);
void Timer_hal_PWM_DC(timer_hal_pwm_t *handle, conf_pwm_e pwm, uint32_t val);
uint32_t Timer_hal_OC_get(timer_hal_conf_t *handle);

// Not MUC standard functions (i.e. you hardly find those in cheaper MCU families, TAKE CARE when writing portable code)
timer_hal_err_t Timer_NotStd_hal_pulse_enable(timer_hal_conf_t *handle);
timer_hal_err_t Timer_NotStd_hal_pulse_disable(timer_hal_conf_t *handle);

void Timer_hal_set_ISR_cb(conf_timer_e tmr, void (*f_pt)(timer_hal_channel_t));


#endif
