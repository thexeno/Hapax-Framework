#include <string.h>
#include "serial.h"
#include "conf.h"

#define SERIAL_BUFF_SZ_MASK (0x0f)


static const serial_conf_t* serial_conf_buff;
static ser_status_t err_type = SER_INIT_PEND;

typedef struct
{
    char buffer[SERIAL_BUFF_SZ_MASK];
    uint8_t head;
    uint8_t tail;
    bool full;
} ser_rb_t;

static ser_rb_t tx[SERIAL_TOTAL_INSTANCES];
static ser_rb_t rx[SERIAL_TOTAL_INSTANCES];

static char console_cb_put_word[SERIAL_TOTAL_INSTANCES](console_generic_t channel, char data);
static char console_cb_get_word[SERIAL_TOTAL_INSTANCES](console_generic_t channel);

/********************************************************
 
 Public function for initialization and simultaneous TX/RX handling

 *********************************************************/

/**
 * @brief      Initialize the serial library
 */
ser_status_t Serial_init(const serial_conf_t* console)
{
    int i;
    serial_conf_buff = console;
    for (i = 0; i < SERIAL_TOTAL_INSTANCES; i++)
    {
       tx[i].full = 0;
       tx[i].head = 0;
       tx[i].tail = 0;
       
       rx[i].full = 0;
       rx[i].head = 0;
       rx[i].tail = 0;


    }
	err_type = SER_OK;
	return (err_type);
}




/**********************************************************
 
 Public functions for TX handling only

 ***********************************************************/


ser_status_t Serial_get_status(conf_serial_e channel)
{
	return (err_type);
}


/**
 * @brief      Trigger the transmission if ISR are used. If riong buffer is empty, do nothing.
 */
ser_status_t Serial_trig(conf_serial_e channel)
{
    uint8_t i = 0;
    for (i = 0; i < SERIAL_TOTAL_INSTANCES; i++)
    {
        if ((serial_conf_buff+i)->serial == channel)
        {
            Serial_TX_handler((serial_conf_buff+i)->source);
            break;
        }
    }
	return SER_OK;
}
 


/**
 * @brief      Add a whole string  to the TX ring buffer.
 *
 * @param[in]  str   The string pointer
 */
void Ser_put_buff(conf_serial_e channel, const char* str)
{
	uint8_t lim = 0, i = 0, max = 0;
    for (i = 0; i < SERIAL_TOTAL_INSTANCES; i++)
    {
        if ((serial_conf_buff+i)->serial == channel)
        {
            if (tx[(serial_conf_buff+i)->serial].full != 0)
            {
                max = 0;  // avoid this line, not sure if is useless. test
            }
            else
            {
                max = SERIAL_BUFF_SZ_MASK - (tx[(serial_conf_buff+i)->serial].head - tx[(serial_conf_buff+i)->serial].tail);
            }
	        lim = strlen(str);
	        if (lim <= max)
	        {
	            for(i=0; i<lim; i++)
	            {
	                Ser_put_char(channel, str[i]);
	            }
	        }
            break;  // for (i = 0; i < SERIAL_TOTAL_INSTANCES; i++)
        }
    }
}

void Ser_put_char(conf_serial_e channel, char ch)
{
    int i = 0;
	for (i = 0; i < SERIAL_TOTAL_INSTANCES; i++)
    {
        if ((serial_conf_buff+i)->serial == channel)
        {
            if ((tx[(serial_conf_buff+i)->serial].head == tx[(serial_conf_buff+i)->serial].tail) && (tx[(serial_conf_buff+i)->serial].full != 0)) // if full
            {
                // overflow
            }
            else
            {
                tx[(serial_conf_buff+i)->serial].buffer[tx[(serial_conf_buff+i)->serial].head] = ch;
                tx[(serial_conf_buff+i)->serial].head = (tx[(serial_conf_buff+i)->serial].head + 1) & SERIAL_BUFF_SZ_MASK;
            }
            break;
        }
    }
}

/**
 * @brief      Add a 16bit integer to the TX ring buffer
 *
 * @param[in]  c     integer
 */
void Ser_put_integer(conf_serial_e channel, char* format, uint16_t n)
{
	int lim = 0, i = 0, max = 0;
	char str[6];
    for (i = 0; i < SERIAL_TOTAL_INSTANCES; i++)
    {
        if ((serial_conf_buff+i)->serial == channel)
        {
            if (tx[(serial_conf_buff+i)->serial].full != 0)
            {
                max = 0;  // avoid this line, not sure if is useless. test
            }
            else
            {
                max = SERIAL_BUFF_SZ_MASK - (tx[(serial_conf_buff+i)->serial].head - tx[(serial_conf_buff+i)->serial].tail);
            }
            sprintf((char*)str, (char*)format, n);
            lim = strlen(str);
            if (lim <= max)
            {
                for(i=0; i<lim; i++)
                {
                    Ser_put_char(channel, str[i]);
                }
            }
            break;
        }
    }
}

uint8_t Serial_get_RX_size(conf_serial_e channel)
{
    int i = 0;

	for ( i = 0; i < SERIAL_TOTAL_INSTANCES; i++)
    {
        if ((serial_conf_buff+i)->serial == channel)
        {
            return (SERIAL_BUFF_SZ_MASK - (rx[(serial_conf_buff+i)->serial].head - rx[(serial_conf_buff+i)->serial].tail));
        }
    }
    return -1;
}

uint8_t Serial_get_TX_size(conf_serial_e channel)
{
    int i = 0;

	for ( i = 0; i < SERIAL_TOTAL_INSTANCES; i++)
    {
        if ((serial_conf_buff+i)->serial == channel)
        {
            return (SERIAL_BUFF_SZ_MASK - (tx[(serial_conf_buff+i)->serial].head - tx[(serial_conf_buff+i)->serial].tail));
        }
    }
    return -1;
}

/**
 * @brief      Get the oldest char in the ring buffer and remove from it
 *
 * @return     Oldest char in the RX ring buffer
 */

uint8_t Serial_get_char(conf_serial_e channel)
{
    int i = 0;

	for ( i = 0; i < SERIAL_TOTAL_INSTANCES; i++)
    {
        if ((serial_conf_buff+i)->serial == channel)
        {
	        if ((rx[(serial_conf_buff+i)->serial].head == rx[(serial_conf_buff+i)->serial].tail) && (rx[(serial_conf_buff+i)->serial].full == 0))   // if empty
            {
                // underflow
            }
            else
            {
                rx[(serial_conf_buff+i)->serial].tail = (rx[(serial_conf_buff+i)->serial].tail + 1) & SERIAL_BUFF_SZ_MASK;
                return rx[(serial_conf_buff+i)->serial].buffer[rx[(serial_conf_buff+i)->serial].head];
            }
            break; // only one channel can be called per time
        }
    }
    return -1;
}


/***************************************************************

Static functions and ISR

 ***************************************************************/


void Serial_RX_handler(serial_generic_t channel)
{
    int i = 0;

	for ( i = 0; i < SERIAL_TOTAL_INSTANCES; i++)
    {
        if ((serial_conf_buff+i)->source == channel)
        {
            if ((rx[(serial_conf_buff+i)->serial].head == rx[(serial_conf_buff+i)->serial].tail) && (rx[(serial_conf_buff+i)->serial].full != 0)) // if full
            {
                (void)console_cb_get_word(channel);
                // overflow
            }
            else
            {
                rx[(serial_conf_buff+i)->serial].buffer[rx[(serial_conf_buff+i)->serial].head] = console_cb_get_word(channel);
                rx[(serial_conf_buff+i)->serial].head = (rx[(serial_conf_buff+i)->serial].head + 1) & SERIAL_BUFF_SZ_MASK;
            }
            break;
        }
    }
}

void Serial_TX_handler(serial_generic_t channel)
{
    int i = 0;

	for ( i = 0; i < SERIAL_TOTAL_INSTANCES; i++)
    {
        if ((serial_conf_buff+i)->source == channel)
        {
            if ((tx[(serial_conf_buff+i)->serial].head == tx[(serial_conf_buff+i)->serial].tail) && (tx[(serial_conf_buff+i)->serial].full == 0))   // if empty
            {
                // underflow
            }
            else
            {
                console_cb_put_word(channel, tx[(serial_conf_buff+i)->serial].buffer[tx[(serial_conf_buff+i)->serial].tail]);
                tx[(serial_conf_buff+i)->serial].tail = (tx[(serial_conf_buff+i)->serial].tail + 1) & SERIAL_BUFF_SZ_MASK;
            }
            break;
        }
    }
}



