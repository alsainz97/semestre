/*
 * LPTMR_driver.h
 *
 *  Created on: Oct 22, 2020
 *      Author: isae_
 */

#ifndef LPTMR_DRIVER_H_
#define LPTMR_DRIVER_H_


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Includes Section
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "MKL25Z4.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Defines & Macros Section
///////////////////////////////////////////////////////////////////////////////////////////////////
#define CLK_LPO           (1)
#define delay_5seg        (delay_1sec * 5)
#define delay_1sec		  (uint32_t)(10000)
#define delay_30seg       (delay_1sec * 30)
#define delay_1min        (delay_1sec * 60)
#define delay_4min        (delay_1min * 4)
#define delay_5min        (delay_1min * 5)
#define delay_8min        (delay_1min * 8)
#define Interrupt_LPTMR   (1<<28)
#define Flag_W1c          (1<<7)

typedef void (*vpfnGenericCallback)(void);

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Functions Section
///////////////////////////////////////////////////////////////////////////////////////////////////

void LPTMR_Init (void);
void LPTMR_interrupt (uint32_t time_inter);
void LPTMR_delay (uint32_t time_delay);

#endif /* LPTMR_DRIVER_H_ */
