#ifndef __PID_H
#define __PID_H

extern float Err;
extern float pwm_CCR;

float PID(float xTemp,float xTarget);


#endif
