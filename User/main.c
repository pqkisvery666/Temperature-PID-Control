#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"
#include "Timer4.h"
#include "PID.h"
#include "Serial.h"
#include "DS18B20.h"

uint8_t RxData;  //串口接收数据
char MyString[10];  //串口发送数据
uint8_t IniTemp; //传感器返回的初始温度值
float Temp;  //实际温度
float Target = 8.0;
uint8_t Temp_int;  //实际温度整数部分
uint8_t Temp_deci;  //实际温度小数部分
uint8_t Target_int;  //目标温度整数部分
uint8_t Target_deci;  //目标温度小数部分
uint8_t Duty;//占空比

uint8_t KeyNum;

uint8_t cnt = 0;


int main(void)
{
	OLED_Init();
	PWM_Init();
	//Timer4_Init();//用于定时中断
	Serial_Init();
	DS18B20_Init();
	Key_Init();
	
	while (1)
	{
		/////////////////////////////////////获取温度，OLED显示温度，以及串口发送温度
		if(DS18B20_Reset()==0)
		{
			IniTemp = DS18B20_ReadTemp();
			Temp_int = IniTemp/10%100;
			Temp_deci = IniTemp%10;
			Temp = Temp_int + Temp_deci / 10.0f;
		
			if(flag==1)
			{
				OLED_ShowString(1, 1, "Temp=");
				OLED_ShowString(1, 9, ".");
				
				OLED_ShowString(1, 6, "-");
				OLED_ShowNum(1,7,Temp_int,2);
				OLED_ShowNum(1,10,Temp_deci,1);
			}
			else
			{
				OLED_ShowString(1, 1, "Temp=");
				OLED_ShowString(1, 9, ".");
				
				OLED_ShowString(1, 6, "+");
				OLED_ShowNum(1,7,Temp_int,2);
				OLED_ShowNum(1,10,Temp_deci,1);
			}
			sprintf(MyString, "%u.%u", Temp_int, Temp_deci);
			Serial_SendString(MyString);
		}
		else
		{
			OLED_ShowString(1,1,"No sensor");
			//Delay_ms(5);
		}
//		
//		
//		
////		//////////////////////////////////////////////////接收上位机发送的目标温度
//		if (Serial_GetRxFlag() == 1)      //表示如果接收到信息
//		{
//			RxData = Serial_GetRxData();  //RxData表示接收到的信息
//			sscanf(MyString, "%hhu.%hhu", &Target_int, &Target_deci);//把接收到的字符串取出整数部分和小数部分
//			Target = Target_int + Target_deci / 10.0f;//再组合得到float类型的Target
//		}	
	//	OLED_ShowNum(2,1,SetSpeed,3);	
		
		
		
		
		KeyNum = Key_GetNum();
		
		//OLED_ShowNum(2,1,SetSpeed,3);	
		
		if (KeyNum == 2)
		{
			Target -= 1.0;
			if (Target < 1)
				Target = 8;
		}
//		
		OLED_ShowNum(3,1,(int)Target,3);		

//		
//		PWM_SetCompare3(SetSpeed);
		
//		if ((Temp - Target) > 1.0)
//		{
//			Duty = 100;
//			cnt = 0;
//		}
//		else if ((Target - Temp) > 1.0)
//		{
//			Duty = 0;
//			cnt = 0;
//		}
//		else
//		{
//			cnt += 1;
//			if (cnt == 1)
//			{
//				pwm_CCR = Duty;
//			}
			Duty = (int)PID(Temp, Target);
//		}
		
			PWM_SetCompare3(Duty);		
		
		OLED_ShowNum(2,1,Duty,3);

		Delay_ms(50);
	}
}	
	
	
//void TIM4_IRQHandler(void)                  //每100ms进一次中断，根据当前温度执行一次PID调整占空比
//{
//	if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)     
//	{	
////		Duty_PID = (int)PID(Temp, Target);	

////		PWM_SetCompare3(Duty_PID);

//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
//	}
//}


	