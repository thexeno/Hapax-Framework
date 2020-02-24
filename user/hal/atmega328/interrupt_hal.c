#include "hal.h"
#include "interrupt_hal.h"

#define INTERRUPT_MAX_SUPPORTED_ISR 26U
typedef void (*isr_pt_t)(void);


typedef enum {
	INTERRUPT_ASSIGNED,
	INTERRUPT_UNASSIGNED,
	INTERRUPT_DEFAULT
} int_hal_state_t;

struct isr_array
{
	isr_pt_t isr_pt;
	int_hal_state_t state;
};

struct isr_array isr_pt_array[INTERRUPT_MAX_SUPPORTED_ISR];

static void IntHal_default_ISR(void)
{
	asm("nop");
	while(1);
}


base_t IntHal_suspend_global_interrupt(void)
{
	//__disable_interrupt();
	base_t oldSREG = SREG;
	cli();
	return oldSREG;
}

void IntHal_restore_global_interrupt(base_t status)
{
	//__enable_interrupt();
	SREG = status;
}

void IntHal_enable_global_interrupt(void)
{
	sei();
}

void IntHal_vector_init(void)
{
	uint8_t temp = 0;
	//conf which part of memory to start. If common to other platforms, use a #define  to configure the thype
	//for now just fixed in a certain way (start at 0x0000)
	temp = MCUCR;
	/* Enable change of Interrupt Vectors */
	MCUCR = temp|(1<<IVCE);
	/* Move interrupts to 0x0002 */
	MCUCR = temp & (~(1<<IVSEL));
	cli();
	for (temp=0; temp<INTERRUPT_MAX_SUPPORTED_ISR; temp++)
	{
		isr_pt_array[temp].isr_pt = &IntHal_default_ISR;
		isr_pt_array[temp].state = INTERRUPT_DEFAULT;	
	}
}


void IntHal_vector_register(void (*f_pt)(void), uint8_t irq_i)
{
	// unsigned long *ulNvicTbl;
	// ulNvicTbl = (ivTable_t*)CONF_REG(HAL_CONF_VTABLE); // check alignment: type of pointer. Try with CC3200
	// ulNvicTbl[id] = (unsigned long)f_pt;
	if (isr_pt_array[irq_i].state != INTERRUPT_ASSIGNED)
	{
		isr_pt_array[irq_i].isr_pt = f_pt;
		//isr_pt_array[irq_i].iv_id  = 
		isr_pt_array[irq_i].state = INTERRUPT_ASSIGNED;           // otherwise marked as unassigned and properly handled
	}
}

void IntHal_vector_unregister(uint8_t irq_i)
{
	isr_pt_array[irq_i].isr_pt = &IntHal_default_ISR;
	isr_pt_array[irq_i].state = INTERRUPT_UNASSIGNED;         
}



ISR(TIMER1_COMPA_vect)
{
	isr_pt_array[TIMER1_COMPA_vect_num].isr_pt();
}

ISR(TIMER1_COMPB_vect)
{
	//	task();
	isr_pt_array[TIMER1_COMPB_vect_num].isr_pt();
}


ISR(USART_RX_vect)
{
//	task();
	isr_pt_array[USART_RX_vect_num].isr_pt();
}


ISR(USART_UDRE_vect)
{
//	task();
	isr_pt_array[USART_UDRE_vect_num].isr_pt();
}

ISR(USART_TX_vect)
{
//	task();
	isr_pt_array[USART_TX_vect_num].isr_pt();
}

ISR(ADC_vect)
{
//	task();
	isr_pt_array[ADC_vect_num].isr_pt();
}

ISR(PCINT0_vect)
{
	isr_pt_array[PCINT0_vect_num].isr_pt();
}

ISR(PCINT1_vect)
{
	isr_pt_array[PCINT1_vect_num].isr_pt();
}

ISR(PCINT2_vect)
{
	isr_pt_array[PCINT2_vect_num].isr_pt();
}

ISR(SPI_STC_vect)
{
	PORTD |= (1 << 5);
	isr_pt_array[SPI_STC_vect_num].isr_pt();
	PORTD &= ~(1 << 5);	
}

ISR(INT0_vect)
{
	isr_pt_array[INT0_vect_num].isr_pt();
}

ISR(INT1_vect)
{
	isr_pt_array[INT1_vect_num].isr_pt();
}


ISR(WDT_vect)
{
	isr_pt_array[WDT_vect_num].isr_pt();
}

ISR(TIMER2_COMPA_vect)
{
//	task();
	isr_pt_array[TIMER2_COMPA_vect_num].isr_pt();
}

ISR(TIMER2_COMPB_vect)
{
	isr_pt_array[TIMER2_COMPB_vect_num].isr_pt();
}

ISR(TIMER2_OVF_vect)
{
	isr_pt_array[TIMER2_OVF_vect_num].isr_pt();
}

ISR(TIMER1_CAPT_vect)
{
	isr_pt_array[TIMER1_CAPT_vect_num].isr_pt();
}

ISR(TIMER1_OVF_vect)
{
	isr_pt_array[TIMER1_OVF_vect_num].isr_pt();
}

ISR(TIMER0_COMPA_vect)
{
	isr_pt_array[TIMER0_COMPA_vect_num].isr_pt();
}

ISR(TIMER0_COMPB_vect)
{
	isr_pt_array[TIMER0_COMPB_vect_num].isr_pt();
}

//ISR(TIMER0_OVF_vect)
//{
	//isr_pt_array[TIMER0_OVF_vect_num].isr_pt();
//}

ISR(EE_READY_vect)
{
	isr_pt_array[EE_READY_vect_num].isr_pt();
}

ISR(ANALOG_COMP_vect)
{
	isr_pt_array[ANALOG_COMP_vect_num].isr_pt();
}

ISR(TWI_vect)
{
	isr_pt_array[TWI_vect_num].isr_pt();
}

ISR(SPM_READY_vect)
{
	isr_pt_array[SPM_READY_vect_num].isr_pt();
}