#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"
#include "Timer4.h"
#include "PID.h"
#include "Serial.h"
#include "DS18B20.h"

uint8_t RxData;  //���ڽ�������
char MyString[10];  //���ڷ�������
uint8_t IniTemp; //���������صĳ�ʼ�¶�ֵ
float Temp;  //ʵ���¶�
float Target = 8.0;
uint8_t Temp_int;  //ʵ���¶���������
uint8_t Temp_deci;  //ʵ���¶�С������
uint8_t Target_int;  //Ŀ���¶���������
uint8_t Target_deci;  //Ŀ���¶�С������
uint8_t Duty;//ռ�ձ�

uint8_t KeyNum;

uint8_t cnt = 0;


int main(void)
{
	OLED_Init();
	PWM_Init();
	//Timer4_Init();//���ڶ�ʱ�ж�
	Serial_Init();
	DS18B20_Init();
	Key_Init();
	
	while (1)
	{
		/////////////////////////////////////��ȡ�¶ȣ�OLED��ʾ�¶ȣ��Լ����ڷ����¶�
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
////		//////////////////////////////////////////////////������λ�����͵�Ŀ���¶�
//		if (Serial_GetRxFlag() == 1)      //��ʾ������յ���Ϣ
//		{
//			RxData = Serial_GetRxData();  //RxData��ʾ���յ�����Ϣ
//			sscanf(MyString, "%hhu.%hhu", &Target_int, &Target_deci);//�ѽ��յ����ַ���ȡ���������ֺ�С������
//			Target = Target_int + Target_deci / 10.0f;//����ϵõ�float���͵�Target
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
	
	
//void TIM4_IRQHandler(void)                  //ÿ100ms��һ���жϣ����ݵ�ǰ�¶�ִ��һ��PID����ռ�ձ�
//{
//	if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)     
//	{	
////		Duty_PID = (int)PID(Temp, Target);	

////		PWM_SetCompare3(Duty_PID);

//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
//	}
//}


	