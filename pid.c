#include "pid.h"

void pid_Init(int16_t Kp_in, int16_t Ki_in, int16_t Kd_in, struct PID_DATA *pid)
{
  pid->sumError = 0;
  pid->Kp = Kp_in;
  pid->Ki = Ki_in;
  pid->Kd = Kd_in;
}

int pid_Controller(int16_t setPoint, int16_t processValue, uint8_t Timestep, int deltaError, struct PID_DATA *pid_st)
{
  int p ,i ,d ,u, error;

  error = (int)(setPoint - processValue);

  if (Timestep == 0) pid_st->sumError = error;
  else pid_st->sumError += error;

  p = pid_st->Kp * error;
  i = pid_st->Ki * pid_st->sumError;
  d = pid_st->Kd * deltaError;

  u =  p + i + d;

  return u; //u = Kp*e + Ki*int(e)
}

void pid_Reset_Integrator(pidData_t *pid_st)
{
  pid_st->sumError = 0;
}



// -----------------------------------------------------------------------
// NY FOR GUI: OPPDATERE PID-VERDIER "ONLINE" / "LIVE"
void pid_ChangeControllerValues(int16_t Kp_in, int16_t Ki_in, int16_t Kd_in, struct PID_DATA *pid)
{
	pid->Kp = Kp_in;
	pid->Ki = Ki_in;
	pid->Kd = Kd_in;
}
