#include "stm32f10x.h"                  // Device header

#ifndef __KEY_H
#define __KEY_H

#define KEY_IN_1 PCin(0)    //¶¨Òå°´¼ü
#define KEY_IN_2 PCin(1)
#define KEY_IN_3 PCin(2)
#define KEY_IN_4 PCin(3)
#define KEY_OUT_1 PCout(4)
#define KEY_OUT_2 PCout(5)
#define KEY_OUT_3 PCout(6)
#define KEY_OUT_4 PCout(7)
#define KEY_1 PCin(8)    
#define KEY_2 PCin(9)
#define KEY_3 PCin(10)
#define KEY_4 PCin(11)
void key_Init(void);
void key(void *pdata);
uint16_t key_Scan(void);
#endif

