#include <stdio.h>
#include <avr/io.h>
#include "setup.h"
#include <util/delay.h>


#define pin PORTC

void blink(){
	pin |= (0b00000001);
	_delay_ms(250);
	pin &= ~(0b00000001);
	_delay_ms(250);
	pin |= (0b00000001);
}
