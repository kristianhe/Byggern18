/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief General PID implementation for AVR.
 *
 * Discrete PID controller implementation. Set up by giving P/I/D terms
 * to Init_PID(), and uses a struct PID_DATA to store internal values.
 *
 * - File:               pid.c
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

#include "pid.h"
#include "stdint.h"

void pid_Init(int16_t Kp_in, int16_t Ki_in, struct PID_DATA *pid)
{
  pid->sumError = 0;
  pid->Kp = Kp_in;
  pid->Ki = Ki_in;
}

int16_t pid_Controller(int16_t setPoint, int16_t processValue, struct PID_DATA *pid_st)
{
  int16_t error, p;
  int32_t i, ret;

  error = setPoint - processValue;
  pid_st->sumError += error;

  p = pid_st->Kp * error;  
  i = pid_st->Ki * pid_st->sumError;

  ret = (p + i) / SCALING_FACTOR;

  return((int16_t)((p + i) / SCALING_FACTOR));
}

void pid_Reset_Integrator(pidData_t *pid_st)
{
  pid_st->sumError = 0;
}
