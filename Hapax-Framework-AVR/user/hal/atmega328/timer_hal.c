#include "timer_hal.h"
#include "interrupt_hal.h"
#include "utils/sys_types.h"
#include "utils/data_handler.h"


// Metti un monitor (con la time library) della attivita CPU

/* Edit in order to configure appropriately the third part drivers */
/* Can rely on defines in hal_conf.h for low level and MCU specific values, and on app_conf.h */

#warning "TODO: metti clock"
#warning "TODO: gestisci spike 1/255"
#warning "TODO: controlla frequenze"

typedef void (*timer_hal_isr_cb)(timer_hal_ch_t );
timer_hal_isr_cb timer_hal_ISR_cb[NUM_TIMER_CHANNELS];
base_t silicon_bug_glitch_zero_active_A[NUM_TIMER_CHANNELS];
base_t silicon_bug_glitch_zero_active_B[NUM_TIMER_CHANNELS];

register_t volatile * const tccra[NUM_TIMER_CHANNELS] =
{
	(register_t*)&TCCR0A,
	(register_t*)&TCCR1A,
	(register_t*)&TCCR2A
};

register_t volatile * const tccrb[NUM_TIMER_CHANNELS] =
{

	(register_t*)&TCCR0B,
	(register_t*)&TCCR1B,
	(register_t*)&TCCR2B
};

register_t volatile * const timsk[NUM_TIMER_CHANNELS] =
{

	(register_t*)&TIMSK0,
	(register_t*)&TIMSK1,
	(register_t*)&TIMSK2
};

register_t volatile * const ocra[NUM_TIMER_CHANNELS] = 
{
	(register_t*)&OCR0A,
	(register_t*)&OCR1AL,
	(register_t*)&OCR2A
};

register_t volatile * const ocrah[NUM_TIMER_CHANNELS] = 
{
	(register_t*)NULL,
	(register_t*)&OCR1AH,
	(register_t*)NULL
};

register_t volatile * const ocrb[NUM_TIMER_CHANNELS] = 
{
	(register_t*)&OCR0B,
	(register_t*)&OCR1BL,
	(register_t*)&OCR2B
};

register_t volatile * const ocrbh[NUM_TIMER_CHANNELS] =
{
	(register_t*)NULL,
	(register_t*)&OCR1BH,
	(register_t*)NULL
};

register_t volatile * const tcnt[NUM_TIMER_CHANNELS] =
{
	(register_t*)&TCNT0,
	(register_t*)&TCNT1L,
	(register_t*)&TCNT2
};

register_t volatile * const tcnth[NUM_TIMER_CHANNELS] =
{
	(register_t*)NULL,
	(register_t*)&TCNT1H,
	(register_t*)NULL
};

register_t volatile * const tifr[NUM_TIMER_CHANNELS] = 
{
	(register_t*)&TIFR0,
	(register_t*)&TIFR1,
	(register_t*)&TIFR2				
};

/* Private variables and prototypes*/


static void timer2_isr()
{
	// metti cb
	timer_hal_ISR_cb[TIMER_2](TIMER_2); // callback a i.e. ser_rx_char_ISR(ser_dev_st* console)
	
}

static void timer1_isr()
{
	// metti cb
	timer_hal_ISR_cb[TIMER_1](TIMER_1); // callback a i.e. ser_rx_char_ISR(ser_dev_st* console)
	
}

static void timer0_isr()
{
	// metti cb
	timer_hal_ISR_cb[TIMER_0](TIMER_0); // callback a i.e. ser_rx_char_ISR(ser_dev_st* console)
	
}

void Timer_hal_ISR_callback_set(const timer_hal_cfg_t* handle, void (*fp_t)(timer_hal_ch_t ))
{
	timer_hal_ISR_cb[handle->channel] = fp_t; 
}

/* Public variables and prototypes */

static void timer_hal_init(const timer_hal_cfg_t* handle)
{
	*tccra[handle->channel] = 0;
	*tccrb[handle->channel] = 0;
	*timsk[handle->channel] = 0;

	silicon_bug_glitch_zero_active_A[handle->channel] = 0;
	silicon_bug_glitch_zero_active_B[handle->channel] = 0;

	if (handle->channel == TIMER_1)
	{
		/* It does not include many options of the T1 */
		switch (handle->mode)
		{
			case TIMER_HAL_NORMAL:
				// keep 0 
			break;

			case TIMER_HAL_PWM:
				*tccra[handle->channel] |= (1<<WGM00);
			break;

			case TIMER_HAL_COMPARE_MATCH:
				*tccrb[handle->channel] |= (1<<WGM02);

			break;

			case TIMER_HAL_FAST_PWM_FIXED:
				*tccra[handle->channel] |= (1<<WGM00);
				*tccrb[handle->channel] |= (1<<WGM02);
			break;

			case TIMER_HAL_FAST_PWM_CUSTOM:
				*tccra[handle->channel] |= (1<<WGM00);
				*tccra[handle->channel] |= (1<<WGM01);
				*tccrb[handle->channel] |= (1<<WGM02);
				*tccrb[handle->channel] |= (1<<WGM13);
			break;

			default:
				// keep 0 
			break;			
		}		
	}
	else
	{
		switch (handle->mode)
		{
			case TIMER_HAL_NORMAL:
				// keep 0 
			break;

			case TIMER_HAL_PWM:
				*tccra[handle->channel] |= (1<<WGM00);
			break;

			case TIMER_HAL_COMPARE_MATCH:
				*tccra[handle->channel] |= (1<<WGM01);

			break;

			case TIMER_HAL_FAST_PWM_FIXED:
				*tccra[handle->channel] |= (1<<WGM00);
				*tccra[handle->channel] |= (1<<WGM01);
			break;

			case TIMER_HAL_FAST_PWM_CUSTOM:
				*tccra[handle->channel] |= (1<<WGM00);
				*tccra[handle->channel] |= (1<<WGM01);
				*tccrb[handle->channel] |= (1<<WGM02);
			break;

			default:
				// keep 0 
			break;
		}			
	}


	// keep timer off
	*tccrb[handle->channel] &= ~((1 << CS00) | (1 << CS01) | (1 << CS02));

	switch(handle->clock_mode)
	{
		case TIMER_HAL_CLKIN_RISE:
			*tccrb[handle->channel] |= (1<<CS02);
			*tccrb[handle->channel] |= (1<<CS01);
		break;

		case TIMER_HAL_CLKIN_FALL:
			*tccrb[handle->channel] |= (1<<CS02);
			*tccrb[handle->channel] |= (1<<CS01);
			*tccrb[handle->channel] |= (1<<CS00);
		break;
		
		case TIMER_HAL_CLK_OSC:
			// use prescaler value
		break;

		default:
			// use prescaler value
		break;
	}

	switch (handle->int_mode)
	{
		case TIMER_HAL_INT_ON:
			if (handle->mode == TIMER_HAL_NORMAL)
			{
				*timsk[handle->channel] |= (1 << TOIE0);
			}
			/* Altrimenti setti interrupt nelle funzioni di compare init o pwm init */
		break;

		case TIMER_HAL_INT_OFF:
			// keep TOIE0 0
		break;

		default:
		// keep TOIE0 0
		break;
	}
}

void Timer_hal_init_compare(const timer_hal_cfg_t* handle)
{
	timer_hal_init(handle);

	switch(handle->channel)
	{
		case TIMER_0:
			if ((handle->compare_mode == TIMER_HAL_COMP_A_CLEAR)  || 
				(handle->compare_mode == TIMER_HAL_COMP_A_SET)    ||
				(handle->compare_mode == TIMER_HAL_COMP_A_TOGGLE) ||
				(handle->compare_mode == TIMER_HAL_COMP_A_CLEAR)  ||
				(handle->compare_mode == TIMER_HAL_COMP_A_DISCONNECT))
			{
				IntHal_vector_register(timer0_isr, TIMER0_COMPA_vect_num);
			}
			else
			{
				IntHal_vector_register(timer0_isr, TIMER0_COMPB_vect_num);
			}
		break;

		case TIMER_1:
			if ((handle->compare_mode == TIMER_HAL_COMP_A_CLEAR)  || 
				(handle->compare_mode == TIMER_HAL_COMP_A_SET)    ||
				(handle->compare_mode == TIMER_HAL_COMP_A_TOGGLE) ||
				(handle->compare_mode == TIMER_HAL_COMP_A_CLEAR)  ||
				(handle->compare_mode == TIMER_HAL_COMP_A_DISCONNECT))
			{
				IntHal_vector_register(timer1_isr, TIMER1_COMPA_vect_num);
			}
			else
			{
				IntHal_vector_register(timer1_isr, TIMER1_COMPB_vect_num);
			}
		break;
		
		case TIMER_2:
			if ((handle->compare_mode == TIMER_HAL_COMP_A_CLEAR)  || 
				(handle->compare_mode == TIMER_HAL_COMP_A_SET)    ||
				(handle->compare_mode == TIMER_HAL_COMP_A_TOGGLE) ||
				(handle->compare_mode == TIMER_HAL_COMP_A_CLEAR)  ||
				(handle->compare_mode == TIMER_HAL_COMP_A_DISCONNECT))
			{
				IntHal_vector_register(timer2_isr, TIMER2_COMPA_vect_num);
			}
			else
			{
				IntHal_vector_register(timer2_isr, TIMER2_COMPB_vect_num);
			}
		break;
	}

	switch(handle->compare_mode)
	{
		case TIMER_HAL_COMP_A_TOGGLE:
			*tccra[handle->channel] |= (1 << COM0A0);
			*tccra[handle->channel] &= ~(1 << COM0A1);
			/* Externally, the DD6 should be set */
			if (ocrah[handle->channel] != NULL)
				*ocrah[handle->channel] = handle->compare_value >> 8;
			*ocra[handle->channel] = handle->compare_value & 0xff;
		break;

		case TIMER_HAL_COMP_A_CLEAR:
			*tccra[handle->channel] &= ~(1 << COM0A0);
			*tccra[handle->channel] |= (1 << COM0A1);
			/* Externally, the DD6 should be set */
			if (ocrah[handle->channel] != NULL)
				*ocrah[handle->channel] = handle->compare_value >> 8;
			*ocra[handle->channel] = handle->compare_value & 0xff;
		break;

		case TIMER_HAL_COMP_A_SET:
			*tccra[handle->channel] |= ((1 << COM0A0) | (1 << COM0A1));
			/* Externally, the DD6 should be set */
			if (ocrah[handle->channel] != NULL)
				*ocrah[handle->channel] = handle->compare_value >> 8;
			*ocra[handle->channel] = handle->compare_value & 0xff;
		break;

		case TIMER_HAL_COMP_B_TOGGLE:
			*tccra[handle->channel] |= (1 << COM0B0);
			*tccra[handle->channel] &= ~(1 << COM0B1);
			/* Externally, the DD5 should be set */
			if (ocrbh[handle->channel] != NULL)
				*ocrbh[handle->channel] = handle->compare_value >> 8;
			*ocrb[handle->channel] = handle->compare_value & 0xff;
		break;

		case TIMER_HAL_COMP_B_CLEAR:
			*tccra[handle->channel] &= ~(1 << COM0B0);
			*tccra[handle->channel] |= (1 << COM0B1);
			/* Externally, the DD5 should be set */
			if (ocrbh[handle->channel] != NULL)
				*ocrbh[handle->channel] = handle->compare_value >> 8;
			*ocrb[handle->channel] = handle->compare_value & 0xff;
		break;

		case TIMER_HAL_COMP_B_SET:
			*tccra[handle->channel] |= ((1 << COM0B0) | (1 << COM0B1));
			/* Externally, the DD5 should be set */
			if (ocrbh[handle->channel] != NULL)
				*ocrbh[handle->channel] = handle->compare_value >> 8;
			*ocrb[handle->channel] = handle->compare_value & 0xff;
		break;

		case TIMER_HAL_COMP_A_DISCONNECT:
			*tccra[handle->channel] &= ~((1 << COM0A0) | (1 << COM0A1));
			/* Externally, the DD5 should be clear */
			if (ocrah[handle->channel] != NULL)
				*ocrah[handle->channel] = handle->compare_value >> 8;
			*ocra[handle->channel] = handle->compare_value & 0xff;
		break;

		case TIMER_HAL_COMP_B_DISCONNECT:
			*tccra[handle->channel] &= ~((1 << COM0B0) | (1 << COM0B1));
			/* Externally, the DD5 should be clear */
			if (ocrbh[handle->channel] != NULL)
				*ocrbh[handle->channel] = handle->compare_value >> 8;
			*ocrb[handle->channel] = handle->compare_value & 0xff;
		break;

		default:
			*tccra[handle->channel] &= ~((1 << COM0B0) | (1 << COM0B1)
									   | (1 << COM0A0) | (1 << COM0A1));
		break;
	}

	switch(handle->int_mode)
	{
		case TIMER_HAL_INT_ON:
			if ((handle->compare_mode == TIMER_HAL_COMP_A_CLEAR)  || 
				(handle->compare_mode == TIMER_HAL_COMP_A_SET)    ||
				(handle->compare_mode == TIMER_HAL_COMP_A_TOGGLE) ||
				(handle->compare_mode == TIMER_HAL_COMP_A_CLEAR)  ||
				(handle->compare_mode == TIMER_HAL_COMP_A_DISCONNECT))
			{
				*timsk[handle->channel] |= (1 << OCIE0A);
			}
			
			if ((handle->compare_mode == TIMER_HAL_COMP_B_CLEAR)  || 
				(handle->compare_mode == TIMER_HAL_COMP_B_SET)    ||
				(handle->compare_mode == TIMER_HAL_COMP_B_TOGGLE) ||
				(handle->compare_mode == TIMER_HAL_COMP_B_CLEAR)  ||
				(handle->compare_mode == TIMER_HAL_COMP_B_DISCONNECT))
			{
				*timsk[handle->channel] |= (1 << OCIE0B);
			}
		break;
	
		case TIMER_HAL_INT_OFF:
			if ((handle->compare_mode == TIMER_HAL_COMP_A_CLEAR)  || 
				(handle->compare_mode == TIMER_HAL_COMP_A_SET)    ||
				(handle->compare_mode == TIMER_HAL_COMP_A_TOGGLE) ||
				(handle->compare_mode == TIMER_HAL_COMP_A_CLEAR)  ||
				(handle->compare_mode == TIMER_HAL_COMP_A_DISCONNECT))
			{
				*timsk[handle->channel] &= ~(1 << OCIE0A);
			}
			
			if ((handle->compare_mode == TIMER_HAL_COMP_B_CLEAR)  || 
				(handle->compare_mode == TIMER_HAL_COMP_B_SET)    ||
				(handle->compare_mode == TIMER_HAL_COMP_B_TOGGLE) ||
				(handle->compare_mode == TIMER_HAL_COMP_B_CLEAR)  ||
				(handle->compare_mode == TIMER_HAL_COMP_B_DISCONNECT))
			{
				*timsk[handle->channel] &= ~(1 << OCIE0B);
			}
		break;

		default:
			*timsk[handle->channel] &= ~(1 << OCIE0A);
			*timsk[handle->channel] &= ~(1 << OCIE0B);
		break;
	}	
}

void Timer_hal_set_compare(const timer_hal_cfg_t* handle, timer_hal_comp_t comp)
{
	timer_hal_comp_t tmp = (comp >> 8);
	if ((handle->compare_mode == TIMER_HAL_COMP_A_CLEAR)  ||
	(handle->compare_mode == TIMER_HAL_COMP_A_SET)    ||
	(handle->compare_mode == TIMER_HAL_COMP_A_TOGGLE) ||
	(handle->compare_mode == TIMER_HAL_COMP_A_CLEAR)  ||
	(handle->compare_mode == TIMER_HAL_COMP_A_DISCONNECT))
	{
//		OCR1AH = (comp >> 8);
//		OCR1AL = comp & 0xff;
		if (ocrah[handle->channel] != NULL)
			*ocrah[handle->channel] = comp >> 8;
		*ocra[handle->channel] =  comp & 0xff;
		//OCR1A  = comp;
	}
	
	if ((handle->compare_mode == TIMER_HAL_COMP_B_CLEAR)  ||
	(handle->compare_mode == TIMER_HAL_COMP_B_SET)    ||
	(handle->compare_mode == TIMER_HAL_COMP_B_TOGGLE) ||
	(handle->compare_mode == TIMER_HAL_COMP_B_CLEAR)  ||
	(handle->compare_mode == TIMER_HAL_COMP_B_DISCONNECT))
	{
		if (ocrbh[handle->channel] != NULL)
			*ocrbh[handle->channel] = comp >> 8;
		*ocrb[handle->channel] = comp & 0xff;
	}
}

void Timer_hal_write_timer(const timer_hal_cfg_t* handle, uint16_t val)
{
	// take care when using 8 bit values
	if (tcnth[handle->channel] != NULL)
		*tcnth[handle->channel] = val >> 8;
	*tcnt[handle->channel] = val & 0xff;
}

void Timer_hal_start_timer(const timer_hal_cfg_t* handle)
{
	//*tccrb[handle->channel] &= ~((1 << CS00) | (1 << CS01) | (1 << CS02));
	if (handle->channel == TIMER_2)
	{
		switch (handle->prescaler)
		{
			case 1:
			*tccrb[handle->channel] |= 1;
			break;
			
			case 8:
			*tccrb[handle->channel] |= 2;
			break;
			
			case 32:
			*tccrb[handle->channel] |= 3;
			break;
			
			case 64:
			*tccrb[handle->channel] |= 4;
			break;
			
			case 128:
			*tccrb[handle->channel] |= 5;
			break;
			
			case 256:
			*tccrb[handle->channel] |= 6;
			break;

			case 1024:
			*tccrb[handle->channel] |= 7;
			break;

			default: /* spento se valore errato */
			*tccrb[handle->channel] |= 0;
			break;
		}
	}
	else
	{
		switch (handle->prescaler)
		{
			case 1:
			*tccrb[handle->channel] |= 1;
			break;
			
			case 8:
			*tccrb[handle->channel] |= 2;
			break;
			
			case 64:
			*tccrb[handle->channel] |= 3;
			break;
			
			case 256:
			*tccrb[handle->channel] |= 4;
			break;
			
			case 1024:
			*tccrb[handle->channel] |= 5;
			break;
			
			default: /* spento se valore errato */
			*tccrb[handle->channel] |= 0;
			break;
		}	
	}
	
}

void Timer_hal_stop_timer(const timer_hal_cfg_t* handle)
{
	*tccrb[handle->channel] &= ~((1 << CS00) | (1 << CS01) | (1 << CS02));
}



void Timer_hal_init_PWM(const timer_hal_cfg_t* handle)
{
	timer_hal_init(handle);

	if (handle->int_mode == TIMER_HAL_INT_ON)
	{
		switch(handle->channel)
		{
			case TIMER_0:
				IntHal_vector_register(timer0_isr, TIMER0_OVF_vect_num);
			break;

			case TIMER_1:
				IntHal_vector_register(timer1_isr, TIMER1_OVF_vect_num);
			break;
			
			case TIMER_2:
				IntHal_vector_register(timer2_isr, TIMER2_OVF_vect_num);
			break;
		}
		*timsk[handle->channel] = (1 << TOIE0);
	}


	switch(handle->pwm_channel)
	{
		case TIMER_HAL_PWM_A:
			switch(handle->pwm_polarity)
			{
				case TIMER_HAL_PWM_POL_NORMAL:
					*tccra[handle->channel] &= ~(1 << COM0A0);
					*tccra[handle->channel] |= (1 << COM0A1);
					if (ocrah[handle->channel] != NULL)
						*ocrah[handle->channel] = 0;					
					*ocra[handle->channel] = 0;
					// remember the bug:
					//pwmPol0A = pol;
				break;

				case TIMER_HAL_PWM_POL_INVERT:
					*tccra[handle->channel] |= (1 << COM0A1);
					*tccra[handle->channel] |= (1 << COM0A0);
					if (ocrah[handle->channel] != NULL)
						*ocrah[handle->channel] = 0xff;
					*ocra[handle->channel] = 0xff;
				break;

				default:
				// output compare?
				break;
			}
		break;

		case TIMER_HAL_PWM_B:
			switch(handle->pwm_polarity)
			{
				case TIMER_HAL_PWM_POL_NORMAL:
					*tccra[handle->channel] &= ~(1 << COM0B0);
					*tccra[handle->channel] |= (1 << COM0B1);
					if (ocrbh[handle->channel] != NULL)
						*ocrbh[handle->channel] = 0;					
					*ocrb[handle->channel] = 0;
					// remember the bug:
					//pwmPol0A = pol;
				break;

				case TIMER_HAL_PWM_POL_INVERT:
					*tccra[handle->channel] |= (1 << COM0B1);
					*tccra[handle->channel] |= (1 << COM0B0);
					if (ocrbh[handle->channel] != NULL)
						*ocrbh[handle->channel] = 0xff;					
					*ocrb[handle->channel] = 0xff;
				break;

				default:
				break;
			}
		break;
		
		default:
		// nothing
		break;
	}
	/* DC = 0%*/
	//Timer_hal_PWM_DC(handle, TIMER_HAL_PWM_A, 0);
	//pwm0Pins(pol);
//	timer_hal_pwm_silicon_bug_override()

}

void Timer_hal_PWM_period(const timer_hal_cfg_t* handle, timer_hal_comp_t per)
{
	if (handle->mode == TIMER_HAL_FAST_PWM_CUSTOM)
	{
		if (ocrah[handle->channel] != NULL)
			*ocrah[handle->channel] = per >> 8;	
		*ocra[handle->channel] = per & 0xff;
	}
}

void Timer_hal_PWM_DC_direct_B(timer_hal_ch_t ch, timer_hal_comp_t dc)
{
	if (ocrbh[ch] != NULL)
		*ocrbh[ch] = dc >> 8;
	*ocrb[ch] = dc & 0xff;
}

void Timer_hal_PWM_DC_direct_A(timer_hal_ch_t ch, timer_hal_comp_t dc)
{
	if (ocrah[ch] != NULL)
		*ocrah[ch] = dc >> 8;
	*ocra[ch] = dc & 0xff;
}

void Timer_hal_clear_pwm_interrupt(const timer_hal_cfg_t* handle)
{
	*tifr[handle->channel] &= ~(1 << TOV0);
}


void Timer_hal_PWM_DC(const timer_hal_cfg_t* handle, timer_hal_pwm_ch_t ch, timer_hal_comp_t dc)
{
	switch(ch)
	{
		case TIMER_HAL_PWM_A:
			if (handle->pwm_polarity == TIMER_HAL_PWM_POL_NORMAL && dc == 0)
			{
				// put invert and set max DC, meaning 0%
				*tccra[handle->channel] |= (1 << COM0A1);
				*tccra[handle->channel] |= (1 << COM0A0);
				if (ocrah[handle->channel] != NULL)
					*ocrah[handle->channel] = 0xff;
				*ocra[handle->channel] = 0xff;
				silicon_bug_glitch_zero_active_A[handle->channel] = 1;
			}
			else if (handle->pwm_polarity == TIMER_HAL_PWM_POL_INVERT && dc == 0)
			{
				// put normal and set max DC, meaning 100%
				*tccra[handle->channel] &= ~(1 << COM0A0);
				*tccra[handle->channel] |= (1 << COM0A1);
				if (ocrah[handle->channel] != NULL)
					*ocrah[handle->channel] = 0xff;
				*ocra[handle->channel] = 0xff;
				silicon_bug_glitch_zero_active_A[handle->channel] = 1;
			}
			else
			{
				if (silicon_bug_glitch_zero_active_A[handle->channel] != 0)
				{
					if (handle->pwm_polarity == TIMER_HAL_PWM_POL_INVERT)
					{
						// restore original settings
						*tccra[handle->channel] |= (1 << COM0A1);
						*tccra[handle->channel] |= (1 << COM0A0);
					}
					else
					{
						*tccra[handle->channel] &= ~(1 << COM0A0);
						*tccra[handle->channel] |= (1 << COM0A1);
					}
					silicon_bug_glitch_zero_active_A[handle->channel] = 0;
				}
				if (ocrah[handle->channel] != NULL)
					*ocrah[handle->channel] = dc >> 8;					
				*ocra[handle->channel] = dc & 0xff;	
			}
			
		break;

		case TIMER_HAL_PWM_B:
			if (handle->pwm_polarity == TIMER_HAL_PWM_POL_NORMAL && dc == 0)
			{
				// put invert and set max DC, meaning 0%
				*tccra[handle->channel] |= (1 << COM0B1);
				*tccra[handle->channel] |= (1 << COM0B0);
				if (ocrbh[handle->channel] != NULL)
					*ocrbh[handle->channel] = 0xff;
				*ocrb[handle->channel] = 0xff;
				silicon_bug_glitch_zero_active_B[handle->channel] = 1;
			}
			else if (handle->pwm_polarity == TIMER_HAL_PWM_POL_INVERT && dc == 0)
			{
				// put normal and set max DC, meaning 100%
				*tccra[handle->channel] &= ~(1 << COM0B0);
				*tccra[handle->channel] |= (1 << COM0B1);
				if (ocrbh[handle->channel] != NULL)
					*ocrbh[handle->channel] = 0xff;
				*ocrb[handle->channel] = 0xff;
				silicon_bug_glitch_zero_active_B[handle->channel] = 1;
			}
			else
			{
				if (silicon_bug_glitch_zero_active_B[handle->channel] != 0)
				{
					if (handle->pwm_polarity == TIMER_HAL_PWM_POL_INVERT)
					{
						// restore original settings
						*tccra[handle->channel] |= (1 << COM0B1);
						*tccra[handle->channel] |= (1 << COM0B0);
					}
					else
					{
						*tccra[handle->channel] &= ~(1 << COM0B0);
						*tccra[handle->channel] |= (1 << COM0B1);
					}
					silicon_bug_glitch_zero_active_B[handle->channel] = 0;
				}
				if (ocrah[handle->channel] != NULL)
					*ocrah[handle->channel] = dc >> 8;
				*ocra[handle->channel] = dc & 0xff;
			}
		break;

		default:
		break;
	}
}


/*
void timerHal_ConfCounter(uint16_t comp) // metti il parametro generico qui in confCounter
{	
	uint8_t tmrDiv = 64;
    #warning "metti cpu freq in a config struct"
	//comp = ((CONF_CPU_FREQ/1000 * comp)/tmrDiv)-1;
	initCompare1(comp, COMP_A_DISCONNECT, tmrDiv, TMR_CLK_RISE, CLKOSC, TMR_INT_ON);
}

void timerHal_StartCounter(void)
{
	startTimer1();
	 //Timer_A_startCounter( TIMER_A0_BASE,TIMER_A_UP_MODE);
}
*/
