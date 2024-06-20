#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "usart.h"
#include "sys.h"
#include "includes.h"
#include "adc.h"
#include "cal.h"

//START任务
#define START_TASK_PRIO 10   //设置start任务优先级
#define START_STK_SIZE 64     //设置start任务堆栈大小
OS_STK START_TASK_STK[START_STK_SIZE];   //定义一个数组，作为start任务堆栈空间
void start_task(void *pdata);     //声明start任务

//ADC0任务
#define ADC0_TASK_PRIO 7   
#define ADC0_STK_SIZE 128     
OS_STK ADC0_TASK_STK[ADC0_STK_SIZE ];  
void adc0_task(void *pdata);     

//APP1任务
#define APP1_TASK_PRIO 6   
#define APP1_STK_SIZE 64    
OS_STK APP1_TASK_STK[APP1_STK_SIZE ];  
void app1_task(void *pdata);    

//APP2任务
#define APP2_TASK_PRIO 3   
#define APP2_STK_SIZE 64   
OS_STK APP2_TASK_STK[APP2_STK_SIZE ];  
void app2_task(void *pdata);    

//APP3任务
#define APP3_TASK_PRIO 5   
#define APP3_STK_SIZE 64     
OS_STK APP3_TASK_STK[APP3_STK_SIZE ];  
void app3_task(void *pdata);    

OS_EVENT * mbox_key;   
OS_EVENT * mbox_scanf; 
INT8U err;

//开始任务
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
  pdata=pdata;
	mbox_key=OSMboxCreate((void *)0);//创建消息邮箱
	mbox_scanf = OSMboxCreate((void *)0);
  OS_ENTER_CRITICAL();   //进入临界段（关中断）
	OSTaskCreate(adc0_task,(void *)0,(OS_STK *)&ADC0_TASK_STK[ADC0_STK_SIZE-1 ],ADC0_TASK_PRIO);
	OSTaskCreate(app1_task,(void *)0,(OS_STK *)&APP1_TASK_STK[APP1_STK_SIZE-1 ],APP1_TASK_PRIO);
  OSTaskCreate(app2_task,(void *)0,(OS_STK *)&APP2_TASK_STK[APP2_STK_SIZE-1 ],APP2_TASK_PRIO);
  OSTaskCreate(app3_task,(void *)0,(OS_STK *)&APP3_TASK_STK[APP3_STK_SIZE-1 ],APP3_TASK_PRIO);
  OSTaskSuspend(START_TASK_PRIO);
  OS_EXIT_CRITICAL();   //退出临界段（开中断）
}

/*ADC0任务:获取adcx的值，并将其作为消息用邮箱mbox_key发给APP1任务*/
void adc0_task(void *pdata)
{
	//adc
	u16 adcx;
	pdata=pdata;
	while(1)
	{
		//电压值
		adcx=Get_Adc_Average(ADC_Channel_1,10);
		if(adcx)
			OSMboxPost(mbox_key,(void *)adcx);//发送消息
		delay_ms(10);
	}
}


//app1任务结合板载的电位器和STM32的AD转换功能，
//测量电位器两端的电压变化，并通过LED的数量提示AD转化的电压值的大小，电压越大点亮的LED越多。
void app1_task(void *pdata)
{
	//adc
	u16 adcx;
	u16 adc;
	pdata=pdata;
	while(1)
	{
		//接收adc值
		adc=(u16)OSMboxPend(mbox_key,10,&err);
		//printf("\r\n OSMboxPend adc = %d \r\n", adc);
		switch(adc/512) {
			case 0: LED_1 = 0;LED_2 = 1;LED_3 = 1;LED_4 = 1;LED_5 = 1;LED_6 = 1;LED_7 = 1;LED_8 = 1;break;
			case 1: LED_1 = 0;LED_2 = 0;LED_3 = 1;LED_4 = 1;LED_5 = 1;LED_6 = 1;LED_7 = 1;LED_8 = 1;break;
			case 2: LED_1 = 0;LED_2 = 0;LED_3 = 0;LED_4 = 1;LED_5 = 1;LED_6 = 1;LED_7 = 1;LED_8 = 1;break;
			case 3: LED_1 = 0;LED_2 = 0;LED_3 = 0;LED_4 = 0;LED_5 = 1;LED_6 = 1;LED_7 = 1;LED_8 = 1;break;
			case 4: LED_1 = 0;LED_2 = 0;LED_3 = 0;LED_4 = 0;LED_5 = 0;LED_6 = 1;LED_7 = 1;LED_8 = 1;break;
			case 5: LED_1 = 0;LED_2 = 0;LED_3 = 0;LED_4 = 0;LED_5 = 0;LED_6 = 0;LED_7 = 1;LED_8 = 1;break;
			case 6: LED_1 = 0;LED_2 = 0;LED_3 = 0;LED_4 = 0;LED_5 = 0;LED_6 = 0;LED_7 = 0;LED_8 = 1;break;
			case 7: LED_1 = 0;LED_2 = 0;LED_3 = 0;LED_4 = 0;LED_5 = 0;LED_6 = 0;LED_7 = 0;LED_8 = 0;break;
		}
		//LED低电平点亮，高电平熄灭
		delay_ms(500);
	}
}

/*APP2任务:按键扫描（矩阵键盘）通过串口输出哪一个按键被按下了*/
void app2_task(void *pdata)
{
	u8 key;
	while(1)
	{
		KEY_Scan(&key);
		//printf("\r\nFLAG = %d, key = %d\r\n", FLAG, key);
		if (FLAG == 1) {//有按键按下
			OSMboxPost(mbox_scanf,(void *)key);//发送消息
			//FLAG = 0;//读取下一个按键
			switch (key) {
					case 1: printf("\r\nK1 test\r\n"); break;
					case 2: printf("\r\nK2 test\r\n"); break;
					case 3: printf("\r\nK3 test\r\n"); break;
					case 4: printf("\r\nK4 test\r\n"); break;
					case 5: printf("\r\nK5 test\r\n"); break;
					case 6: printf("\r\nK6 test\r\n"); break;
					case 7: printf("\r\nK7 test\r\n"); break;
					case 8: printf("\r\nK8  test\r\n"); break;
					case 9: printf("\r\nK9  test\n\r"); break;
					case 10: printf("\r\nK10  test\r\n"); break;
					case 11: printf("\r\nK11 test\r\n"); break;
					case 12: printf("\r\nK12  test\r\n"); break;
					case 13: printf("\r\nK13  test\r\n"); break;
					case 14: printf("\r\nK14  test\r\n"); break;
					case 15: printf("\r\nK15  test\r\n"); break;
					case 16: printf("\r\nK16  test\r\n"); break;
			}
		}
		delay_ms(50);//任务1的挂起时间
	}
}


void app3_task(void *pdata)
{
	int Total = 0;
	int Num_1=0,Num_2=0;
	u8 key_flag;     //按键标志
	int Oper = 0; //运算符
	int t=0;
	int key_val;

   while(1)
   {
		 //扫描键盘，读取键值
		 key_flag = (u8)OSMboxPend(mbox_scanf,10,&err);
 if(FLAG) 
     {
		switch ( key_flag) {
					case 1:key_val=1; printf("\r\n1\r\n"); break;
					case 2:key_val=2; printf("\r\n2\r\n");  break;
					case 3:key_val=3;printf("\r\n3\r\n"); break;
					case 4:key_val=4;printf("\r\n4\r\n"); break;
					case 5:key_val=5;printf("\r\n5\r\n"); break;
					case 6:key_val=6;printf("\r\n6\r\n"); break;
					case 7:key_val=7;printf("\r\n7\r\n"); break;
					case 8:key_val=8; printf("\r\n8\r\n");break;
					case 9:key_val=9;printf("\r\n9\r\n");break;
					case 10:key_val=0; printf("\r\n0\r\n");  break;
					case 11:key_val=11; printf("\r\n+\r\n"); break;
					case 12:key_val=12;  printf("\r\n-\r\n"); break;
					case 13: key_val=13; printf("\r\n*\r\n"); break;
					case 14: key_val=14; printf("\r\n/\r\n"); break;
					case 15:key_val=15;  printf("\r\n=\r\n"); break;
					case 16:key_val=16;  printf("\r\nclean\r\n"); break;
			}
			
		if(key_val<10){
			if(t==0){
					 Num_1= key_val;
			  }
				 
		  else{
					 Num_2= key_val;
	
			  }
		  }
			
			if(key_val>10&&key_val!=15){
					switch (key_val) {	
				  case 11: Oper=1;break;//+
					case 12: Oper=2; break;//-
					case 13:Oper=3;break;//*
					case 14: Oper=4;break;///
					case 16: Oper=5; break;//clean
			     }
					t=1;
				}
				 
			if(key_val==15){
				if(Oper==1){
					Total=Num_1+Num_2;
					printf("\r\n=%d\r\n",Total);
				}
				if(Oper==2){
					Total=Num_1-Num_2;
					printf("\r\n=%d\r\n",Total);
				}
				if(Oper==3){
					Total=Num_1*Num_2;
					printf("\r\n=%d\r\n",Total);
				}
				if(Oper==4){
					if(Num_2!=0){
						Total=Num_1/Num_2;
					printf("\r\n=%d\r\n",Total);
				}
					else
						printf("\r\nerror!\r\n");			
			  }
				if(Oper==5){
						Num_1=Num_2=Total=0;
	         printf("\r\ncleaned\r\n");				
			  }
		  t=0;
       }
		}
		 	FLAG = 0;
			delay_ms(50);

	}
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
	LED_Init();
	KEY_Init();
	Adc_Init();
	uart_init(9600);
	printf("key arrange\r\n");
	printf("1 2 3 4 \r\n");
	printf("5 6 7 8 \r\n");
	printf("9 0 + - \r\n");
	printf("* / = cleanr\n");
	OSInit();
	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO);
	OSStart();
}
