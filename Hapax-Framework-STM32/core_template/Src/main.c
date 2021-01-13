/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
#include "conf.h"
#include "hal.h"
#include "gpio_hal.h"
#include "timer_hal.h"
#include "clock_hal.h"
#include "interrupt_hal.h"
#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

const gpio_hal_cfg_t*  gpio_prova_conf;
const clk_hal_conf_t*  clock_prova_conf;
//const timer_hal_conf_t* timer_prova_conf;
//const timer_hal_oc_conf_t* oc_prova_conf;
//const timer_hal_pwm_conf_t* pwm_prova_conf;
//
//void prova_isr_app(timer_hal_irq_src_t timer)
//{
//	if (timer == TIMER_HAL_CH1) // livello applicativo, puo avere qualisasi nome
//	{
//
//		Gpio_hal_write_value(DEBUG_LED, GPIO_HAL_VAL_FALSE, GPIO_HAL_MASK_AND_UNUSED);  // stesso per gpio, puo avere qualisasi nome
//
//		Timer_hal_OC_period(OC_CHANNEL_TEST_1, (uint16_t)(Timer_hal_OC_get(OC_CHANNEL_TEST_1)+8500));
//		Gpio_hal_write_value(DEBUG_LED, GPIO_HAL_VAL_TRUE, GPIO_HAL_MASK_AND_UNUSED);
//	}
//	else
//	{
//		asm("nop");
//	}
//}


int main(void)
{
    gpio_prova_conf = Gpio_hal_conf_get();
    clock_prova_conf = Clock_hal_conf_get();
	// prepare the eventual additiona HAL layers
	Core_hal_init();
	// clock
	Clock_hal_init(clock_prova_conf);
	// interrupt systeme
	IntHal_vector_init();

	Gpio_hal_init(gpio_prova_conf);
	Gpio_hal_write_value(DEBUG_LED, GPIO_HAL_VAL_TRUE, GPIO_HAL_MASK_AND_UNUSED);
	Gpio_hal_write_value(DEBUG_LED, GPIO_HAL_VAL_FALSE, GPIO_HAL_MASK_AND_UNUSED);
	// Clock_hal_clk_out(RCC_MCO1SOURCE_HSI); // debug - not HAL compliant

//	// //tmr start
//    timer_prova_conf = Timer_hal_conf_get();
//    oc_prova_conf = Timer_hal_OC_conf_get();
//	Timer_hal_init(timer_prova_conf);
//	Timer_hal_set_ISR_cb(TIMER_4, prova_isr_app);
//	Timer_hal_OC_init(oc_prova_conf);
//	Timer_hal_OC_period(OC_CHANNEL_TEST_1, 0xff55);
//
//	IntHal_enable_global_interrupt();
//	Timer_hal_OC_start(OC_CHANNEL_TEST_1);
//	// //pwm
//    pwm_prova_conf = Timer_hal_PWM_conf_get();
//	Timer_hal_PWM_init(pwm_prova_conf);
//	// Timer_hal_PWM_start(&timer_hal_test_conf[0]);
//	Timer_hal_PWM_DC(PWM_CHANNEL_TEST_1, 95);
//	// // OC
//	// Timer_hal_OC_init(&timer_hal_test_conf[1]);
//	Timer_hal_PWM_start(PWM_CHANNEL_TEST_1);
//	// // one pulse
//
//
//	//Timer_hal_OC_period(&timer_hal_test_conf[1], 1500);
//
////int enable


	for(;;);
}
