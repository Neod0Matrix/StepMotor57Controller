#include "stdafx.h"
//code by </MATRIX>/Neod Anderjon
//==========================================================================
/*
	LCD1602驱动
	常用指令集
	0x28   设置16*2显示，5*7点阵，4位数据接口 
	0x38   设置16*2显示，5*7点阵，8位数据接口 
	0x01   清屏,光标复位到地址00H位置 
	0x02   光标归原点，DDRAM中内容不变，地址计数器AC=0;（此时地址为0x80）  
	0x0F   开显示，显示光标，光标闪烁 
	0x0e   开显示，显示光标，光标不闪烁 
	0x0c   开显示，不显示光标 
	0x08   只开显示  
	0x06   地址加一，当写入数据的时候光标右移  设定显示屏或光标移动方向指令  
	0x18   字符全部左移一格，但光标不动 
	0x1c   字符全部右移一格，但光标不动 
	0x10   光标左移1格，且AC值减1 
	0x14   光标右移1格，且AC值加1
*/

LCD1602EnumScreen lcd_es = dis_status;

//向LCD1602寄存器写命令
void LCD1602_WriteCommand (u8 com)	            
{
	IO_LCD1602_rs = 0;
	IO_LCD1602_en = 0;
	LCD1602CallPort = com;
	delay_ms(5);
	IO_LCD1602_en = 1;
	delay_ms(5);
	IO_LCD1602_en = 0;	
}

//向LCD1602寄存器写数据
void LCD1602_WriteData (u8 dat)	         
{
	IO_LCD1602_rs = 1;
	IO_LCD1602_en = 0;
	LCD1602CallPort = dat;
	delay_ms(5);
	IO_LCD1602_en = 1;
	delay_ms(5);
	IO_LCD1602_en = 0;	
}

//LCD1602清屏
void LCD1602_ClearScreen (void)
{
	LCD1602_WriteCommand(0x01);	
}

//LCD1602显示初始化
void LCD1602_Init (void)
{
	IO_LCD1602_en = 0;
	
	LCD1602_WriteCommand(0x38);			//设置显示模式16*2显示，5*7点阵，8位数据接口
	LCD1602_WriteCommand(0x0c);			//去除光标
	LCD1602_WriteCommand(0x06);			//设置显示模式：光标右移字符不移  
	LCD1602_ClearScreen();				//清屏
}

//LCD1602显示字符串
void LCD1602_DisplayString (u8 addr, LCD1602_Row row, u8 array[])		    	
{
	u16 num;
	
	//选择行号
	switch (row)
	{
	case ROW1: LCD1602_WriteCommand(0x80 + addr); break;
	case ROW2: LCD1602_WriteCommand(0x80 + 0x40 + addr); break;
	}

	//逐字符显示，strlen求取字符串数组长度
    for (num = 0; num < strlen(array); num++)	
	{
		LCD1602_WriteData(array[num]);	 
		delay_us(10);					//显示延时(LCD1602内部寄存器读写延时)
	}
}

//LCD1602显示数字
void LCD1602_DisplayNumber (u8 addr, u32 dat, LCD1602_Row row)		    
{	
	//选择行号
	switch (row)
	{
	case ROW1: LCD1602_WriteCommand(0x80 + addr); break;
	case ROW2: LCD1602_WriteCommand(0x80 + 0x40 + addr); break;
	}
	
	//仅兼容上限4位宽十进制数，自带ASCII转换
	LCD1602_WriteData(AsciiTransfer(dat / 1000));      
	LCD1602_WriteData(AsciiTransfer((dat % 1000) / 100));     
	LCD1602_WriteData(AsciiTransfer((dat % 100) / 10));             
	LCD1602_WriteData(AsciiTransfer(dat % 10));       
}

//==========================================================================
//code by </MATRIX>/Neod Anderjon
