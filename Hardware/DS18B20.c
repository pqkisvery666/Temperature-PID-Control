#include "stm32f10x.h"                  // Device header
#include "Delay.h"


#define DQ_H     GPIO_SetBits(GPIOB,GPIO_Pin_1)        			//PB1����
#define DQ_L     GPIO_ResetBits(GPIOB,GPIO_Pin_1)						//PB1����
#define DQ_Read  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)    //PB1��ȡ

uint8_t flag=0;

void DS18B20_Init(void) 																		//PB1��ʼ��
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;             //DQ=PB1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void DS18B20_Output(void)      //���ģʽ            
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;             
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void DS18B20_Input(void)      //����ģʽ
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;             
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t DS18B20_Reset(void)      //��λ
{
	uint8_t flag;
	DS18B20_Output();
	DQ_H;
	Delay_us(5);
	
	DQ_L;
	Delay_us(480);
	DQ_H;
	Delay_us(60);
	DS18B20_Input();
	flag=DQ_Read;
	Delay_us(480);
	DS18B20_Output();
	DQ_H;
	return flag;
}

void DS18B20_WriteData(uint8_t data)     // д����
{
	for(uint8_t i=0;i<8;i++)
	{
		DS18B20_Output();  //���״̬
		DQ_L;
		Delay_us(2);
		if(data&0x01)     //��λ��ʼ������һ����Ƶ����ϸ����
		{
			DQ_H;
		}
		else
		{
			DQ_L;
		}
		Delay_us(60);
		DQ_H;
		data = data>>1;
	}
	
}

uint8_t DS18B20_ReadData(void)      //������
{
	uint8_t data =0;
	
	for(uint8_t i=0;i<8;i++)
	{
		data=data>>1;
		DS18B20_Output();   //���״̬
		DQ_L;
		Delay_us(2);
		DQ_H;
		Delay_us(2);
		DS18B20_Input();   //����״̬
		if(DQ_Read)
			data|=0x80;     //�����λ������λ����λ������һ����Ƶ����ϸ����
		Delay_us(60);
	}
	return data;
}

uint16_t DS18B20_ReadTemp(void)   //��ȡ�¶�
{
	uint8_t DL,DH;
	uint16_t data;	
	uint16_t Temperature=0;
	flag=0;
	DS18B20_Reset();              //��λ
	DS18B20_WriteData(0XCC);      //����ROM���
	DS18B20_WriteData(0X44);      //�����¶�ת��
	Delay_ms(750);                //��ʱ���ȴ�ת�����
	DS18B20_Reset();              //��λ
	DS18B20_WriteData(0XCC);      //����ROM���
	DS18B20_WriteData(0XBE);      //��ȡ�ݴ���ָ��
	DL=DS18B20_ReadData();        //���¶ȵ�λ
	DH=DS18B20_ReadData();        //���¶ȸ�λ
	data=DH;
	data=data<<8;
	data|=DL;
	if((data&0XF800)==0XF800)    //����һ����Ƶ������ϸ����
	{
		data=~data+0X01;
		flag=1;
	}
		
	Temperature=data * 0.0625*10;
	return Temperature;
}

