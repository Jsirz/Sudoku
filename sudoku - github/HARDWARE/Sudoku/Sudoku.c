/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "Lcd_Driver.h"
#include "GUI.h"
#include "lcd.h"
#include "lcd_init.h"
#include "stdio.h"
#include "Sudoku.h"
#include "stdlib.h"
#include "stdbool.h"
#include "adc.h"
#include<time.h>


char ss[100];
#define SIZE 9


bool is_valid_move(int sudoku[], int index, int num) {
		int i,j,row,col;
		int start_row,start_col;
	
     row = index / SIZE;
     col = index % SIZE;

    for ( i = row * SIZE; i < row * SIZE + SIZE; i++) {
        if (sudoku[i] == num) {
            return false;
        }
    }

    for ( i = col; i < SIZE * SIZE; i += SIZE) {
        if (sudoku[i] == num) {
            return false;
        }
    }
     start_row = 3 * (row / 3);
     start_col = 3 * (col / 3);
    for ( i = start_row; i < start_row + 3; i++) {
        for ( j = start_col; j < start_col + 3; j++) {
            if (sudoku[i * SIZE + j] == num) {
                return false;
            }
        }
    }

    return true; 
}


void shuffle_array(int array[], int size) {
		int i,j,temp;
    for ( i = size - 1; i > 0; i--) {
         j = rand() % (i + 1);
         temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}


bool fill_sudoku(int sudoku[], int index) {
		static int numbers[SIZE] ={1,2,3,4,5,6,7,8,9};
		int i,num;
    if (index >= SIZE * SIZE) {
        return true; // 
    }

    if (sudoku[index] != 0) {
        return fill_sudoku(sudoku, index + 1); 
    }

   
    shuffle_array(numbers, SIZE);
    for ( i = 0; i < SIZE; i++) {
         num = numbers[i];
        if (is_valid_move(sudoku, index, num)) {
            sudoku[index] = num;
            if (fill_sudoku(sudoku, index + 1)) {
                return true;
            }
            sudoku[index] = 0; 
        }
    }

    return false; 
}


void generate_random_sudoku(int sudoku[]) {
    int i,index,num;
    for ( i = 0; i < SIZE * SIZE; i++) {
        sudoku[i] = 0;
    }
 
    for ( i = 0; i < SIZE; i++) {
         index = rand() % (SIZE * SIZE);
         num = rand() % SIZE + 1;
        while (!is_valid_move(sudoku, index, num)) {
            index = rand() % (SIZE * SIZE);
            num = rand() % SIZE + 1;
        }
        sudoku[index] = num;
    }

    fill_sudoku(sudoku, 0); 
}

void random_remove(int sudoku[], int num_to_remove) {
    int removed = 0;
		int index;
    while (removed < num_to_remove) {
         index = rand() % (SIZE * SIZE);
        if (sudoku[index] != 0) {
            sudoku[index] = 0;
            removed++;
        }
    }
}


void Display_sudoko(int *s,int *solve_sudoko){
	int i;
	int num_to_remove = 30;
	int sudoku[SIZE * SIZE];
	int a=rand()%3;
	int adc =Get_Adc(a);
	srand(adc);
	generate_random_sudoku(sudoku);
	for(i=0;i<81;i++){
		solve_sudoko[i] = sudoku[i];
	}
		random_remove(sudoku, num_to_remove);
	 for(i=0;i<81;i++){
		s[i] = sudoku[i];
	}
}

void show_sudoku(int *s,int p)
{
	int i=0,x=8,y=0,k=1;
	Lcd_Clear(GRAY0);
	for(i=0 ;i<81;i++)
	{
		sprintf(ss,"%d",s[i]);
		if(i==p)
		{
			Gui_DrawFont_GBK16(x, y, RED,GRAY0, (unsigned char *)ss);
		}
		else
			Gui_DrawFont_GBK16(x, y, BLUE,GRAY0, (unsigned char *)ss);
		x+=13;
		if(k==9)
		{
			k=0;
			y+=14;
			x=8;
		}
		k++;
	}
	y=14;
	x=19;
	for(i=0;i<8;i++)//»­ÐÐÏß
	{
		Gui_DrawLine(0,y,130,y,BLACK);
		y+=14;
	}
	for(i=0;i<8;i++)//»­ÊúÏß
	{
		Gui_DrawLine(x,0,x,130,BLACK);
		x+=13;
	}
}

int is_correct(int *s1,int *s2)
{
	int i;
	for(i=0;i<81;i++)
	{
		if(s1[i]!=s2[i])
		{
			return 0;
		}
	}
	return 1;
}
