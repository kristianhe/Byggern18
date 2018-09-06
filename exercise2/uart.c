#include "uart.h"
#include <avr/io.h>

FILE *uart;

char test[10];

int UART_Init(unsigned int ubrr)
{
	//Setter UART0-baudrate
	UBRR0L = (unsigned char)(ubrr);
	UBRR0H = (unsigned char)(ubrr >> 8);

	// Sl�r p� receiver (RXEN0) og transmitter (TXEN0)
	UCSR0B = (1<<RXCIE0) | (1<<TXCIE0) | (1<<RXEN0) | (1<<TXEN0);
	//UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	// Setter frame-formatet: 8data, 2stop bit
	UCSR0C = (1<<URSEL0) | (1<<USBS0) | (3<<UCSZ00);

	// Kobler printf. Neat'o. Lite triks fra tidligere gruppe
	uart = fdevopen(&UART_Transmit, &UART_Receive);

	return 0;
}

int UART_Transmit(char c, FILE *f)
{
	// Venter p� tom sendings-buffer
	while (!(UCSR0A & (1<<UDRE0)));

	// Putter dataen (char) i bufferen (UDR0)
	UDR0 = c;

	return 0;
}

unsigned char UART_Receive(void)
{
	// Venter på at data skal mottas
	while (!(UCSR0A & (1<<RXC0)));

	// Grabber og returnerer mottatt data fra bufferen
	return UDR0;
}

// Flushing av receive-bufferen
void USART_Flush( void )
{
	unsigned char dummy;
	while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
}
/*
// Print-funksjon.
int UART_print(char * tekst)
{
	int i = 0;
	while (tekst[i] != 0x00)	// Så lenge vi har mer tekst å gå gjennom ...
	{
		UART_Transmit(tekst[i], uart);	// .. send gjeldende char
		i++;
	}
	return 0;
}
*/

/*
int UART_mottat(uin8_t a){


}
*/
