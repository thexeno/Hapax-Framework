#include "uart_hal.h"
#include "interrupt_hal.h"



typedef void (*uart_hal_isr_cb)(uart_hal_ch_t );
uart_hal_isr_cb uartHal_ISR_RX_cb[NUM_UART_CHANNELS];
uart_hal_isr_cb uartHal_ISR_TX_cb[NUM_UART_CHANNELS];

//static struct uartHal_ISR_RX_cb_array uartHal_ISR_RX_cb[NUM_UART_CHANNELS];


#warning "inverti con volatile"
register_t volatile * const ucsra[NUM_UART_CHANNELS] =
{
	//(*(volatile uint8_t *)(mem_addr))
	(register_t*)&UCSR0A
	//(address_t*)&UCSR1A
	//(*(volatile uint8_t *)(mem_addr))
};
register_t volatile * const ucsrb[NUM_UART_CHANNELS] =
{
	(register_t*)&UCSR0B
//	(address_t*)&UCSR1B
};

register_t volatile * const ucsrc[NUM_UART_CHANNELS] =
{
	(register_t*)&UCSR0C,
//	(address_t*)&UCSR1C
};

//address_t volatile * const ubrrl[NUM_UART_CHANNELS] =
register_t volatile * const ubrrl[NUM_UART_CHANNELS] =
{
	(register_t*)&UBRR0L,
//	(address_t*)&UBRR1L
};

register_t volatile * const ubrrh[NUM_UART_CHANNELS] =
{
	(register_t*)&UBRR0H,
//	(address_t*)&UBRR1H
};

register_t volatile * const udr[NUM_UART_CHANNELS] =
{
	(register_t*)&UDR0,
//	(address_t*)&UDR1
};





#define UART_HAL_ASYNC 0
#define UART_HAL_NO_PARITY 0
#define UART_HAL_STOP_BIT1 0
#define UART_HAL_DATA_8BIT 0x06

typedef enum {

	UART_HAL_INT_TX_BUFF_EMPTY,
	UART_HAL_INT_TX_DONE,
	UART_HAL_INT_RX_BUFF_FULL

} uart_hal_int_t;

// uart_struct_container_fp contiene funzioni 
// che accettano parametri tipo uart_struct_container_parameter




static void uart0_rx_isr()
{
	// metti cb
	uartHal_ISR_RX_cb[UART_0](UART_0); // callback a i.e. ser_rx_char_ISR(ser_dev_st* console)
	
}


static void uart0_tx_isr()
{
	// metti cb
	uartHal_ISR_TX_cb[UART_0](UART_0); // callback a i.e. ser_rx_char_ISR(ser_dev_st* console)
	
}

static void uart0_tx_done_isr()
{
	asm("nop");
}

static void uart1_rx_isr()
{
	// metti cb
//	isr_rx_cb[UART_1](UART_1); // callback a i.e. ser_rx_char_ISR(ser_dev_st* console)	
}

static void uart1_tx_isr()
{
	// metti cb
	//uartHal_ISR_TX_cb[UART_1](UART_1); // callback a i.e. ser_rx_char_ISR(ser_dev_st* console)
	
}



void UartHal_ISR_RX_callback_set(void (*fp_t)(uart_hal_ch_t ), uart_hal_ch_t channel)
{
	uartHal_ISR_RX_cb[channel] = fp_t; 
}

void UartHal_ISR_TX_callback_set(void (*fp_t)(uart_hal_ch_t ), uart_hal_ch_t channel)
{
	uartHal_ISR_TX_cb[channel] = fp_t; 
}

/*********************************************************
 
 Private functions
  
 ********************************************************/


static void uart_interrupt_enable(uart_hal_ch_t ch, uart_hal_int_t mode)
{
	switch (mode)
	{
		case UART_HAL_INT_TX_BUFF_EMPTY:
			*ucsrb[ch] |= (1<<UDRIE0);
		break;

		case UART_HAL_INT_TX_DONE:
			*ucsrb[ch] |= (1<<TXCIE0);
		break;

		case UART_HAL_INT_RX_BUFF_FULL:
			*ucsrb[ch] |= (1<<RXCIE0);
		break;

		default:
		break;
	}
}

static void uart_interrupt_disable(uart_hal_ch_t ch, uart_hal_int_t mode)
{
	switch (mode)
	{
		case UART_HAL_INT_TX_BUFF_EMPTY:
			*ucsrb[ch] &= ~(1<<UDRIE0);
		break;

		case UART_HAL_INT_TX_DONE:
			*ucsrb[ch] &= ~(1<<TXCIE0);
		break;

		case UART_HAL_INT_RX_BUFF_FULL:
			*ucsrb[ch] &= ~(1<<RXCIE0);
		break;

		default:
		break;
	}
}


/*********************************************************
 
 Public functions
  
 ********************************************************/




uint8_t UartHal_get_byte(const uart_hal_cfg_t* handle)
{
	return *udr[handle->channel];
}

void UartHal_put_byte(const uart_hal_cfg_t* handle, uint8_t d)
{
	*udr[handle->channel] = d;
}

void UartHal_init(const uart_hal_cfg_t* handle)
{
	uint16_t baud_reg = 0;

	*ucsra[handle->channel] = 0;
	*ucsrb[handle->channel] = 0;
	*ucsrc[handle->channel] = 0;

	baud_reg = (uint16_t)(handle->sys_clock/(16 * handle->baudrate))-1;

	*ubrrl[handle->channel] = (uint8_t)baud_reg;
	*ubrrh[handle->channel] = (uint8_t)(baud_reg >> 8);

	switch (handle->mode) {
		case UART_HALF_DUPLEX_TX:
		*ucsrb[handle->channel] |= (1<< TXEN0);
		uart_interrupt_disable(handle->channel, UART_HAL_INT_RX_BUFF_FULL);
		uart_interrupt_enable(handle->channel, UART_HAL_INT_TX_DONE);
		//uart_interrupt_enable(handle->channel, UART_HAL_INT_TX_BUFF_EMPTY);
		break;
		
		case UART_HALF_DUPLEX_RX:
		*ucsrb[handle->channel] |= (1<< RXEN0);
		uart_interrupt_enable(handle->channel, UART_HAL_INT_RX_BUFF_FULL);
		uart_interrupt_disable(handle->channel, UART_HAL_INT_TX_DONE);
		//uart_interrupt_disable(handle->channel, UART_HAL_INT_TX_BUFF_EMPTY);
		break;
		
		case UART_FULL_DUPLEX:
		*ucsrb[handle->channel] |= ((1<< TXEN0) | (1<<RXEN0));
		#warning "why? you should be able to remove it, check"
		*ucsra[handle->channel] |= (1 << TXC0); // Bug fix. Cleared. Otherwise will trigger an initial interrupt. WHY?
		uart_interrupt_enable(handle->channel, UART_HAL_INT_RX_BUFF_FULL);
		uart_interrupt_enable(handle->channel, UART_HAL_INT_TX_DONE);
		//uart_interrupt_enable(handle->channel, UART_HAL_INT_TX_BUFF_EMPTY);
		break;
		
		default:
		break;
	}
	
	uart_interrupt_disable(handle->channel, UART_HAL_INT_TX_BUFF_EMPTY);
	*ucsrc[handle->channel] |= (UART_HAL_ASYNC | UART_HAL_NO_PARITY | UART_HAL_STOP_BIT1 | UART_HAL_DATA_8BIT);	
	//*ucsra[handle->channel] |= (1 << TXC0); // Bug fix. Cleared. Otherwise will trigger an initial interrupt.
	
	
	// CB to the HW interrupt
	IntHal_vector_register(uart0_rx_isr, USART_RX_vect_num);
	IntHal_vector_register(uart0_tx_isr, USART_TX_vect_num);
	//IntHal_vector_register(uart0_tx_isr, HAL_VECT_UART_TX_BUFF_EMPTY_ID);
}
