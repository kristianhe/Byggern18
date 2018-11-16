
#include "setup.h"
#include "uart.h"
#include "util.h"
#include "adc.h"
#include "slider.h"
#include "joystick.h"
#include "spi.h"
#include "MCP2515.h"
#include "can.h"
#include "timer.h"
#include "Motor_driver.h"
#include "pid.h"

#include <stdbool.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// PI
#define K_P		1
#define K_I 	0
#define K_D 	2
#define TIME_INTERVAL 157
struct PID_DATA pidData;

bool Deltime = false;
bool INTflag = false;

//Global counters
uint8_t trigCounter = 0;
uint8_t timeStep = 0;
uint8_t startupCounter = 0;

//Timer0 overflow interrupt
ISR(TIMER0_OVF_vect){
	trigCounter++;
	timeStep++;
	startupCounter++;
}

//Timer1 overflow interrupt
ISR(TIMER1_OVF_vect){
}

//Initialize the drivers
void drivers(){
	UART_Init(MYUBRR);
	PWM_init();
	timer0_init();
	timer1_init();
	canInit();
	IR_Init();
	MotorInit();
	pid_Init(K_P, K_I, K_D, &pidData);
}

int main(void)
{
	DDRE |= (1<<PE4);

	drivers();
	OCR3A = 3200;

	//Inputs and Outputs
	uint8_t servoVal = 0;
	uint8_t joyPos_x = 0;
	uint8_t dirVal = 0;
	uint8_t solenoid = 0;
	uint8_t sliderPos = 0;
	uint8_t IR_value = 255;

	//For PID controller
	uint16_t enc = 0;
	int motorInput = 0;
	uint16_t tmpEnc1 = 0;
	uint16_t tmpEnc2 = 0;
	int deltaEnc = 0;

	//Boolean control variables
	bool startFlag = false;
	bool trigFlag = false;
	bool trigCountFlag = false;

	//For state machine
	bool oddFlag = true;
	bool evenFlag = false;
	bool calcDelta = false;
	bool pidFlag = false;

	CAN_frame RXtest;
	RXtest.id = 0;
	RXtest.length = 5;
	RXtest.data[0] = 1;   //x pos
	RXtest.data[1] = 2;   //y pos
	RXtest.data[2] = 3; 	//x dir
	RXtest.data[3] = 4; 	//button
	RXtest.data[4] = 5; 	//sliPosLeft

	sei();

	//Reseting the position for the DC-motor and encoder
	while((startupCounter < 80) && !(startFlag)){
		PORTH |= (1 << DIR);
		DAC_Write(128);
	}
	Encoder_Reset();

	printf("Node 2 opertional\n");
	
	
	// TESTLOGIKK FOR STYRING VIA GUI
	unsigned char guidata = "";
	servoVal = 127;
	solenoid = 0;
	sliderPos = 127;
	
	
	while(1)
	{
		MotorContrl(motorInput);
		enc = Scale16(EncoderRead(), 10000, 200, 255, 0);
		OCR3A = Scale(servoVal, 255,0, 4600, 1800);
		IR_value = IR_Read_withFilter();
		
		/*
		if (canRecive(&RXtest))
		{
			servoVal = RXtest.data[0];
			dirVal =  RXtest.data[2];
			solenoid = RXtest.data[3];
			sliderPos = RXtest.data[4];
		}
		*/
		
		// NY FOR GUI. UART_Receive_GUI() returnerer 1 om det er ulest data i RX-bufferen, og putter dataen i guidata. Returnerer 0 om vi ikke har mottatt noe.
		if (UART_Receive_GUI(&guidata))
		{
			if (guidata[1] == 'S')		// Solenoid
			{
				solenoid = (uint8_t)guidata[2];
			}
			else if (guidata[1] == 'D')	// Servo
			{
				servoVal = (uint8_t)guidata[2];
			}
			else if (guidata[1] == 'R')	// DC-motor referanse (slider)
			{
				sliderPos = (uint8_t)guidata[2];
			}
			else if (guidata[1] == 'C') // PID-verdier
			{
				// Gjør ingenting enda
			}
		}

		//State machine ---------------------------------------
		if (((timeStep%2) == 0) && (oddFlag) && !(evenFlag)){ //Previous encoder value, sample at odd numbers of the counter value
			tmpEnc1 = enc;
			oddFlag = false;
			evenFlag = true;
		}
		else if (((timeStep%2) != 0) && (evenFlag) && !(oddFlag)){ //current encoder value, sample at even numbers of the counter value
			tmpEnc2 = enc;
			evenFlag = false;
			calcDelta = true;
		}
		if (calcDelta){
			deltaEnc = (int)(tmpEnc2 - tmpEnc1);
			calcDelta = false;
			pidFlag = true;
		}
		if (pidFlag){
			motorInput = pid_Controller(sliderPos, enc, timeStep, deltaEnc, &pidData);
			pidFlag = false;
			oddFlag = true;
		}
		//--------------------------------------------------------

		// Logic for solenoid trigering--------------------------
		if (solenoid == 133) trigFlag = true;
		if (trigFlag){
			PORTE |= (1<<PE4);
			printf("jalla \n");
			trigFlag = false;
			trigCountFlag = true;
			trigCounter = 0;
		}
		if ((trigCountFlag) && (trigCounter > 10)){
			PORTE &= ~(1<<PE4);
			printf("not jalla \n");
			trigCountFlag = false;
			trigCounter = 0;
		}
		//--------------------------------------------------------

		if(timeStep > 10)
	  {
			timeStep = 0; //reseting the counter
		}

	}

	return 0;
}
