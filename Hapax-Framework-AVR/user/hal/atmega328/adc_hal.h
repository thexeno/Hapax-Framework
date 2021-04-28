

#include "../app_conf.h"

//#define adcSize_t (adcData_t)
typedef uint16_t adcData_t;

adcData_t adcHal_Read(void);
uint8_t adcHal_InitRead(uint8_t channel);
void adcHal_ResetInput(uint8_t channel);

