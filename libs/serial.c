#include "serial.h"
#include "hal/atmega328/uart_hal.h"

#warning "do the DMX version che si basa su UART"

#define SER_NUM_CHANNELS 2

// into header: NO!
typedef struct ser_dev_buffered
{
	ser_dev_st* ser_dev_pt;
} ser_dev_buffered_t;



uint8_t channels_to_serial[SER_NUM_CHANNELS] = {
	UART_0
};


ser_dev_buffered_t ser_dev_internal_pt[SER_NUM_CHANNELS];


static void ser_rx_byte_handler(uart_hal_ch_t channel);
//static void ser_tx_done_ISR(void);
static void ser_tx_byte_handler(uart_hal_ch_t channel);

/********************************************************
 
 Public function for initialization and simultaneous TX/RX handling

 *********************************************************/

/**
 * @brief      Initialize the serial library
 */
int8_t Ser_init(ser_dev_st* console, const uart_hal_cgf_t* config, uint8_t* txb, base_t tx_sz, uint8_t* rxb, base_t rx_sz)
{

	console->config = config;
	// With this code the ISR handler seen from the application can have a parameter.
	// Given the source of the ISR, the parameter will change and will be used as index
	// for the ser_dev_internal_pt, which will use the ser_dev_st data without the application calling it (is buffered internally in serial.c)
	ser_dev_internal_pt[channels_to_serial[config->channel]].ser_dev_pt = console;

	Rb_init(&console->serial_tx_buff, txb, (rb_sz_t)tx_sz);
	Rb_init(&console->serial_rx_buff, rxb, (rb_sz_t)rx_sz);

	UartHal_ISR_RX_callback_set(ser_rx_byte_handler, console->config->channel);
	UartHal_ISR_TX_callback_set(ser_tx_byte_handler, console->config->channel);

	UartHal_init(console->config);

	return 0;
}




/**********************************************************
 
 Public functions for TX handling only

 ***********************************************************/





/**
 * @brief      Trigger the transmission if ISR are used. If riong buffer is empty, do nothing.
 */
void Ser_trig_tx(ser_dev_st* console)
{
	if (!Rb_is_empty(&console->serial_tx_buff))
		UartHal_put_byte(console->config, Rb_retrieve_char(&console->serial_tx_buff));
}
 


/**
 * @brief      Add a whole string  to the TX ring buffer.
 *
 * @param[in]  str   The string pointer
 */
void Ser_put_buff(ser_dev_st* console, const char* str)
{
	Rb_put_buff(str, &console->serial_tx_buff);
}

void Ser_put_char(ser_dev_st* console, char ch)
{
	Rb_put_char(&console->serial_tx_buff, ch);
}

/**
 * @brief      Add a 16bit integer to the TX ring buffer
 *
 * @param[in]  c     integer
 */
void Ser_put_integer(ser_dev_st* console, uint16_t c)
{
	Rb_put_integer("%d", c, &console->serial_tx_buff);
}

/**
 * @brief      The available space in the RX ring buffer
 *
 * @return     Size of free space.
 */
uint8_t Ser_tx_buffer_space_av(ser_dev_st* console)
{
	uint16_t available_size = 0;
	available_size = SER_RB_SIZE - (uint16_t)Rb_used_size(&console->serial_tx_buff);
	return available_size;
}


// uint8_t Ser_tx_busy_check(void)
// {
// 	return ser.tx_busy;
// }

/**********************************************************
 
 Public functions for RX handling only

 ***********************************************************/

/**
 * @brief      The available space in the RX ring buffer
 *
 * @return     Size of free space
 */
uint8_t Ser_rx_buffer_space_av(ser_dev_st* console)
{
	uint16_t available_size = 0;
	available_size = SER_RB_SIZE - (uint16_t)Rb_used_size(&console->serial_rx_buff);
	return available_size;
}

/**
 * @brief      The elements in the RX ring buffer
 *
 * @return     Number of elements
 */
uint8_t Ser_rx_buffer_elem(ser_dev_st* console)
{
    uint16_t elem = 0;
    elem = (uint16_t)Rb_used_size(&console->serial_rx_buff);
    return elem;
}

/**
 * @brief      The elements in the TX ring buffer
 *
 * @return     Number of elements
 */
uint8_t Ser_tx_buffer_elem(ser_dev_st* console)
{
    uint16_t elem = 0;
    elem = (uint16_t)Rb_used_size(&console->serial_tx_buff);
    return elem;
}


/**
 * @brief      Get the oldest char in the ring buffer and remove from it
 *
 * @return     Oldest char in the RX ring buffer
 */

uint8_t Ser_get_char(ser_dev_st* console)
{
	uint8_t ret = 0;
	ret = Rb_retrieve_char(&console->serial_rx_buff);
	return ret;
}


/***************************************************************

Static functions and ISR

 ***************************************************************/


static void ser_rx_byte_handler(uart_hal_ch_t channel)
{
	ser_dev_st* console = ser_dev_internal_pt[channels_to_serial[channel]].ser_dev_pt; // buffered for debug
	if (!Rb_is_full(&console->serial_rx_buff))
	{
		Rb_put_char(&console->serial_rx_buff, UartHal_get_byte(console->config));		
	}
	else
	{
		/* Discard the byte */
		(void)UartHal_get_byte(console->config);
		#warning "todo: XOFF/XON"
	}
}



static void ser_tx_byte_handler(uart_hal_ch_t channel)
{
	#warning "todo: XOFF/XON"
	const char dummy = 0x00;
	ser_dev_st* console = ser_dev_internal_pt[channels_to_serial[channel]].ser_dev_pt;
	/* put data in tx buffer */
	if (!Rb_is_empty(&console->serial_tx_buff) && (Rb_used_size(&console->serial_tx_buff)>=1))
	{
		UartHal_put_byte(console->config, Rb_retrieve_char(&console->serial_tx_buff));
	}
	//else if (Rb_used_size(&console->serial_tx_buff) <= 1)
	//{
		//Last data
		//UartHal_put_byte(console->config, Rb_retrieve_char(&console->serial_tx_buff));
	//}
	else
	{
		// to stop interrupt on certain patforms. like Atmega. Check compatiblity
		//UartHal_put_byte(console->config, dummy);
	}
}



