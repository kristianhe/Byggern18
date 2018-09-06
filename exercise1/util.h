#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "setup.h"

#define pin PORTC
#define OFFSET 0x1000                    // Markerer staren på de eksterne minneadressene

void blink()
{
	PORTC |= (0b00000001);
	_delay_ms(250);
	PORTC &= ~(0b00000001);
	_delay_ms(250);
	PORTC |= (0b00000001);
}

void enable_external_memory()
{
  set_bit(MCUCR, SRE);
}

void disable_external_memory()
{
  clear_bit(MCUCR, SRE);
}
