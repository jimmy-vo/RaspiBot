#include "Speed_PID.h"
#include "Timing_Manage.h"

/*working variables*/
unsigned long lastTime;
double Input, Output, Setpoint;
double errSum, lastErr;
double kp, ki, kd;

void Compute()
	{
		/*How long since we last calculated*/
		unsigned long now = Get_msTick();
		double timeChange = (double)(now - lastTime);
		double dErr;
		double error;
		
		/*Compute all the working error variables*/
		error = Setpoint - Input;
		errSum += (error * timeChange);
		dErr = (error - lastErr) / timeChange;

		/*Compute PID Output*/
		Output = kp * error + ki * errSum + kd * dErr;

		/*Remember some variables for next time*/
		lastErr = error;
		lastTime = now;
	}
  
void SetTunings(double Kp, double Ki, double Kd)
	{
		kp = Kp;
		ki = Ki;
		kd = Kd;
	}
