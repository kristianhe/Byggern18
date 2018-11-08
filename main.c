/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief Example of use of general PID implementation for AVR.
 *
 * Example of how to setup and use the general PID implementation in pid.c.
 *
 * - File:               main.c
 * - Compiler:           IAR EWAAVR 4.11A
 * - Supported devices:  All AVR devices can be used.
 * - AppNote:            AVR221 - Discrete PID controller
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support email: avr@atmel.com
 *
 * $Name$
 * $Revision: 456 $
 * $RCSfile$
 * $Date: 2006-02-16 12:46:13 +0100 (to, 16 feb 2006) $
 *****************************************************************************/

#include <inavr.h>
#include <ioavr.h>
#include "stdint.h"
#include "pid.h"

#define K_P     1.00
#define K_I     0.00


struct PID_DATA pidData;

#define TIME_INTERVAL   157

#pragma vector = TIMER0_OVF_vect
__interrupt void TIMER0_OVF_ISR( void )
{
  static uint16_t i = 0;
  if(i < TIME_INTERVAL)
    i++;
  else{
    PIDflag = true;
    i = 0;
  }
}

/*! \brief Init of PID controller demo
 */
void Init(void)
{
  pid_Init(K_P * SCALING_FACTOR, K_I * SCALING_FACTOR, &pidData);

  TCCR0A = (1<<CS00);
  TIMSK0 = (1<<TOIE0);
  TCNT0 = 0;
}

void main(void)
{
  int16_t referenceValue, measurementValue, inputValue;
  Init();

  while(1){

    // Run PID calculations once every PID timer timeout
    if(PIDflag)
    {
      referenceValue = Get_Reference();			// Hent joystickverdi
      measurementValue = Get_Measurement();		// Hent enkoderverdi

      inputValue = pid_Controller(referenceValue, measurementValue, &pidData);

      Set_Input(inputValue);					// Send verdi til motoren

      PIDflag = false;
    }
  }
}

/*! \mainpage
 * \section Intro Introduction
 * This documents data structures, functions, variables, defines, enums, and
 * typedefs in the software for application note AVR221.
 *
 * \section CI Compilation Info
 * This software was written for the IAR Embedded Workbench 4.11A.
 *
 * To make project:
 * <ol>
 * <li> Add the file main.c and pid.c to project.
 * <li> Under processor configuration, select desired Atmel AVR device.
 * <li> Enable bit definitions in I/O include files
 * <li> High optimization on speed is recommended for best performance
 * </ol>
 *
 * \section DI Device Info
 * The included source code is written for all Atmel AVR devices.
 *
 * \section TDL ToDo List
 * \todo Put in own code in:
 * \ref Get_Reference(void), \ref Get_Measurement(void) and \ref Set_Input(int16_t inputValue)
 *
 * \todo Modify the \ref K_P (P), \ref K_I (I) and \ref K_D (D) gain to adapt to your application
 * \todo Specify the sampling interval time \ref TIME_INTERVAL
 */

