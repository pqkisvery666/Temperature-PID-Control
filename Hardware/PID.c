#include "stm32f10x.h"                  // Device header

float Err=0,LastErr=0,LastLastErr=0;
float pwm_CCR=0,Add_CCR=0;                                //pwm新的PWM比较值，add，新的PWM占空比更改值
float p=20.0,i=0.4,d=1;     //比例系数、积分系数、微分系数  
                            //设定目标速度
float PID(float xTemp,float xTarget)
{
    Err=-(xTarget - xTemp);    	 //设定速度-实时速度
	
    Add_CCR = p*(Err-LastErr)+i*(Err)+d*(Err-2*LastErr+LastLastErr);   //PWM占空比增加的部分=比例环节+积分环节                         
	pwm_CCR+=Add_CCR;                                                      
	 
    LastErr=Err;							 //把最新误差赋值给上次误差
	LastLastErr = LastErr;
	
	if (pwm_CCR > 100)
		pwm_CCR = 100;
	if (pwm_CCR < 0)
		pwm_CCR = 0;
	
    return pwm_CCR;								 //返回PWM新的占空比值
}

