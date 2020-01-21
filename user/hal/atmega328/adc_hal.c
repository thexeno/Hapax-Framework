#include "adc_hal.h"
#include "../app_conf.h"

// adcData_t adcHal_Read(void)
// {
// 	return (adcData_t)adc_GetResult();
// }



/**
 * @brief      ADC start with previous defined settings.
 *             STATIC function for the ADC hal.
 *
 * @return     Zero if start was successful.
 */
static uint8_t _adc_Start(void)
{
	uint8_t ret = 0;
	if ((ADCSRA & (1 << ADSC))==0)
	{
		ADCSRA |= (1 << ADSC);	
	}
	else
	{
		ret = 1;
	}
	return ret;
}

/**
 * @brief      Initialize the ADC. STATIC function for the ADC hal, is called inside adc_StartRead normally.
 *             Does not trigger a read.
 *
 * @param[in]  channel  The channel of the ADC to be initialized
 * @param[in]  presc    The prescaler of the ADC clock
 * @param[in]  enInt    Select the usage of the inetrrupt
 */
static void _adc_InitRead(uint8_t channel, uint8_t presc, uint8_t enInt)
{
	ADCSRA = 0;
	switch (presc)
	{
		case 2:
		ADCSRA = 1;
		break;
		
		case 4:
		ADCSRA = 2;
		break;
		
		case 8:
		ADCSRA = 3;
		break;
		
		case 16:
		ADCSRA = 4;
		break;
		
		case 32:
		ADCSRA = 5;
		break;
	
		case 64:
		ADCSRA = 6;
		break;
		
		case 128:
		ADCSRA = 7;
		break;
		
		default:
		ADCSRA = 0; // Uguale a 2
		break;
	}
	
	if (enInt == ADC_INTON)
	{
		ADCSRA |= (1<<ADIE);
	}
	else 
	{
		ADCSRA &= ~(1<<ADIE);
	}
	ADCSRA &= ~(1<<ADATE); /* No auto update */
	ADCSRA |= (1<<ADEN);
	ADCSRB = 0; /* no source needed */
	ADMUX = 0;
	if (channel < 6)
	{
		DIDR0 |= (1<<channel);
	}
	ADMUX |= (1<<REFS0) | (channel & 0x0F); /* AVcc ref */
	//ADMUX &= ~(1<<ADLAR); // right adj- leggi L poi H
	ADMUX |= (1<<ADLAR);
	
	//adc.channel = channel;
}


/**
 * @brief      Setup ADC to read a channel, configure the hardware with the speed and interrupt settings 
 *             given in the configuraton.
 *             Trigger the start of the ADC.
 *
 * @param[in]  channel  The channel selected of the ADC.
 * @param[in]  poll_mode  When ZERO the ISR
 * 
 * @return     Zero if start was successful.
 */

uint8_t adc_StartRead(uint8_t channel, uint8_t poll_mode)
{
	// put some adapter for the speed
	_adc_EnableAnalogPin(channel);
	if (!poll_mode)
		_adc_InitRead(channel, ADC_SPEED, ADC_INTON);
	else
		_adc_InitRead(channel, ADC_SPEED, ADC_INTOFF);
	return _adc_Start();
}


/**
 * @brief      Set the pin used for the ADC as digital, so that either become available for digital usage
 *             or to save power, if compatible with the circuitry.
 *             
 *             TODO: set digital and also as input.
 *
 *
 * @param[in]  channel  The channel of the ADC
 */
void adc_EnableDigitalPin(uint8_t channel)
{
	DIDR0 &= ~(1<<channel);
}

void adc_EnableAnalogPin(uint8_t channel)
{
	DIDR0 |= (1<<channel);
}

/**
 * @brief      Get the last conversion result. This can either be polled or inserted in an ISR
 *
 * @return     { description_of_the_return_value }
 */
adcData_t adc_GetResult(void)
{
	adcData_t val;
	val = ADCL>>6;
	val |= (ADCH<<2);
	return val;
}




// void _adc_EnableDigitalPin(uint8_t channel)
// {
// 	DIDR0 &= ~(1<<channel);
// }




