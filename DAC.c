/*
 * DAC.c
 *
 * Created: 01.11.2018 17.13.45
 *  Author: Simon
 */ 

#include "TWI_Master.h"

// Initialiserer I2C/TWI-kommunikasjon
void DAC_Init()
{
	// Set up the TWI master to its initial standby state
	TWI_Master_Initialise();
	
	// Er dette n�dvendig? Er de outputs fra f�r av? Sikkert un�dvendig
	// DDRD |= (1 << PD1) | (1 << PD0);	// Setter PD0 (SCL) og PD1 (SDA) som outputs. 
}

// Skriver fra MCU til DAC-IC (MAX520) vha I2C/TWI-bussen. Vi styrer motorspenning med utgangen til DACen.
void DAC_Write(uint8_t motorVoltage)
{
	uint8_t data[3];
	
	data[0] = 0x50;			// Slave adress byte (uendret, de f�rste 0101 er fabrikk-kodet. Resten er 0000).
	// data[0] = 0x00;		// Pr�v begge deler. Skal gi det samme, siden 0101 er fabrikk-kodet. 	
	data[1] = 0;			// Command byte (ikke gj�r noe, vi skal bare sette DAC-outputs)
	data[2] = motorVoltage;	// Output byte 
	
	TWI_Start_Transceiver_With_Data(data, 3);	// 3 er meldingslengden. Logikk for START og STOPP skjer i denne funksjonen
}