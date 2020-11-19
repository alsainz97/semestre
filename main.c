/*
 
 * main implementation: use this 'C' sample to create your own application
 *
 */





#include "derivative.h" /* include peripheral declarations */
#include "MKL25Z4.h"
#include "uart.h"
#include "conversion_ADC.h"
#include "LPTMR_driver.h"
#include "Valvulas_driver.h"

#define Bus_CLK  (uint32_t)(24000000)
#define Baud_rate (uint32_t)(21013)

#define dato1_Tx_Size		(uint16_t)(sizeof(dato1_Tx)/sizeof(dato1_Tx[0]))
#define dato2_Tx_Size		(uint16_t)(sizeof(dato2_Tx)/sizeof(dato2_Tx[0]))
#define dato3_Tx_Size		(uint16_t)(sizeof(dato3_Tx)/sizeof(dato3_Tx[0]))
#define dato4_Tx_Size		(uint16_t)(sizeof(dato4_Tx)/sizeof(dato4_Tx[0]))
#define dato5_Tx_Size		(uint16_t)(sizeof(dato5_Tx)/sizeof(dato5_Tx[0]))
#define dato6_Tx_Size		(uint16_t)(sizeof(dato6_Tx)/sizeof(dato6_Tx[0]))
#define dato7_Tx_Size		(uint16_t)(sizeof(dato7_Tx)/sizeof(dato7_Tx[0]))
#define dato8_Tx_Size		(uint16_t)(sizeof(dato8_Tx)/sizeof(dato8_Tx[0]))
#define dato9_Tx_Size		(uint16_t)(sizeof(dato9_Tx)/sizeof(dato9_Tx[0]))
#define dato10_Tx_Size		(uint16_t)(sizeof(dato10_Tx)/sizeof(dato10_Tx[0]))
#define dato11_Tx_Size		(uint16_t)(sizeof(dato11_Tx)/sizeof(dato11_Tx[0]))

uint8_t Buffer_Rx[10];
uint8_t dato1_Tx[]={"Prender placa\n"};
uint8_t dato2_Tx[]={"no se prendio\n"};
uint8_t dato3_Tx[]={"Salio de rango\n"};
uint8_t dato4_Tx[]={"Peligro de sobrecalentamiento\n"};
uint8_t dato5_Tx[]={"Apagar placa\n"};
uint8_t dato6_Tx[]={"No se apago placa\n"};
uint8_t dato7_Tx[]={"Abrir bomba\n"};
uint8_t dato8_Tx[]={"No se abrio bomba\n"};
uint8_t dato9_Tx[]={"Cerrar aire comprimido\n"};
uint8_t dato10_Tx[]={"No se cerro aire comprimido\n"};
uint8_t dato11_Tx[]={"Finalizó proceso\n"};
uint8_t index_Rx=0;
uint8_t resultado_conversion;

void final_recieve (uint8_t data);
void Delay(void);
void lmpbffer(void);


int main(void)
{
	uint8_t j;
	uint8_t error=0;
//init modulos
	UART_init((void *)UART0_BASE_PTR, Baud_rate ,&final_recieve);
	LPTMR_Init ();
	Init_ADC();
	init_valvulas();
	
	
	
	for(;;) 
	{	 
		if (Buffer_Rx[0]=='Z')
		{
			lmpbffer();
		    //SUBLIMACION
			//Prender placa
			uart_write(&dato1_Tx[0],dato1_Tx_Size);
			LPTMR_delay (delay_5min); //polling
					
			if (Buffer_Rx[0]!='B')//no se prendio placa
			{		
				uart_write(&dato2_Tx[0],dato2_Tx_Size); //UART-cel alarma?
			}
			else
			{
				//lmpbffer();
				resultado_conversion = conversion();
			    while (resultado_conversion<21)
			    {
				LPTMR_delay (delay_5seg);
				resultado_conversion = conversion();
			    }
			    for (j=0;j<96;j++)
				{
					LPTMR_delay (delay_5seg);
					resultado_conversion = conversion();
					if ((24<=resultado_conversion) && (resultado_conversion<=27))
					{
						uart_write(&dato3_Tx[0],dato3_Tx_Size);//salio de rango
						error=1;
					}
					else if (resultado_conversion>90)//peligro de sobrecalentamiento
					{
						uart_write(&dato4_Tx[0],dato4_Tx_Size);
						error=2;
						//detener
					}
					
					if((error==0) && (j==12))
					{
						uart_write(&dato5_Tx[0],dato5_Tx_Size); //apagar placa
					}
				}	
						
					if (Buffer_Rx[0]!='G')//no se apago placa
					{
						uart_write(&dato6_Tx[0],dato6_Tx_Size); //UART-cel alarma
						//detener proceso
					}
				    else 
					{
				    	//lmpbffer();
						abrir_valvula (3);
						LPTMR_delay (delay_4min);
						uart_write(&dato7_Tx[0],dato7_Tx_Size);//abrir bomba
						LPTMR_delay (delay_1min);
								
						if (Buffer_Rx[0]!='J')
						{
							uart_write(&dato8_Tx[0],dato8_Tx_Size); //no se abrio bomba
						}
						else 
						{
							//lmpbffer();
							cerrar_valvula (3);
							abrir_valvula (4); //aire comprimido
							LPTMR_delay (delay_30seg);
							cerrar_valvula (4);
							uart_write(&dato9_Tx[0],dato9_Tx_Size);//cerrar aire
							LPTMR_delay (delay_1min);
							if (Buffer_Rx[0]!='M')
							{
								uart_write(&dato10_Tx[10],dato10_Tx_Size);//no se cerro
							}
							else
							{
								lmpbffer();
								uart_write(&dato11_Tx[0],dato11_Tx_Size);//finalizo 
							}
						}
								
					}
						
			     }
			  }
	}
	
	return 0;
}

void final_recieve (uint8_t data)
{
	uint8_t *pBffer = Buffer_Rx;
	
		if (index_Rx>=3)
		{
			index_Rx = 0;
		}
		*pBffer=data;
		index_Rx++;
		
}

void lmpbffer(void)
{
	uint8_t index=0;
	index_Rx = 0;
    while (index<3)
    {
    	Buffer_Rx[index]=0;
    	index++;
    }
}

