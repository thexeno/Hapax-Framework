#include <wrappers/hal.h>
#include <wrappers/interrupt_hal.h>

#define INTERRUPT_MAX_SUPPORTED_ISR 26U
typedef void (*isr_pt_t)(void);


typedef enum {
	INTERRUPT_ASSIGNED,
	INTERRUPT_UNASSIGNED,
	INTERRUPT_DEFAULT
} int_hal_state_t;




struct isr_array
{
	isr_pt_t isr_pt;
	int_hal_state_t state;
};

struct isr_array isr_pt_array[INTERRUPT_MAX_SUPPORTED_ISR];

static void IntHal_default_ISR(void)
{
	asm("nop");
	while(1);
}


base_t IntHal_suspend_global_interrupt(void)
{
	base_t prim;
    
    prim = __get_PRIMASK();
    
    /* Disable interrupts */
    __disable_irq();

    return prim;
}

void IntHal_restore_global_interrupt(base_t status)
{
	if (!status) {
        __enable_irq();
    }
}

void IntHal_enable_global_interrupt(void)
{
	__enable_irq();
}

void IntHal_vector_init(void)
{
	uint8_t temp = 0;
	base_t status = 0;
	status = IntHal_suspend_global_interrupt();
	for (temp=0; temp<INTERRUPT_MAX_SUPPORTED_ISR; temp++)
	{
		isr_pt_array[temp].isr_pt = &IntHal_default_ISR;
		isr_pt_array[temp].state = INTERRUPT_DEFAULT;	
	}
	//IntHal_restore_global_interrupt(status);
}


void IntHal_vector_register(void (*f_pt)(void), int_hal_vect_num_t irq_i)
{
	// unsigned long *ulNvicTbl;
	// ulNvicTbl = (ivTable_t*)CONF_REG(HAL_CONF_VTABLE); // check alignment: type of pointer. Try with CC3200
	// ulNvicTbl[id] = (unsigned long)f_pt;
	if (isr_pt_array[irq_i].state != INTERRUPT_ASSIGNED)
	{
		isr_pt_array[irq_i].isr_pt = f_pt;
		//isr_pt_array[irq_i].iv_id  = 
		isr_pt_array[irq_i].state = INTERRUPT_ASSIGNED;           // otherwise marked as unassigned and properly handled
	}
}

void IntHal_vector_unregister(int_hal_vect_num_t irq_i)
{
	isr_pt_array[irq_i].isr_pt = &IntHal_default_ISR;
	isr_pt_array[irq_i].state = INTERRUPT_UNASSIGNED;         
}


void I2C1_EV_IRQHandler(void)
{
  isr_pt_array[I2C1_EV_IRQHandler_num].isr_pt();
}


/**
  * @brief This function handles I2C1 error interrupt.
  */
void I2C1_ER_IRQHandler(void)
{ 
  isr_pt_array[I2C1_ER_IRQHandler_num].isr_pt();
}

void TIM4_IRQHandler(void)
{
  isr_pt_array[TIM4_IRQHandler_num].isr_pt();
}

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
 // HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}
