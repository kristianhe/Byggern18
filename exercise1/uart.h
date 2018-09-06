#include <stdio.h>

#ifndef UART_H_
#define UART_H_

int UART_Init(unsigned int baudrate);
int UART_Transmit(char c, FILE *f);
unsigned char UART_Receive(void);
void USART_Flush(void);
//int UART_print(char * tekst);
extern FILE *uart;

#endif
