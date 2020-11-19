/*
 * Valvulas_driver.c
 *
 *  Created on: Nov 17, 2020
 *      Author: alesainz
 */

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Includes Section
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "Valvulas_driver.h"
#include "MKL25Z4.h"

//PTC3 -> conexion camaras
//PTC4 -> aire comprimido

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Functions Section
///////////////////////////////////////////////////////////////////////////////////////////////////
void init_valvulas (void)
{
	SIM_SCGC5|= SIM_SCGC5_PORTC_MASK;//clk PTC
	PORTC_PCR3 = PORT_PCR_MUX(GPIO_MUX); //GPIO's
	PORTC_PCR4 = PORT_PCR_MUX(GPIO_MUX); 
	GPIOC_PDDR|=  GPIO_PDDR_PDD(1<<PTC3_pin); //Salidas
	GPIOC_PDDR|=  GPIO_PDDR_PDD(1<<PTC4_pin); 
}

void abrir_valvula (uint8_t pin_a)
{
	
		GPIOC_PSOR|= (1<<pin_a);
	
}

void cerrar_valvula (uint8_t pin_c)
{
	
			GPIOC_PCOR|= (1<<pin_c);
		
}
