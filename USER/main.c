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



#define START_TASK_PRIO		1   //任务优先级
#define START_STK_SIZE 		128   //任务堆栈大小	
TaskHandle_t StartTask_Handler;   //任务句柄
void start_task(void *pvParameters);  //任务函数

 

#define desktop_TASK_PRIO		2   //任务优先级
#define desktop_STK_SIZE 		128   //任务堆栈大小	
TaskHandle_t desktop_Handler;   //任务句柄
void desktop_task(void *pvParameters);  //任务函数

#define light_TASK_PRIO		3   //任务优先级
#define light_STK_SIZE 		128   //任务堆栈大小	
TaskHandle_t light_Handler;   //任务句柄
void light_task(void *pvParameters);  //任务函数

#define output_TASK_PRIO		4   //任务优先级
#define output_STK_SIZE 		128   //任务堆栈大小	
TaskHandle_t output_Handler;   //任务句柄
void output_task(void *pvParameters);  //任务函数

//消息队列

#define Dpart_Q_NUM    1   	//发送数据的消息队列的数量 

QueueHandle_t Dpart_Queue;	//信息队列句柄


int main(void)
{
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();		  		//初始化与LED连接的硬件接口
	BEEP_Init();         	//初始化蜂鸣器端口
	KEY_Init();         	//初始化与按键连接的硬件接口
	LCD_Init();	
	//EXTIX_Init();		 	//外部中断初始化
	//LED0=0;					//点亮LED0
	Tim3_Init(4999,7199);
	Wave_SRD_Init();
	Wave1_SRD_Init();
	Wave2_SRD_Init();
	Wave3_SRD_Init();
	Lsens_Init(); 			//初始化光敏传感器
	
  TIM5_Cap_Init(0XFFFF,72-1);  
	TIM2_PWM_Init(899,0);	
	TIM4_PWM_Init(899,0);	
//	TIM3_PWM_Init(899,0);	
	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(60,50,200,16,16,"?_?");	
	
	
	//显示提示信息
	POINT_COLOR=BLUE;//设置字体为蓝色
	LCD_ShowString(60,70,200,16,16,"Distance:000.000cm");	      
	LCD_ShowString(60,90,200,16,16,"Badtime :0      s");	
	LCD_ShowString(60,110,200,16,16,"Distanc1:000.000cm");	
	LCD_ShowString(60,130,200,16,16,"Distanc2:000.000cm");	
	LCD_ShowString(60,150,200,16,16,"Distanc3:000.000cm");	
	LCD_ShowString(60,170,200,16,16,"LSENS_VAL:");
	
		//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}



void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
	
	//创建消息队列
   
    Dpart_Queue=xQueueCreate(Dpart_Q_NUM,sizeof(u8));        //创建消息Dpart
	
 
    //创建desktop任务
    xTaskCreate((TaskFunction_t )desktop_task,             
                (const char*    )"desktop_task",           
                (uint16_t       )desktop_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )desktop_TASK_PRIO,        
                (TaskHandle_t*  )&desktop_Handler);   
    //创建light任务
    xTaskCreate((TaskFunction_t )light_task,     
                (const char*    )"light_task",   
                (uint16_t       )light_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )light_TASK_PRIO,
                (TaskHandle_t*  )&light_Handler); 
								
		//创建output任务
    xTaskCreate((TaskFunction_t )output_task,     
                (const char*    )"output_task",   
                (uint16_t       )output_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )output_TASK_PRIO,
                (TaskHandle_t*  )&output_Handler); 
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
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
 printf("未成功");}
	 else{
	 printf("成功%d\r\n",flag);
 }
													
			
	}
}

void light_task (void *pvParameters)
{
		u8 adcx; 

	while(1)
	{
		adcx=Lsens_Get_Val();
		LCD_ShowxNum(150,170,adcx,3,16,0);//显示ADC的值 
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
 	int num=0;     //总us
float nums=0;  //总s
  
	u32 temp=0; 
	while(1)
	{	    
		  
   
			c=0;                        //次数置0
			 
		 
			for(k=0;k<10;k++){ 				//0.1秒检测一次，连续检测50次达到5秒的持续监控，一旦恢复好坐姿就break跳出
				
				c++;    //记录循环次数，以便恢复正常后，计算总时间
			  flag=0;
				xQueueReceive(Dpart_Queue,&Dpart,portMAX_DELAY);
				

			   
				if(Dpart==0){						 //再次检测
				 flag=1;        //标志位
			   break;}}   
			  if(flag==0) {        //坐姿不正超过五秒钟
				 TIM_SetCompare1(TIM2,0);     //开始计时
				 num+=4000000;   //总时间加4秒
			   TIM_SetCompare3(TIM4,300);    //蜂鸣器响
			   flag2=1; }                        
		     else if(flag==1)  
						{ 
							if(flag2==1)            //表示已经坐姿不良
			           { num+=c*400000;
									 flag2=0;
									 TIM_SetCompare1(TIM2,1000);      //停止计时
			             TIM_SetCompare3(TIM4,899);     //蜂鸣器不响
			             
			             
												
		
															 }
														 }
						                     if(TIM5CH1_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
		                            {temp=TIM5CH1_CAPTURE_STA&0X3F;
			                           temp*=65536;//溢出时间总和
			                           temp+=TIM5CH1_CAPTURE_VAL;//得到总的高电平时间
			                           TIM5CH1_CAPTURE_STA=0;//开启下一次捕获
						                     num+=temp-1000000;
						                     nums=num/1000000;
						                     printf("HIGH:%f s\r\n",nums);//打印总的高点平时间
						                     LCD_ShowxNum(132,90,nums,5,16,0);}}
}