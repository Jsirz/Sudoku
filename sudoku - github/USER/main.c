#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "usart.h"
#include "sys.h"
#include "includes.h"
#include "Menu.h"
#include "Sudoku.h"
#include "Lcd_Driver.h"
#include "lcd_init.h"
#include "lcd.h"
#include "adc.h"
#include "stdlib.h"

//START任务：创建LED任务和name_task任务
#define START_TASK_PRIO 20   //设置start任务优先级
#define START_STK_SIZE 512     //设置start任务堆栈大小
OS_STK START_TASK_STK[START_STK_SIZE];   //定义一个数组，作为start任务堆栈空间
void start_task(void *pdata);     //声明start任务

//LED任务
#define LED_TASK_PRIO 14
#define LED_STK_SIZE 128     
OS_STK LED_TASK_STK[LED_STK_SIZE ];  
void led_task(void *pdata);     

//name任务：展示姓名和学号，检测按键，按下后进入game_menu任务
#define Name_TASK_PRIO 17   
#define Name_STK_SIZE 1024    
OS_STK Name_TASK_STK[Name_STK_SIZE ];  
void name_task(void *pdata);   

//game_menu任务:展示游戏信息
#define Game_menu_TASK_PRIO 16   
#define Game_menu_STK_SIZE 64     
OS_STK Game_menu_TASK_STK[Game_menu_STK_SIZE ];  
void game_menu_task(void *pdata);    

//game任务:展示游戏界面
#define Game_TASK_PRIO 18  
#define Game_STK_SIZE 512     
OS_STK Game_TASK_STK[Game_STK_SIZE ];  
void game_task(void *pdata); 

//time任务:实现计时功能
#define Time_TASK_PRIO 19  
#define Time_STK_SIZE 64     
OS_STK Time_TASK_STK[Game_STK_SIZE ];  
void time_task(void *pdata); 


OS_EVENT * mbox_key;   
OS_EVENT * mbox_scanf; 
uint16_t key_f1;//扫描返回接收值；
int count=0;//计时
int overtime=300;//限时时间,默认300秒
int sudoku[81];
int answer_sudoku[81];

//开始任务
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
  pdata=pdata;
	mbox_key=OSMboxCreate((void *)0);//创建消息邮箱
	mbox_scanf = OSMboxCreate((void *)0);
  OS_ENTER_CRITICAL();   //进入临界段（关中断）
	OSTaskCreate(led_task,(void *)0,(OS_STK *)&LED_TASK_STK[LED_STK_SIZE-1 ],LED_TASK_PRIO);
	OSTaskCreate(name_task,(void *)0,(OS_STK *)&Name_TASK_STK[Name_STK_SIZE-1 ],Name_TASK_PRIO);
  OSTaskDel(START_TASK_PRIO);
  OS_EXIT_CRITICAL();   //退出临界段（开中断）
}

/*LED任务:红绿蓝灯轮流闪烁*/
void led_task(void *pdata)
{

	LED_B=1;
	LED_R=1;
	LED_G=1;
	while(1)
	{
		
		delay_ms(500);
		LED_B=1;
		LED_R=0;
		LED_G=1;
		delay_ms(500);
		LED_B=1;
		LED_R=1;
		LED_G=0;
		delay_ms(500);
		LED_B=0;
		LED_R=1;
		LED_G=1;

		//LED低电平点亮，高电平熄灭
	}
	
}

//展示姓名和学号
void name_task(void *pdata)
{
	name_menu();
	while(1)
		{
			key_f1=key_Scan();
			if(key_f1==20)
			{
				Lcd_Clear(GRAY0);
				OSTaskCreate(game_menu_task,(void *)0,(OS_STK *)&Game_menu_TASK_STK[Game_menu_STK_SIZE-1 ],Game_menu_TASK_PRIO);
			}
			else if(key_f1==14)
			{
				while(1)
				{
					key_f1=key_Scan();
					if(key_f1==14)
					{
						while(1)
						{
							key_f1=key_Scan();
							if(key_f1==16)
							{
								show_eggs();
							}
							else if(key_f1==0)
							{}
							else
								break;
						}
					}
					else if(key_f1==0)
					{}
					else
							break;
				}
			}
			
		}
}
//实现计时功能
void time_task(void *pdata)
{
	count=0;
	while(1)
	{
		delay_ms(900);
		count++;
		if(overtime<count)
		{
			show_timeover(count);
			delay_ms(3000);
			OSTaskCreate(game_menu_task,(void *)0,(OS_STK *)&Game_menu_TASK_STK[Game_menu_STK_SIZE-1 ],Game_menu_TASK_PRIO);
			OSTaskDel(Game_TASK_PRIO);
			OSTaskDel(Time_TASK_PRIO);
		}
	}
}


//游戏界面
void game_task(void *pdata)
{
	int p=24,a=0;
	sudoku[p]=0;
	Display_sudoko(sudoku,answer_sudoku);
	show_sudoku(sudoku,p);
	while(1)
		{
			key_f1=key_Scan();
			switch(key_f1)
			{
				case 20://检查答案
				{
					a=is_correct(sudoku,answer_sudoku);
					if(a)
					{
						show_victory(count);
						show_score(count);
						while(1)
						{
							key_f1=key_Scan();
							if(key_f1==20)
							{
								OSTaskCreate(game_menu_task,(void *)0,(OS_STK *)&Game_menu_TASK_STK[Game_menu_STK_SIZE-1 ],Game_menu_TASK_PRIO);
								OSTaskDel(Game_TASK_PRIO);
								OSTaskDel(Time_TASK_PRIO);
								break;
							}
						}
						
					}
					else
						{
							OSTaskSuspend(Time_TASK_PRIO);
							show_false();
							OSTaskResume(Time_TASK_PRIO);
							show_sudoku(sudoku,p);
						}
				}break;
				case 19://返回上一层界面
					{
						OSTaskCreate(game_menu_task,(void *)0,(OS_STK *)&Game_menu_TASK_STK[Game_menu_STK_SIZE-1 ],Game_menu_TASK_PRIO);
						OSTaskDel(Game_TASK_PRIO);
						OSTaskDel(Time_TASK_PRIO);
					}break;
				case 18://查看当前已游戏时间
				{
					OSTaskSuspend(Time_TASK_PRIO);
					show_time(count);
					while(1)
					{
						key_f1=key_Scan();
						if(key_f1==18)
						{
							OSTaskResume(Time_TASK_PRIO);
							break;
						}
					}
					show_sudoku(sudoku,p);
				}break;
				case 17://查看答案
				{
					show_sudoku(answer_sudoku,1);
					delay_ms(3000);
					show_sudoku(sudoku,24);
				}break;
				case 1://上选
				{
					p-=9;
					if(p>0)
					{
						show_sudoku(sudoku,p);
					}
					else
						p+=9;
				}break;
				case 2://下选
				{
					p+=9;
					if(p<82)
					{
						show_sudoku(sudoku,p);
					}
					else
						p-=9;
					
				}break;
				case 3://左选
				{
					p-=1;
					if(p>0)
					{
						show_sudoku(sudoku,p);
					}
					else
						p+=1;
				}break;
				case 4://右选
				{
					p+=1;
					if(p<82)
					{
						show_sudoku(sudoku,p);
					}
					else
						p-=1;
					
				}break;
				case 5://将选中的位置数字改为1
				{
					sudoku[p]=1;
					show_sudoku(sudoku,p);
				}break;
				case 6://将选中的位置数字改为2
				{
					sudoku[p]=2;
					show_sudoku(sudoku,p);
				}break;
				case 7://将选中的位置数字改为3
				{
					sudoku[p]=3;
					show_sudoku(sudoku,p);
				}break;
				case 8://将选中的位置数字改为4
				{
					sudoku[p]=4;
					show_sudoku(sudoku,p);
				}break;
				case 9://将选中的位置数字改为5
				{
					sudoku[p]=5;
					show_sudoku(sudoku,p);
				}break;
				case 10://将选中的位置数字改为6
				{
					sudoku[p]=6;
					show_sudoku(sudoku,p);
				}break;
				case 11://将选中的位置数字改为7
				{
					sudoku[p]=7;
					show_sudoku(sudoku,p);
				}break;
				case 12://将选中的位置数字改为8
				{
					sudoku[p]=8;
					show_sudoku(sudoku,p);
				}break;
				case 13://将选中的位置数字改为9
				{
					sudoku[p]=9;
					show_sudoku(sudoku,p);
				}break;
				case 15://测试游戏胜利画面
				{
					for(p=0;p<81;p++)
					{
						sudoku[p]=0;
						answer_sudoku[p]=0;
					}
					show_sudoku(sudoku,p);
				}break;
			}
			
		}
}



//设置游戏限时时长，默认300s
void set_time()
{
	show_time(overtime);
	while(1)
	{
		key_f1=key_Scan();
		if(key_f1==1)
		{
			if(overtime<989)
			overtime+=10;
			show_time(overtime);
		}
		else if(key_f1==2)
		{
			if(overtime>10)
			overtime-=10;
			show_time(overtime);
		}
		else if(key_f1==19)
			break;
	}
}

//展示游戏菜单界面
void game_menu_task(void *pdata)
{
	OSTaskSuspend(Name_TASK_PRIO);
	game_menu();
	while(1)
		{
			key_f1=key_Scan();
			if(key_f1==20)
			{
					OSTaskCreate(game_task,(void *)0,(OS_STK *)&Game_TASK_STK[Game_STK_SIZE-1 ],Game_TASK_PRIO);
					OSTaskCreate(time_task,(void *)0,(OS_STK *)&Time_TASK_STK[Time_STK_SIZE-1 ],Time_TASK_PRIO);
					OSTaskDel(Game_menu_TASK_PRIO);
			}
			else if(key_f1==19)
			{
				count=0;
				set_time();
				game_menu();
			}
		}
}

int main(void)
{
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断相关
	delay_init();
	LED_Init();
	key_Init();
	uart_init(9600);
	Adc_Init();
	OSInit();
	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO);
	OSStart();
}



