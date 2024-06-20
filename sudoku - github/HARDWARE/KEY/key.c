//#include "stm32f10x.h"// Device header
#include "key.h"
#include "sys.h"
#include "delay.h"



void key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3|GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
}

uint16_t key_Scan(void)
{
    uint16_t key_val = 0;
    uint8_t i, j;
    uint8_t key_state[4][4] = {
        {1, 1, 1, 1},
        {1, 1, 1, 1},
        {1, 1, 1, 1},
				{1, 1, 1, 1},
        
    };
	//开发版上的四个按钮
	if(KEY_1 == 0)
	{
		key_val = 17;
	}
	if(KEY_2 == 0)
	{
		key_val = 18;
	}
	if(KEY_3 == 0)
	{
		key_val = 19;
	}
		if(KEY_4 == 0)
	{
		key_val = 20;
	}
    for (i = 0; i < 4; i++)
    {
        switch (i)
        {
        case 0:
            KEY_OUT_1 = 0;
            KEY_OUT_2 = 1;
            KEY_OUT_3 = 1;
						KEY_OUT_4 = 1;
            
            break;
        case 1:
            KEY_OUT_1 = 1;
            KEY_OUT_2 = 0;
            KEY_OUT_3 = 1;
				    KEY_OUT_4 = 1;
           
            break;
        case 2:
            KEY_OUT_1 = 1;
            KEY_OUT_2 = 1;
            KEY_OUT_3 = 0;
						KEY_OUT_4 = 1;
            
            break;
				case 3:
            KEY_OUT_1 = 1;
            KEY_OUT_2 = 1;
            KEY_OUT_3 = 1;
						KEY_OUT_4 = 0;
            
            break;
        }

        for (j = 0; j < 4; j++)
        {
            switch (j)
            {
            case 0:
                if (KEY_IN_1 == 0)
                    key_state[i][j] = 0;
                break;
            case 1:
                if (KEY_IN_2 == 0)
                    key_state[i][j] = 0;
                break;
            case 2:
                if (KEY_IN_3 == 0)
                    key_state[i][j] = 0;
                break;
            case 3:
                if (KEY_IN_4 == 0)
                    key_state[i][j] = 0;
                break;
            }
						
        }
    }

    // 返回矩阵键盘按下的值
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (key_state[i][j] == 0)
            {
                // 第i+1行第j+1列的按钮被按下
                key_val = i * 4 + j + 1;
							
                break;
            }
        }
    }
		delay_ms(150);
    return key_val;
}

