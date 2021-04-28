#include "hal.h"
#include "kernel/core_interrupt.h"

extern struct isr_array isr_pt_array[HAL_SUPPORTED_ISR];

#if HAL_CONF_USE_INTERRUPT_VECTOR_SCHEME


#else

ISR(TIMER1_COMPA_vect)
{
//	task();
	isr_pt_array[HAL_VECT_TIMER1_ID].isr_pt();
}

ISR(USART_RX_vect)
{
//	task();
	isr_pt_array[HAL_VECT_UART_RX_ID].isr_pt();
}

ISR(USART_UDRE_vect)
{
//	task();
	isr_pt_array[HAL_VECT_UART_TX_BUFF_EMPTY_ID].isr_pt();
}

ISR(USART_TX_vect)
{
//	task();
	isr_pt_array[HAL_VECT_UART_TX_DONE_ID].isr_pt();
}

ISR(ADC_vect)
{
//	task();
	isr_pt_array[HAL_VECT_ADC_ID].isr_pt();
}

ISR(PCINT0_vect)
{
	isr_pt_array[HAL_VECT_INT0_ID].isr_pt();
}

ISR(PCINT1_vect)
{
	isr_pt_array[HAL_VECT_INT1_ID].isr_pt();
}

ISR(PCINT2_vect)
{
	isr_pt_array[HAL_VECT_INT2_ID].isr_pt();
}

ISR(SPI_STC_vect)
{
	isr_pt_array[HAL_VECT_SPI_ID].isr_pt();
}

/*#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void)
{
	isr_pt_array[HAL_CONF_INT_TIMER0_ID][0]();
}*/


// MSP430 structure
// #pragma vector = TIMER0_A0_VECTOR
// __interrupt void TIMER0_A0_ISR (void)
// {
// }

#endif
