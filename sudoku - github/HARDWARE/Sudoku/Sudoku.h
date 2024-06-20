#ifndef __Sudoku_H
#define __Sudoku_H 

#include "stm32f10x.h"


void Game_sudoku(void);
void show_sudoku(int *s,int p);
int is_correct(int *s1,int *s2);
void Display_sudoko(int *s,int *solve_sudoko);

#endif
