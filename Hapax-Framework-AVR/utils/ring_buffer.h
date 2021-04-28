#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include "sys_types.h"



typedef uint8_t rb_sz_t;


typedef struct ring_buff
{
	uint8_t* buff;//uint8_t buff[RB_SIZE];
	uint8_t head;
	uint8_t tail;
	uint8_t count;
	rb_sz_t size;
	bool full;
} rb_t;


void Rb_init(rb_t *_this, uint8_t* buffer, rb_sz_t sz);
bool  Rb_is_empty (rb_t *_this);
rb_sz_t  Rb_used_size (rb_t *_this);
bool  Rb_is_full (rb_t *_this);
char Rb_retrieve_char(rb_t *_this);
int8_t Rb_put_char(rb_t *_this, char c);
int8_t Rb_put_buff(const char* s, rb_t* buff);
int8_t Rb_put_integer(char* s, uint16_t n, rb_t* rb);
rb_sz_t  Rb_available_size (rb_t *_this);



#endif






