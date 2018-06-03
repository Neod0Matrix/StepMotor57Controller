#include "stdafx.h"
//code by </MATRIX>@Neod Anderjon
//==========================================================================
//串口任务/调试助手

//串口控制寄存器配置
void SerialControlRegisterConfig (u8 scon_baud)
{
	SCON = scon_baud;
}

//串口模式配置
void SerialModeRegisterConfig (u8 sm0, u8 sm1, u8 ren, u8 pcon, u8 es)
{
	REN = ren;
	SM0 = sm0;
	SM1 = sm1;
	PCON = pcon;
	ES = es;
}

//串口发送一个字节
void UART_SendByte (u8 ds)
{
	ES = 0;
	SBUF = ds;                        	
	while (!TI);
	TI = 0;
	ES = 1;
}

//串口发送一个字符
void UART_SendString (u8* str)
{
	while (*str != '\0')
	{
		UART_SendByte(*str);
		str++;
	}
}

//上电初始化显示串口显示系统信息
void InitDisplayUARTInfo (void)
{
	UART_SendString("\r\nT.WKVER | </MATRIX> 2018 MCU-51\r\n");
	UART_SendString(__AbbrProjectName__);UART_SendString(" ");UART_SendString(__Code_Version__);
	UART_SendString("\r\nCode by </MATRIX>@Neod Anderjon\r\n");
}

//==========================================================================
//code by </MATRIX>@Neod Anderjon
