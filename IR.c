/*
 * IR.c
 *
 * Created: 28.10.2018 13.12.37
 *  Author: Simon
 */ 

void IR_Init()
{
	// Les inn verdien fra IR-sensor på port F0
	DDRF &= ~(1 << PF0);	// Setter PF0 til input
	
	// ADCSRA – ADC Control and Status Register A (s. 285)
	ADCSRA |= (1 << ADEN);										// Enable ADC
	ADCSRA |= (1 << ADPS2) || (1 << ADPS1) || (1 << ADPS0);		// Division factor / prescale = 128. Usikker på hvilken som er rett, prøver dette først.
	
	// ADMUX – ADC Multiplexer Selection Register
	ADMUX |= (1 << REFS0);										// Bruker AVCC som referanse		(AVCC with external capacitor at AREF pin (!!!))
	
	// ADLAR: ADC Left Adjust Result
	ADMUX |= (1 << ADLAR);										// Resultatet blir left-shifted. Dette gjør at vi kan lese fra bare ADCH om vi ikke trenger mer enn 8-bit oppløsning
}

uint8_t IR_Read()
{
	// A single conversion is started by writing a logical one to the ADC Start Conversion bit, ADSC. This bit stays high as
	// long as the conversion is in progress and will be cleared by hardware when the conversion is completed.
	
	ADCSRA |= (1 << ADSC);	// ADSC: ADC Start Conversion. In Single Conversion mode, write this bit to one to start each conversion (s. 285)
	
	while(ADCSRA & (1 << ADSC));	// Når konverteringen er ferdig, settes ADSC lav av hardware. 
	
	// Resultatet havner i 10-bit registeret ADCH + ADCL. Right-shifted by default, men vi har satt til left-shifted i init. 
	// If the result is left adjusted and no more than 8-bit precision is required, it is sufficient to read ADCH
	return ADCH;
	
	// ADC = (Vin * 1024) / Vref
}

uint16_t IR_Read_withFilter()
{
	uint8_t sum = 0;
	uint8_t numberReadings = 4;
	
	for (int i = 0; i < numberReadings; i++)
	{
		sum += IR_Read();
	}
	
	return (sum / numberReadings);
}