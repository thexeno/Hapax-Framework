#include "data_handler.h"


	// cpu_dw_t st;
	// st = IntHal_suspend_global_interrupt();//cli();
	// *out_h = (uint8_t)(input_n >> 8);
	// *out_l = (uint8_t)(input_n);
	// IntHal_restore_global_interrupt(st);//sei();

/**
 * @brief      When critical, use 
 *
 * @param[in]  input_n  The input n
 * @param      out_h    The out h
 * @param      out_l    The out l
 */
void dh_writeTo16bit(uint16_t input_n, volatile uint8_t* out_h, volatile uint8_t* out_l)
{
	//cpu_dw_t st;
	//st = IntHal_suspend_global_interrupt();//cli();
	*out_h = (uint8_t)(input_n >> 8);
	*out_l = (uint8_t)(input_n);
	//IntHal_restore_global_interrupt(st);//sei();
}

void dh_readFrom16bit(volatile uint16_t *out_n, uint8_t in_h, uint8_t in_l)
{
	//cpu_dw_t st;
	//st = IntHal_suspend_global_interrupt();
	*out_n = (uint16_t)in_l;
	*out_n |= ((uint16_t)(in_h))<<8;
	//IntHal_restore_global_interrupt(st);
}


uint8_t dh_Char2hex(unsigned char msb, unsigned char lsb)
{
	if ((msb < 'a' && msb > '9') || msb < '0' || msb > 'f' ||
	(lsb < 'a' && lsb > '9') || lsb < '0' || lsb > 'f')
	{
		// printf("Not HEX value. msb: %c, lsb: %c\n", msb, lsb);
		return 0;
	}
	if (msb >= 'a' && msb <= 'f')
	{
		msb = msb - 'a' + 10;
	} 
	else
	{
		msb = msb - '0';
	}
	
	if (lsb >= 'a' && lsb <= 'f')
	{
		lsb = lsb - 'a' + 10;
	} 
	else 
	{
		lsb = lsb - '0';
	}
	return (lsb+(msb*16));
}

