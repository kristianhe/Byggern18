/*
 * motor_openloop.c
 *
 * Created: 01.11.2018 18.09.18
 *  Author: Simon
 */ 

#define OE PF3
#define RST PF4
#define SEL PF5
#define EN PF6
#define DIR PF7


void Motor_Init()
{
	// Setter alle portene vi vil bruke til motorstyring som outputs (MJ1)
	DDRF |= (1 << OE) | (1 << RST) | (1 << SEL) | (1 << EN) | (1 << DIR);
	
	// Setter alle portene vi vil bruke til lesing av enkoderen til inputs (MJ2)
	DDRC = 0;
	
	PORTF |= (1 << EN);		// Enable motor
	PORTF &= ~(1 << OE);	// Output enable (encoder) (active low)
	Encoder_Reset();		// Reset encoder
}

void Encoder_Reset()
{
	// Toggle !RST to reset encoder
	PORTF &= ~(1 << RST);		// Reset encoder (active low)
	_delay_ms(10);				// Wait
	PORTF |= (1 << RST);		// Toggle back
}

void Motor_SetVoltage(int motorVoltage)
{
	// Setter retningen til motoren etter fortegn på ønsket motorspenning
	if (motorVoltage > 0)	
	{
		PORTF |= (1 << DIR);
	}
	else					
	{
		PORTF &= ~(1 << DIR);
	}
	
	// Skriver absoluttverdien av spenningen til DAC-output
	DAC_Write(abs(motorVoltage));
}

uint16_t Encoder_Read()
{
	PORTF &= ~(1 << SEL);		// Setter SEL lav for å få de høye bittene
	_delay_us(30);				// Vent ~20 mikrosekunder
	uint8_t encoderH = 0;		// Leser de høyeste bittene
	
	PORTF |= (1 << SEL);		// Setter SEL høy for å få de lave bittene
	_delay_us(30);				// Vent ~20 mikrosekunder
	uint8_t encoderL = 0;		// Leser de laveste bittene
	
	// This 16-bit counter uses two’s complement form to represent negative numbers.
		
	
	Encoder_Reset();			// Resetter enkoderen
}