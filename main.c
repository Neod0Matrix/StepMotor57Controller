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
	
	Timer_InitConfig();							//定时器及脉冲计算器初始化配置
	
	//显示器初始化	
	LCD1602_Init();		
	//来一段动画演示
	LCD1602DisplayLogo();						
	delay_ms(800);		
	LCD1602DisplayStatus();				
}

//total prototype: int main (int argc, char *argv[], char *envp[]);
int main (void)
{
#ifdef DEBUG 											
	debug();									//debug/release											
#endif
	PreSetUpHardware();							//初始化硬件底层
	PreInitFinishedBlink();						//初始化完成
	
	while (True)
	{
		MatrixKeyValueHandler();				//处理矩阵键盘输入信号
		RemoteDecodeValueHandler();				//处理红外遥控器输入信号
		LCD1602DisplayUpdate();					//设定输入变量范围并更新显示
	}
}

//==========================================================================
//code by </MATRIX>@Neod Anderjon
