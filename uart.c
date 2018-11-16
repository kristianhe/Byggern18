#include "uart.h"

FILE *uart;

char test[10];

void UART_Init(unsigned int ubrr)
{
	//Setter UART0-baudrate
	UBRR0L = (unsigned char)(ubrr);
	UBRR0H = (unsigned char)(ubrr >> 8);
	UCSR0B = (1<<RXEN0) | (1<<TXEN0); // Slår på receiver (RXEN0) og transmitter (TXEN0)
	//UCSR0C = (1<<URSEL0) | (1<<USBS0) | (3<<UCSZ00); // Setter frame-formatet: 8data, 2stop bit. None parity
	UCSR0C = (1<<USBS0) | (3<<UCSZ00); // Setter frame-formatet: 8data, 2stop bit. None parity
	//UCSR0B = (1 << RXCIE0);	// RX Complete Interrupt Enable. Gjør at vi får en interrupt når data er mottatt
	uart = fdevopen(&UART_Transmit, &UART_Receive);// Kobler printf. Neat'o. Lite triks fra tidligere gruppe
}

int UART_Transmit(char c, FILE *f)
{
	// Venter p� tom sendings-buffer
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = c; // Putter dataen (char) i bufferen (UDR0)
	return 0;
}

unsigned char UART_Receive(void)
{
	while (!(UCSR0A & (1<<RXC0))); // Venter på at data skal mottas

	return UDR0; // Grabber og returnerer mottatt data fra bufferen
}

// Flushing av receive-bufferen
void USART_Flush( void )
{
	unsigned char dummy;
	while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
}




// -----------------------------------------------------------------------
// NY FOR GUI
uint8_t UART_Receive_GUI(unsigned char * guidata)
{
	if ((UCSR0A & (1<<RXC0)))
	{
		guidata = UDR0;	// Clearer RXC-flagget når vi leser fra UDR.
		return 1;
	}
	return 0;
}
