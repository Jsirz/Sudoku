#include "led.h"
#include "stm32f10x.h"    
//LED IO≥ı ºªØ
void LED_Init()
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);    //


	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	



	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 ; 

	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	GPIO_ResetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4); 
	

	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	
	GPIO_Init(GPIOD, &GPIO_InitStructure); 

	GPIO_SetBits(GPIOD,GPIO_Pin_2);
}
