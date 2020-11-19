/*
 * ADC0_driver.c
 *
 *  Created on: Oct 20, 2020
 *      Author: isae_
 */

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Includes Section
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "ADC0_driver.h"
#include "MKL25Z4.h"
#include "derivative.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Functions Section
///////////////////////////////////////////////////////////////////////////////////////////////////

void ADC_Init(ADC_resolution mode)
{
	SIM_SCGC6|=SIM_SCGC6_ADC0_MASK; //CLK de ADC0
	SIM_SCGC5|=SIM_SCGC5_PORTE_MASK;//Puerto E20
	//PTE20 ADC0 default
	ADC0_CFG1|= (ADC_CFG1_MODE(mode)|ADC_CFG1_ADIV(Prescaler_4)); //bus clock 24MHz, vel de muestreo
	
}

uint16_t ADC_Polling (uint8_t Channel)
{
	uint16_t Valor_ADC;
	ADC0_SC1A&= ~ADC_SC1_ADCH_MASK; //limpio canal
	ADC0_SC1A |= ADC_SC1_ADCH(Channel);//Habilito canal
	while ((ADC0_SC1A & ADC_SC1_COCO_MASK)==0); //reviso bandera
	Valor_ADC=ADC0_RA; //Apago bandera
	return Valor_ADC;
}
