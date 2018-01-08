#include "stdafx.h"
//code by </MATRIX>@Neod Anderjon
//==========================================================================
//延时函数

//延时1us
void delay_us (uint16_t x)              
{
   uint16_t i, j;            
	
   for (i = x; i > 0; i--)
       for (j = 10; j > 0; j--);
}

//延时0.14ms，红外解码使用
void delay_0p14ms (uint8_t x)     		
{
	uint8_t i;        
	
	while (x--)                     		
	{
		for (i = 0; i < 13; i++);    		
	}
}

//延时1ms
void delay_ms (uint16_t i)	            
{
	uint16_t m, n;
	
	for (m = i; m > 0; m--)
	    for (n = 125; n > 0; n--);
}

//==========================================================================
//Code by </MATRIX>@Neod Anderjon
