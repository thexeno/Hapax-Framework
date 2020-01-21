#include "encoders_hal.h"
#include "../app_conf.h"


/**
 * @brief      Activates the given pin ISR functionality. The function prepares the right pin number to 
 *             be sent into the _intPin_confIntPin()
 *
 * @param[in]  pin   The pin
 */
void intPin_PinToISR(uint8_t pin)
{
	uint8_t i = 0;
	if ((pin&0xf0) == PORT_B)
	{
		i = pin & 0x0f;
	}
	else if ((pin&0xf0) == PORT_C)
	{
		i = (pin & 0x0f)+8;
	}
	else if ((pin&0xf0) == PORT_D)
	{
		i = (pin & 0x0f)+16;
	}
	_intPin_confIntPin(i);
}



/**
 * @brief      Connect the phisical pin to its ISR functionality
 *
 * @param[in]  pin   The pin
 */
static void _intPin_confIntPin(uint8_t pin)
{
	if(pin >= 0 && pin < 8)
	{
		PCICR |= (1<<PCIE0);
		PCMSK0 |= (1 << pin);
	}
	else if (pin >= 8 && pin < 15)
	{
		PCICR |= (1<<PCIE1);
		PCMSK1 |= (1 << (pin-8));	
	}
	else if (pin >= 16 && pin < 24)
	{
		PCICR |= (1<<PCIE2);
		PCMSK2 |= (1 << (pin-16));	
	}
	else
	{
		/*error */
	}
}


/**
 * @brief      Set the type of edge in the pin to activate the associated ISR
 *
 * @param[in]  edge  The edge
 * @param[in]  ext   The ISR resource
 */
void intPin_setEdge(uint8_t edge, uint8_t ext)
{
	if (ext == EXT0)
	{
	sbi(EIMSK, INT0);
	switch (edge)
	{
		case LOW_LEVEL:
		cbi(EICRA, ISC00);
		cbi(EICRA, ISC01);
		break;
		
		case ANY_CHANGE:
		sbi(EICRA, ISC00);
		cbi(EICRA, ISC01);
		break;
		
		case RISING:
		sbi(EICRA, ISC00);
		sbi(EICRA, ISC01);
		break;
		
		case FALLING:
		cbi(EICRA, ISC00);
		sbi(EICRA, ISC01);
		break;
		
		default:
		break;
	}		
	}
	else if (ext == EXT1)
	{
		sbi(EIMSK, INT1);
		switch (edge)
		{
			case LOW_LEVEL:
			cbi(EICRA, ISC10);
			cbi(EICRA, ISC11);
			break;
			
			case ANY_CHANGE:
			sbi(EICRA, ISC10);
			cbi(EICRA, ISC11);
			break;
			
			case RISING:
			sbi(EICRA, ISC10);
			sbi(EICRA, ISC11);
			break;
			
			case FALLING:
			cbi(EICRA, ISC10);
			sbi(EICRA, ISC11);
			break;
			
			default:
			break;
		}
	}
	else 
	{
		cbi(EIMSK, INT0);
		cbi(EIMSK, INT1);
	}
	
}