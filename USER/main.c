#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "exti.h"
#include "beep.h"
#include "lcd.h"
#include "timer.h"
#include "wave.h"
#include "wai_bian.h"
#include "wdg.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "adc.h"
#include "lsens.h"



#define START_TASK_PRIO		1   //�������ȼ�
#define START_STK_SIZE 		128   //�����ջ��С	
TaskHandle_t StartTask_Handler;   //������
void start_task(void *pvParameters);  //������

 

#define desktop_TASK_PRIO		2   //�������ȼ�
#define desktop_STK_SIZE 		128   //�����ջ��С	
TaskHandle_t desktop_Handler;   //������
void desktop_task(void *pvParameters);  //������

#define light_TASK_PRIO		3   //�������ȼ�
#define light_STK_SIZE 		128   //�����ջ��С	
TaskHandle_t light_Handler;   //������
void light_task(void *pvParameters);  //������

#define output_TASK_PRIO		4   //�������ȼ�
#define output_STK_SIZE 		128   //�����ջ��С	
TaskHandle_t output_Handler;   //������
void output_task(void *pvParameters);  //������

//��Ϣ����

#define Dpart_Q_NUM    1   	//�������ݵ���Ϣ���е����� 

QueueHandle_t Dpart_Queue;	//��Ϣ���о��


int main(void)
{
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	BEEP_Init();         	//��ʼ���������˿�
	KEY_Init();         	//��ʼ���밴�����ӵ�Ӳ���ӿ�
	LCD_Init();	
	//EXTIX_Init();		 	//�ⲿ�жϳ�ʼ��
	//LED0=0;					//����LED0
	Tim3_Init(4999,7199);
	Wave_SRD_Init();
	Wave1_SRD_Init();
	Wave2_SRD_Init();
	Wave3_SRD_Init();
	Lsens_Init(); 			//��ʼ������������
	
  TIM5_Cap_Init(0XFFFF,72-1);  
	TIM2_PWM_Init(899,0);	
	TIM4_PWM_Init(899,0);	
//	TIM3_PWM_Init(899,0);	
	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(60,50,200,16,16,"?_?");	
	
	
	//��ʾ��ʾ��Ϣ
	POINT_COLOR=BLUE;//��������Ϊ��ɫ
	LCD_ShowString(60,70,200,16,16,"Distance:000.000cm");	      
	LCD_ShowString(60,90,200,16,16,"Badtime :0      s");	
	LCD_ShowString(60,110,200,16,16,"Distanc1:000.000cm");	
	LCD_ShowString(60,130,200,16,16,"Distanc2:000.000cm");	
	LCD_ShowString(60,150,200,16,16,"Distanc3:000.000cm");	
	LCD_ShowString(60,170,200,16,16,"LSENS_VAL:");
	
		//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}



void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
	
	//������Ϣ����
   
    Dpart_Queue=xQueueCreate(Dpart_Q_NUM,sizeof(u8));        //������ϢDpart
	
 
    //����desktop����
    xTaskCreate((TaskFunction_t )desktop_task,             
                (const char*    )"desktop_task",           
                (uint16_t       )desktop_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )desktop_TASK_PRIO,        
                (TaskHandle_t*  )&desktop_Handler);   
    //����light����
    xTaskCreate((TaskFunction_t )light_task,     
                (const char*    )"light_task",   
                (uint16_t       )light_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )light_TASK_PRIO,
                (TaskHandle_t*  )&light_Handler); 
								
		//����output����
    xTaskCreate((TaskFunction_t )output_task,     
                (const char*    )"output_task",   
                (uint16_t       )output_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )output_TASK_PRIO,
                (TaskHandle_t*  )&output_Handler); 
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}
	   

void desktop_task (void *pvParameters)
{
	u8 flag;
	int Juli;
	int Juli1;
	int Juli2;
	int Juli3;
	BaseType_t err;
	while(1)
	{	    
		    
		    Juli=1;
		    Juli1=1;
		    Juli2=1;
		    Juli3=1;
		    Wave_SRD_Strat();
      	Wave1_SRD_Strat();
        Wave2_SRD_Strat();
        Wave3_SRD_Strat();
 		    if((Distance_zhen<7)&&(Distance_zhen>3))
				{Juli=0;}
		    if((Distance_zhen1<41)&&(Distance_zhen1>30))
				{Juli1=0;}
				if((Distance_zhen2<41)&&(Distance_zhen2>30))
				{Juli2=0;}
				if((Distance_zhen3<41)&&(Distance_zhen3>30))
				{Juli3=0;}
		     
			   
				if((Juli1+Juli2+Juli3+Juli)==0)	
		     flag=0;
        else 
		     flag=1;   
		
		     err=xQueueSend(Dpart_Queue,&flag,10);	
				 if(err==errQUEUE_FULL) //?????
 {
 printf("δ�ɹ�");}
	 else{
	 printf("�ɹ�%d\r\n",flag);
 }
													
			
	}
}

void light_task (void *pvParameters)
{
		u8 adcx; 

	while(1)
	{
		adcx=Lsens_Get_Val();
		LCD_ShowxNum(150,170,adcx,3,16,0);//��ʾADC��ֵ 
		if(adcx<10)
			TIM_SetCompare1(TIM4,300);
		if((adcx>10)&&(adcx<30))
			TIM_SetCompare1(TIM4,800);
		if(adcx>30)
			TIM_SetCompare1(TIM4,899);
		
		delay_ms(250);	
	}
}


void output_task (void *pvParameters)
{
	extern u8 TIM5CH1_CAPTURE_STA; 
  extern u16 TIM5CH1_CAPTURE_VAL; 
 
	u8 Dpart=0;
	int k;
	int c;
	int flag;
	int flag2;
 	int num=0;     //��us
float nums=0;  //��s
  
	u32 temp=0; 
	while(1)
	{	    
		  
   
			c=0;                        //������0
			 
		 
			for(k=0;k<10;k++){ 				//0.1����һ�Σ��������50�δﵽ5��ĳ�����أ�һ���ָ������˾�break����
				
				c++;    //��¼ѭ���������Ա�ָ������󣬼�����ʱ��
			  flag=0;
				xQueueReceive(Dpart_Queue,&Dpart,portMAX_DELAY);
				

			   
				if(Dpart==0){						 //�ٴμ��
				 flag=1;        //��־λ
			   break;}}   
			  if(flag==0) {        //���˲�������������
				 TIM_SetCompare1(TIM2,0);     //��ʼ��ʱ
				 num+=4000000;   //��ʱ���4��
			   TIM_SetCompare3(TIM4,300);    //��������
			   flag2=1; }                        
		     else if(flag==1)  
						{ 
							if(flag2==1)            //��ʾ�Ѿ����˲���
			           { num+=c*400000;
									 flag2=0;
									 TIM_SetCompare1(TIM2,1000);      //ֹͣ��ʱ
			             TIM_SetCompare3(TIM4,899);     //����������
			             
			             
												
		
															 }
														 }
						                     if(TIM5CH1_CAPTURE_STA&0X80)//�ɹ�������һ��������
		                            {temp=TIM5CH1_CAPTURE_STA&0X3F;
			                           temp*=65536;//���ʱ���ܺ�
			                           temp+=TIM5CH1_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
			                           TIM5CH1_CAPTURE_STA=0;//������һ�β���
						                     num+=temp-1000000;
						                     nums=num/1000000;
						                     printf("HIGH:%f s\r\n",nums);//��ӡ�ܵĸߵ�ƽʱ��
						                     LCD_ShowxNum(132,90,nums,5,16,0);}}
}