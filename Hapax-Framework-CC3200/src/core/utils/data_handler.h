#ifndef DATA_HANDLER_H_
#define DATA_HANDLER_H_

#include "sys_types.h"

void dh_writeTo16bit(uint16_t input_n, volatile uint8_t* out_h, volatile uint8_t* out_l);
void dh_readFrom16bit(volatile uint16_t *out_n, uint8_t in_h, uint8_t in_l);
uint8_t dh_char2hex(unsigned char msb, unsigned char lsb);


#endif
