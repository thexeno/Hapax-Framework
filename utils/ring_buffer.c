#include "ring_buffer.h"

/******************************************************

Control interfaces of RB

********************************************************/

void Rb_init(rb_t *_this, uint8_t* buffer, rb_sz_t size)
{
	_this->buff = buffer;
	_this->count  = 0;
	_this->head   = 0;
	_this->tail   = 0;
	_this->size	= size;
	_this->full  = false;
}



bool  Rb_is_empty (rb_t *_this)
{

    if ((_this->head == _this->tail) && !_this->full)
	{
		return true;
	}
	else
	{
		return false;
	}
}

uint8_t  Rb_used_size (rb_t *_this)
{
	uint8_t ret = 0;
	if (_this->full)
	{
	    ret = _this->size;  // avoid this line, not sure if is useless. test
	}
	else
	{
	    ret = (*_this).head - (*_this).tail;
	}

	return ret;
}


bool  Rb_is_full (rb_t *_this)
{
    /*
	if (_this->head == (_this->tail-1))
	{
		return 1;
	}
	else
	{
		return 0;
	}*/
	return _this->full;
}



/******************************************************

Buffer read interfaces

********************************************************/

char Rb_retrieve_char(rb_t *_this)
{
	char ch = 0;
	if (!Rb_is_empty(_this))
	{
		ch = _this->buff[_this->tail];
		_this->full = false;
		_this->tail = (_this->tail + 1) % _this->size;
		return ch;
	}
	else
	{
		return 0;
	}
}



/******************************************************

Buffer write interfaces

********************************************************/

int8_t Rb_put_char(rb_t *_this, char c)
{
	//if (_this->count < RBUF_SIZE)
	if (!_this->full) // controlla per scritto i casi
	{
		_this->buff[_this->head] = c;
		_this->head = (_this->head + 1) % _this->size;
		_this->full = (_this->head == _this->tail);
		//_this->count++;
	}
	else
	{
		return -1;
	}

	return 0;
}




int8_t Rb_put_buff(const char* s, rb_t* buff)
{
	uint8_t lim = 0, i = 0;
	lim = strlen(s);
	for(i=0; i<lim; i++)
	{
		if (Rb_put_char(buff, s[i]) == -1)
		{
			return -1;
		}
	}
	return 0;
}

int8_t Rb_put_integer(char* s, uint16_t n, rb_t* rb)
{
	uint8_t lim = 0, i = 0, buff[8];
	sprintf((char*)buff, (char*)s, n);
	lim = strlen((char*)buff);
	for(i=0; i<lim; i++)
	{
		if (Rb_put_char((rb_t*)rb, buff[i]) == -1)
		{
			return -1;
		}
	}
	return 0;
}

