#include "stm32f10x.h"                  // Device header

float Err=0,LastErr=0,LastLastErr=0;
float pwm_CCR=0,Add_CCR=0;                                //pwm�µ�PWM�Ƚ�ֵ��add���µ�PWMռ�ձȸ���ֵ
float p=20.0,i=0.4,d=1;     //����ϵ��������ϵ����΢��ϵ��  
                            //�趨Ŀ���ٶ�
float PID(float xTemp,float xTarget)
{
    Err=-(xTarget - xTemp);    	 //�趨�ٶ�-ʵʱ�ٶ�
	
    Add_CCR = p*(Err-LastErr)+i*(Err)+d*(Err-2*LastErr+LastLastErr);   //PWMռ�ձ����ӵĲ���=��������+���ֻ���                         
	pwm_CCR+=Add_CCR;                                                      
	 
    LastErr=Err;							 //��������ֵ���ϴ����
	LastLastErr = LastErr;
	
	if (pwm_CCR > 100)
		pwm_CCR = 100;
	if (pwm_CCR < 0)
		pwm_CCR = 0;
	
    return pwm_CCR;								 //����PWM�µ�ռ�ձ�ֵ
}

