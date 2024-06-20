/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "Lcd_Driver.h"
#include "GUI.h"
#include "delay.h"
#include "Picture.h"
#include "Menu.h"
#include "lcd.h"
#include "lcd_init.h"
#include "stdio.h"

int *arr1;
char arr2[100];
unsigned char Num[10]={0,1,2,3,4,5,6,7,8,9};
u8 *s;
u8* pin1;
u8* pin2;
u8* pin3;
u8* pin4;
u8* pin5;
void name_menu(void)
{

	Lcd_Init();
	LCD_LED_SET;
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(40,5,BLUE,GRAY0,"XXX");
	Gui_DrawFont_GBK16(10,25,RED,GRAY0,"123456");
	Gui_DrawFont_GBK16(40,50,BLUE,GRAY0,"XXX");
	Gui_DrawFont_GBK16(10,75,RED,GRAY0,"123456");
	delay_ms(1000);
}

void game_menu(void)
{
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(30,30,RED,GRAY0,"数独游戏");
	Gui_DrawFont_GBK16(30,65,BLUE,GRAY0,"开始游戏");
	Gui_DrawFont_GBK16(30,85,BLUE,GRAY0,"设置时间");
	delay_ms(1000);
}
void show_time(int count)
{
	int x=0,xx=75,j=0;
	Lcd_Clear(GRAY0);
	if(count>999)
	{
		Gui_DrawFont_GBK16(30,70,BLUE,GRAY0,"over");
	}
	else
	{
		Gui_DrawFont_GBK24(110,70,BLUE,GRAY0,"S");
		for(j=0;j<3;j++)
		{
			x=count%10;
			Gui_DrawFont_Num32(xx, 50, BLUE, GRAY0, x);
			xx-=25;
			count=count/10;
			
		}
	}
	//delay_ms(1000);
}

void show_victory(int count)
{
	Lcd_Clear(GRAY0);
	show_time(count);
	Gui_DrawFont_GBK16(38,25,RED,GRAY0,"Victory");
	delay_ms(3000);
}

void show_score(int count)
{
	int score=0,x=0,xx=70,j=0;
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(38,25,RED,GRAY0,"Score");
	score=(1000-count)/10;
	Gui_DrawFont_GBK16(105,70,BLUE,GRAY0,"分");
	for(j=0;j<3;j++)
	{
		x=score%10;
		Gui_DrawFont_Num32(xx, 50, BLUE, GRAY0, x);
		xx-=25;
		score=score/10;
			
	}
	
}

void show_timeover(int count)
{
	Lcd_Clear(GRAY0);
	show_time(count);
	Gui_DrawFont_GBK16(35,25,RED,GRAY0,"Time out");
	delay_ms(1000);
}


void show_false(void)
{
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(50,50,RED,GRAY0,"Error");
	delay_ms(1000);
}

void show_eggs(void)
{
	Lcd_Clear(GRAY0);
	Gui_DrawLine(45, 50,55, 50,BLACK);
	Gui_DrawLine(75, 50,85, 50,BLACK);
	Gui_Circle(65,70,3,RED);
}
