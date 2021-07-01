#include "wave.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "wai_bian.h"
#include "wdg.h"

#define Trig GPIO_Pin_4
#define Echo GPIO_Pin_6

#define Trig1 GPIO_Pin_1
#define Echo1 GPIO_Pin_3

#define Trig2 GPIO_Pin_2
#define Echo2 GPIO_Pin_4

#define Trig3 GPIO_Pin_7
#define Echo3 GPIO_Pin_10

u16 Distance_zhen;
u16 Distance_zhen1;
u16 Distance_zhen2;
u16 Distance_zhen3;

float Distance;
float Distance1;
float Distance2;
float Distance3;

void Wave_SRD_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitSture;
	EXTI_InitTypeDef  EXTI_InitSture;
	NVIC_InitTypeDef  NVIC_InitSture;
	//����ⲿ�жϵĻ���һ��ʹ��AFIO���ù���
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOE,ENABLE);
	
	
	//����IO�˿�
	GPIO_InitSture.GPIO_Mode=GPIO_Mode_Out_PP;   //�������ģʽ
	GPIO_InitSture.GPIO_Pin=Trig;                //��PE4��Trig����
	GPIO_InitSture.GPIO_Speed=GPIO_Speed_50MHz;  
	GPIO_Init(GPIOE,&GPIO_InitSture);
	
	GPIO_InitSture.GPIO_Mode=GPIO_Mode_IPD;      //������ģʽ
	GPIO_InitSture.GPIO_Pin=Echo;                //��PE6��Echo����
	GPIO_InitSture.GPIO_Speed=GPIO_Speed_50MHz;  
	GPIO_Init(GPIOE,&GPIO_InitSture);
	
	//�жϺ�6�˿�ӳ��һ��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource6);
	
	//�ⲿ�ж�����
	EXTI_InitSture.EXTI_Line=EXTI_Line6;
	EXTI_InitSture.EXTI_LineCmd=ENABLE;
	EXTI_InitSture.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitSture.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitSture);
	
	
	//�ж����ȼ�����
	NVIC_InitSture.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVIC_InitSture.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitSture.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitSture.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_InitSture);
}

void EXTI9_5_IRQHandler(void)
{
	
	delay_us(20);
	
	
	if(EXTI_GetITStatus(EXTI_Line6)!=RESET)
	{
		TIM_SetCounter(TIM3,0);
		TIM_Cmd(TIM3,ENABLE);
		
		while(GPIO_ReadInputDataBit(GPIOE,Echo));  //�ȴ��͵�ƽ
		
		TIM_Cmd(TIM3,DISABLE);
		
		Distance=TIM_GetCounter(TIM3)*340/200.0;
		
		if(Distance>0)
		{
			printf("Distance:%f cm\r\n",Distance);
			Distance_zhen=Distance;
			LCD_ShowxNum(132,70,Distance_zhen,3,16,0);
			Distance-=Distance_zhen;
			Distance*=1000;
			LCD_ShowxNum(164,70,Distance,3,16,0);


			
		}
			
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	
	
}

void Wave_SRD_Strat(void)
{
	GPIO_SetBits(GPIOE,Trig);   //��Trig����Ϊ�ߵ�ƽ
	delay_us(90000);               //��������10us����������������ģ�鹤��
	GPIO_ResetBits(GPIOE,Trig); 
	
}

void Wave2_SRD_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitSture;
	EXTI_InitTypeDef  EXTI_InitSture;
	NVIC_InitTypeDef  NVIC_InitSture;
	//����ⲿ�жϵĻ���һ��ʹ��AFIO���ù���
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOF,ENABLE);
	
	
	//����IO�˿�
	GPIO_InitSture.GPIO_Mode=GPIO_Mode_Out_PP;   //�������ģʽ
	GPIO_InitSture.GPIO_Pin=Trig2;                //��PE4��Trig����
	GPIO_InitSture.GPIO_Speed=GPIO_Speed_50MHz;  
	GPIO_Init(GPIOF,&GPIO_InitSture);
	
	GPIO_InitSture.GPIO_Mode=GPIO_Mode_IPD;      //������ģʽ
	GPIO_InitSture.GPIO_Pin=Echo2;                //��PE6��Echo����
	GPIO_InitSture.GPIO_Speed=GPIO_Speed_50MHz;  
	GPIO_Init(GPIOF,&GPIO_InitSture);
	
	//�жϺ�6�˿�ӳ��һ��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource4);
	
	//�ⲿ�ж�����
	EXTI_InitSture.EXTI_Line=EXTI_Line4;
	EXTI_InitSture.EXTI_LineCmd=ENABLE;
	EXTI_InitSture.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitSture.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitSture);
	
	
	//�ж����ȼ�����
	NVIC_InitSture.NVIC_IRQChannel=EXTI4_IRQn;
	NVIC_InitSture.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitSture.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitSture.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_InitSture);
}

void EXTI4_IRQHandler(void)
{
	
	delay_us(20);
	
	
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
	{
		TIM_SetCounter(TIM3,0);
		TIM_Cmd(TIM3,ENABLE);
		
		while(GPIO_ReadInputDataBit(GPIOF,Echo2));  //�ȴ��͵�ƽ
		
		TIM_Cmd(TIM3,DISABLE);
		
		Distance2=TIM_GetCounter(TIM3)*340/200.0;
		
		if(Distance2>0)
		{
			printf("Distance2:%f cm\r\n",Distance2);
			Distance_zhen2=Distance2;
			LCD_ShowxNum(132,130,Distance_zhen2,3,16,0);
			Distance2-=Distance_zhen2;
			Distance2*=1000;
			LCD_ShowxNum(164,130,Distance2,3,16,0);


			
		}
			
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

void Wave2_SRD_Strat(void)
{
	GPIO_SetBits(GPIOF,Trig2);   //��Trig����Ϊ�ߵ�ƽ
	delay_us(90000);               //��������10us����������������ģ�鹤��
	GPIO_ResetBits(GPIOF,Trig2); 
	
}


void Wave1_SRD_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitSture;
	EXTI_InitTypeDef  EXTI_InitSture;
	NVIC_InitTypeDef  NVIC_InitSture;
	//����ⲿ�жϵĻ���һ��ʹ��AFIO���ù���
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOE,ENABLE);
	
	
	//����IO�˿�
	GPIO_InitSture.GPIO_Mode=GPIO_Mode_Out_PP;   //�������ģʽ
	GPIO_InitSture.GPIO_Pin=Trig1;                //��PE4��Trig����
	GPIO_InitSture.GPIO_Speed=GPIO_Speed_50MHz;  
	GPIO_Init(GPIOE,&GPIO_InitSture);
	
	GPIO_InitSture.GPIO_Mode=GPIO_Mode_IPD;      //������ģʽ
	GPIO_InitSture.GPIO_Pin=Echo1;                //��PE6��Echo����
	GPIO_InitSture.GPIO_Speed=GPIO_Speed_50MHz;  
	GPIO_Init(GPIOE,&GPIO_InitSture);
	
	//�жϺ�6�˿�ӳ��һ��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
	
	//�ⲿ�ж�����
	EXTI_InitSture.EXTI_Line=EXTI_Line3;
	EXTI_InitSture.EXTI_LineCmd=ENABLE;
	EXTI_InitSture.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitSture.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitSture);
	
	
	//�ж����ȼ�����
	NVIC_InitSture.NVIC_IRQChannel=EXTI3_IRQn;
	NVIC_InitSture.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitSture.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitSture.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_InitSture);
}

void EXTI3_IRQHandler(void)
{
	
	delay_us(20);
	
	
	if(EXTI_GetITStatus(EXTI_Line3)!=RESET)
	{
		TIM_SetCounter(TIM3,0);
		TIM_Cmd(TIM3,ENABLE);
		
		while(GPIO_ReadInputDataBit(GPIOE,Echo1));  //�ȴ��͵�ƽ
		
		TIM_Cmd(TIM3,DISABLE);
		
		Distance1=TIM_GetCounter(TIM3)*340/200.0;
		
		if(Distance1>0)
		{
			printf("Distance1:%f cm\r\n",Distance1);
			Distance_zhen1=Distance1;
			LCD_ShowxNum(132,110,Distance_zhen1,3,16,0);
			Distance1-=Distance_zhen1;
			Distance1*=1000;
			LCD_ShowxNum(164,110,Distance1,3,16,0);


			
		}
			
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}

void Wave1_SRD_Strat(void)
{
	GPIO_SetBits(GPIOE,Trig1);   //��Trig����Ϊ�ߵ�ƽ
	delay_us(90000);               //��������10us����������������ģ�鹤��
	GPIO_ResetBits(GPIOE,Trig1); 
	
}





void Wave3_SRD_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitSture;
	EXTI_InitTypeDef  EXTI_InitSture;
	NVIC_InitTypeDef  NVIC_InitSture;
	//����ⲿ�жϵĻ���һ��ʹ��AFIO���ù���
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOF,ENABLE);
	
	
	//����IO�˿�
	GPIO_InitSture.GPIO_Mode=GPIO_Mode_Out_PP;   //�������ģʽ
	GPIO_InitSture.GPIO_Pin=Trig3;                //��PE4��Trig����
	GPIO_InitSture.GPIO_Speed=GPIO_Speed_50MHz;  
	GPIO_Init(GPIOF,&GPIO_InitSture);
	
	GPIO_InitSture.GPIO_Mode=GPIO_Mode_IPD;      //������ģʽ
	GPIO_InitSture.GPIO_Pin=Echo3;                //��PE6��Echo����
	GPIO_InitSture.GPIO_Speed=GPIO_Speed_50MHz;  
	GPIO_Init(GPIOF,&GPIO_InitSture);
	
	//�жϺ�6�˿�ӳ��һ��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource10);
	
	//�ⲿ�ж�����
	EXTI_InitSture.EXTI_Line=EXTI_Line10;
	EXTI_InitSture.EXTI_LineCmd=ENABLE;
	EXTI_InitSture.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitSture.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitSture);
	
	
	//�ж����ȼ�����
	NVIC_InitSture.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_InitSture.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitSture.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitSture.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_InitSture);
}

void EXTI15_10_IRQHandler(void)
{
	
	delay_us(20);
	
	
	if(EXTI_GetITStatus(EXTI_Line10)!=RESET)
	{
		TIM_SetCounter(TIM3,0);
		TIM_Cmd(TIM3,ENABLE);
		
		while(GPIO_ReadInputDataBit(GPIOF,Echo3));  //�ȴ��͵�ƽ
		
		TIM_Cmd(TIM3,DISABLE);
		
		Distance3=TIM_GetCounter(TIM3)*340/200.0;
		
		if(Distance3>0)
		{
			printf("Distance3:%f cm\r\n",Distance3);
			Distance_zhen3=Distance3;
			LCD_ShowxNum(132,150,Distance_zhen3,3,16,0);
			Distance3-=Distance_zhen3;
			Distance3*=1000;
			LCD_ShowxNum(164,150,Distance3,3,16,0);


			
		}
			
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
	
	
}

void Wave3_SRD_Strat(void)
{
	GPIO_SetBits(GPIOF,Trig3);   //��Trig����Ϊ�ߵ�ƽ
	delay_us(90000);               //��������10us����������������ģ�鹤��
	GPIO_ResetBits(GPIOF,Trig3); 
	
}  


