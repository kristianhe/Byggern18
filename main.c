/*
 * FungerendeKode.c
 *
 * Created: 31.08.2018 17.26.17
 *  Author: Simon
 */
#include "uart.h"
#include "memory.h"
#include "setup.h"
#include "util.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


int main(void)
{
	char c[50];
		DDRC = 0x01;
		UART_Init(MYUBRR);	// Finner MYUBRR i setup.h
		printf("Vi har kontakt.\n\r");

		blink();
		// Muliggj�r globale interrupts
		// sei();
			USART_Flush();

	    while(1)
	    {
				blink();
				//UART_print("Mottat\n\r");

				printf("Enter value\n\r");
				scanf("%s",c);
				printf("Mottatt: %s\n\r",c);
				_delay_ms(2000);
	    }
		return 0;
}
