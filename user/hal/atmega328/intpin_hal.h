#ifndef ENCODERS_HAL_H_
#define ENCODERS_HAL_H_

#include "../app_conf.h"

//uint8_t encHal_rdPin(uint8_t );
//void encHal_mdPin(uint8_t sel, uint8_t mode);
//void encHal_wrPin(uint8_t sel, uint8_t val);
//void encHal_interruptPin(uint8_t sel);
void intPin_PinToISR(uint8_t pin);
void intPin_setEdge(uint8_t edge, uint8_t ext);
#endif