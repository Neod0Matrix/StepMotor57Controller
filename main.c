#include "stdafx.h"
//code by </MATRIX>@Neod Anderjon
//==========================================================================
//调用最顶层

//底层硬件初始化
void PreSetUpHardware (void)
{
	//上电初始化状态
	IO_MainPulse = 1;
	MotorPostiveRot;
	LEDGroupCtrl(led_0, Off);
	LEDGroupCtrl(led_1, Off);
	
	//定时器初始化配置
	Timer_InitConfig();	
	
	//显示器初始化	
	LCD1602_Init();								
	LCD1602DisplayLogo();						
	delay_ms(800);		
	LCD1602DisplayStatus();
	
	//初始化完成
	Aft_PeriInit_Blink();						
}

//int main (int argc, char *argv[], char *envp[])			
int main (void)
{
#ifdef DEBUG 											
	debug();									//debug/release											
#endif
	PreSetUpHardware();							//初始化硬件底层
	
	while (True)
	{
		MatrixKeyValueHandler();				//处理矩阵键盘输入信号
		RemoteDecodeValueHandler();				//处理红外遥控器输入信号
		LCD1602DisplayUpdate();					//设定输入变量范围并更新显示
	}
}

//==========================================================================
//code by </MATRIX>@Neod Anderjon
