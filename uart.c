/*****************************************************************************
​ ​* ​ ​ @file​ ​  uart.c
​ * ​ ​ @brief​ ​ This function initializes the UART and contains the blocking and
 *		     non-blocking method to communicate with the UART. It also contains
 *		     the interrupt initialization.
 *   @Tools_Used GCC
​ * ​ ​ @author​ ​Devansh Mittal, Souvik De
​ * ​ ​ @date​ ​ November 27th, 2018
​ * ​ ​ @version​ ​ 1.0
*****************************************************************************/

#include "uart.h"
#include "MKL25Z4.h"

call_back call_back_pointer;
uint16_t tamano_buffer;
uint8_t  index_I=0;
uint8_t*  pointer_buffer; //obtener info


/* Initializes UART */
void UART_init(void *UARTx, uint32_t Baud,call_back call_back_f)
{
	uint8_t i;
	uint32_t calculated_baud = 0;
	uint32_t baud_diff = 0;
	uint32_t osr_val = 0;
	uint32_t sbr_val, uart0clk;
	uint32_t baud_rate;
	uint32_t reg_temp = 0;
	uint32_t temp = 0;
	call_back_pointer = call_back_f; 

	if ((UART0_MemMapPtr) UARTx == UART0_BASE_PTR)
	{
		SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;			/* Enable GPIOA clock	*/
		SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;			/* Enable UART0 clock	*/
		
		/* Configure the GPIOs */
		PORTA_PCR1 |= PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x2);
		PORTA_PCR2 |= PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x2);
		

		/* Disable the transmit and receive enable bits while configuring UART0 */
		UART0_C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK);

		((UART0_MemMapPtr) UARTx)->C1 = 0x00; //default
		((UART0_MemMapPtr) UARTx)->C2 = UARTLP_C2_RE_MASK | UARTLP_C2_TE_MASK; //Enable transmitter and reciever
		((UART0_MemMapPtr) UARTx)->C3 = 0x00; //defualt
		((UART0_MemMapPtr) UARTx)->C4 = 0x00; //default
		((UART0_MemMapPtr) UARTx)->C5 = 0x00; //default
		SIM_SOPT2 &= ~(SIM_SOPT2_PLLFLLSEL_MASK);	/* Clock for UART0 - MCGFLLCLK */
		SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);		/* Using MCGFLLPLL */

		 baud_rate = Baud;
		 uart0clk = 48000000;
		 
			/* Calculate first baud rate using lowest osr value */
			i = 4;
			sbr_val = (uint32_t)(uart0clk/(baud_rate * i));
			calculated_baud = (uart0clk / (i * sbr_val));

			/* Calculate difference between calculated baud rate and desired baud rate */
			if (calculated_baud > baud_rate)
				baud_diff = calculated_baud - baud_rate;
			else
				baud_diff = baud_rate - calculated_baud;

			osr_val = i;

			/* Select the best OSR value */
			for (i = 5; i <= 32; i++)
			{
				sbr_val = (uint32_t)(uart0clk/(baud_rate * i));
				calculated_baud = (uart0clk / (i * sbr_val));

				if (calculated_baud > baud_rate)
					temp = calculated_baud - baud_rate;
				else
					temp = baud_rate - calculated_baud;

				/* Choose that osr value which has the lowest difference between calculated and desired baud rate */
				if (temp <= baud_diff)
				{
					baud_diff = temp;
					osr_val = i;
				}
			}

			if (baud_diff < ((baud_rate / 100) * 3))
			{
				

				/* Setup OSR value */
				reg_temp = UART0_C4;
				reg_temp &= ~UART0_C4_OSR_MASK;
				reg_temp |= UART0_C4_OSR(osr_val-1);

				/* Write reg_temp to C4 register */
				UART0_C4 = reg_temp;

				reg_temp = (reg_temp & UART0_C4_OSR_MASK) + 1;
				sbr_val = (uint32_t)((uart0clk)/(baud_rate * (reg_temp)));

				 /* Save off the current value of the uart0_BDH except for the SBR field */
				reg_temp = UART0_BDH & ~(UART0_BDH_SBR(0x1F));

				/* Write the calculated baud rate divisor value to the uart0_BDH and uart0_BDL register */
				UART0_BDH = reg_temp |  UART0_BDH_SBR(((sbr_val & 0x1F00) >> 8));
				UART0_BDL = (uint8_t)(sbr_val & UART0_BDL_SBR_MASK);

				/* Enable the receiver interrupt */
				UART0_C2 |= UART_C2_RIE_MASK;

				/* Enable the receiver */
				UART0_C2 |= UART0_C2_RE_MASK;
				UART0_C2 |= UART0_C2_TE_MASK;

				/* Enable Uart0 interrupt */
				enable_irq(INT_UART0 - 16);
			}
	}

}



void UART0_IRQHandler (void)
{

 if ((UART0_S1 & UART_S1_TDRE_MASK))
  {
	if (index_I<tamano_buffer) //mientras no se haya temrinado de mandar datos
	{
		UART0_D = pointer_buffer[index_I]; //va mandando conforme avanza el index
		index_I++;
	}
	else
	{
		UART0_C2 &= ~(UART_C2_TIE_MASK);
	}
  }

 if (UART0_S1&UART_S1_RDRF_MASK)
 {

	 call_back_pointer(UART0_D);
 }
}

/* Enabling the UART0 interrupt */
void enable_irq (int irq)
{
      /* Set the ICPR and ISER registers */
      NVIC_ICPR = 1 << (irq%32);
      NVIC_ISER = 1 << (irq%32);
}

void uart_write(uint8_t *p,uint16_t numero_bits)
{

	    index_I=0;  //se empieza a contar
		pointer_buffer = p;
		tamano_buffer = numero_bits;
		UART0_C2 |= UART_C2_TIE_MASK; //Enable the transmit interrupt
		
		do {} while ((UART0_S1 & UART_S1_TC_MASK )==0);

}

