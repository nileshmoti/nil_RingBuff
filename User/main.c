/*
 * main.c
 *
 *  Created on: 14 Sep 2017
 *      Author: nmoti
 */



/* Includes */

#include "nil_AIO.h"
#include "stdio.h"

#include "nil_RingBuff.h"

/* Functions */
void Init_USART();


/* Variables */
char Serial_Receive_String[100] = {'\0'};

int main(void)
{
	/* Initialize Components */
	Init_USART();
	Init_Timer2();

	char data2[] = "Hell0my";
	char data[] = "What";

char *buffer;

while(1){


WriteRingBuffer(data);
buffer = ReadRingBuff();
printf("Outcome: %s \n", buffer);

WriteRingBuffer(data2);
buffer = ReadRingBuff();
printf("Outcome2: %s \n", buffer);




	}

	return 0;

}











void Init_USART(){
	/* Initialize USART1 */
 	InitUSART( USART1,					//Select what USART to use
 		       GPIOA,					//Select port of USART
		       GPIO_Pin_9,				//USART's Tx Pin
		       GPIO_Pin_10,				//USART's Rx Pin
			   RCC_AHBPeriph_GPIOA,		//Select port of USART, should be same as above
			   RCC_APB2Periph_USART1,	//Change accordingly, check in stm32f30x.h for correct entry
			   GPIO_PinSource9,			//USART's Tx Pin, should be same as above
			   GPIO_PinSource10,		//USART's Rx Pin, should be same as above
			   USART1_IRQn);			//Interrupt on which USART
}

void USART1_IRQHandler(void){

	int k;
	//int Rx_USART_Count = 0;

	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE)){							//Test if Rx Not Empty

		if (Rx_USART_Count < 100){
			Serial_Receive_String[Rx_USART_Count] = USART_ReceiveData(USART1);		//If Rx_USART_String[] not full, receive message
		}
		else if (Rx_USART_Count > 99){												//If Rx_USART_String[] full, shift out 1st Character

			for (k = 0; k < 99 ; k++){
				Serial_Receive_String[k] = Serial_Receive_String[k + 1];			//First Character replaced with 2nd etc....
			}

			Serial_Receive_String[99] = USART_ReceiveData(USART1);					//Receive last Character
			Rx_USART_Count = 99;													//Keep counter at NON-Reset value
		}

		Rx_USART_Count++;															//Increment Counter
	}

	USART_ClearFlag(USART1, USART_FLAG_ORE);

	//Serial_Transmit_String(USART1, Serial_Receive_String);							// Transmits what is received to terminal, not needed, used for testing

}
