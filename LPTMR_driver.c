/*
 * LPTMR_driver.c
 *
 *  Created on: Oct 22, 2020
 *      Author: isae_
 */



///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Includes Section
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "LPTMR_driver.h"
#include "MKL25Z4.h"

vpfnGenericCallback LowerLevelCallbackPointer;

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Functions Section
///////////////////////////////////////////////////////////////////////////////////////////////////

void LPTMR_Init (void)
{
	SIM_SCGC5|= SIM_SCGC5_LPTMR_MASK;//CLK
	LPTMR0_PSR|= LPTMR_PSR_PBYP_MASK|LPTMR_PSR_PCS(CLK_LPO);//Bypas y LPO como base de tiempo
	NVIC_ISER|= Interrupt_LPTMR;
}



void LPTMR_delay (uint32_t time_delay)
{
	//LPTMR0_CSR &=~ LPTMR_CSR_TIE_MASK; //deshabilito interrupcion
	LPTMR0_CMR = time_delay - 1;
	LPTMR0_CSR |= LPTMR_CSR_TEN_MASK; //inicio conteo
	
	while (!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK));
	LPTMR0_CSR&=~LPTMR_CSR_TEN_MASK;	
	
	
}

void LPTMR_interrupt (uint32_t time_inter)
{
	LPTMR0_CMR = time_inter - 1;
	LPTMR0_CSR |= LPTMR_CSR_TIE_MASK|LPTMR_CSR_TEN_MASK; //se habilita interrupcion y timer
}

void LPTimer_IRQHandler (void)
{
	LPTMR0_CSR &= ~(LPTMR_CSR_TIE_MASK | LPTMR_CSR_TEN_MASK);
	LowerLevelCallbackPointer();
}
