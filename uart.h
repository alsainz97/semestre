/*
 * uart.h
 *
 *  Created on: Nov 15, 2020
 *      Author: alesainz
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include "derivative.h"
#include "MKL25Z4.h"

#define MUX_UART_ALT2   (0x2)

typedef void (*call_back)(uint8_t);

void UART_init(void *UARTx, uint32_t Baud, call_back call_back_f);


/**
* @Brief This function handles the interrupt operations for UART0.
*
* @Param  void
*
* @Return void
**/
void UART0_IRQHandler (void);

/**
* @Brief This function enables UART0.
*
* @Param interrupt number.
*
* @Return void
**/
void enable_irq (int irq);

/**
* @Brief This function writes data to be transmitted out of UART to the circular buffer.
*
* @Param pointer to character to be transmitted out of UART.
*
* @Return void
**/
void uart_write(uint8_t *p,uint16_t numero_bits);

#endif /* UART_H_ */
