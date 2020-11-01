#include "hal.h"

extern void (* const g_pfnVectors[])(void);

void Core_hal_init(void)
{
        MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);

        MAP_IntMasterEnable();
        MAP_IntEnable(FAULT_SYSTICK);

        PRCMCC3200MCUInit();

}
