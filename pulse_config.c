#include "stdafx.h"
//code by </MATRIX>@Neod Anderjon
//==========================================================================
//脉冲配置发生器

u32 ReversalCnt;
u32 SettingSpeedHz;
u32 RotationDistance;
u32 divFreqCnt;
u32 CalDivFreqConst;
u32 ReversalRange;
MotorRunStatus MotorStatusFlag;
MotorRunMode MotorModeFlag;

//EA寄存器操作
void TimerEnableAllOperate (FunctionStatus flag)
{
	EA = flag;
}

//TMOD定时器模式寄存器操作
void TimerModeRegisterOperate (char order)
{
	TMOD = order;
}

//ITx寄存器操作
void TimerInterruptOperate (TimerNumber nbr, FunctionStatus flag)
{
	if (nbr == Timer0)
		IT0 = flag;
	else if (nbr == Timer1)
		IT1 = flag;
}

//ETx寄存器操作
void TimerEnableTimerOperate (TimerNumber nbr, FunctionStatus flag)
{
	if (nbr == Timer0)
		ET0 = flag;
	else if (nbr == Timer1)
		ET1 = flag;
}

//TRx寄存器操作
void TimerTriggerRegisterOperate (TimerNumber nbr, FunctionStatus flag)
{
	if (nbr == Timer0)
		TR0 = flag;
	else if (nbr == Timer1)
		TR1 = flag;
}

//EXx寄存器操作
void TimerExternInterruptOperate (TimerNumber nbr, FunctionStatus flag)
{
	if (nbr == Timer0)
		EX0 = flag;
	else if (nbr == Timer1)
		EX1 = flag;
}

//THx，TLx寄存器操作
void TimerInitValueOperate (TimerNumber nbr, u32 value, TimerBit bt)
{
	uint32_t hBitValue, lBitValue;
	
	switch (bt)
	{
	case bit8: hBitValue = (256 - value); lBitValue = (256 - value); break;
	case bit13: hBitValue = (8192 - value) / 256; lBitValue = (8192 - value) % 256; break;
	case bit16: hBitValue = (65536 - value) / 256; lBitValue = (65536 - value) % 256; break;
	case bitNone: hBitValue = value; lBitValue = value; break;
	}
	if (nbr == Timer0)
	{
		TH0 = hBitValue;
		TL0 = lBitValue;
	}
	else if (nbr == Timer1)
	{
		TH1 = hBitValue;
		TL1 = lBitValue;
	}
}

//定时器初始配置
void Timer_InitConfig (void)
{
	/*
		定时器0用于脉冲发生器及急停外部中断
		定时器1开启外部中断用于红外解码接收
	*/
	TimerModeRegisterOperate(0x01);		
	TimerInitValueOperate(Timer0, Timer0Value, bit16);
	TimerInitValueOperate(Timer1, Timer1Value, bitNone);    
	TimerEnableAllOperate(Enable);					//打开定时器总使能
	
	TimerInterruptOperate(Timer0, Enable);
	TimerEnableTimerOperate(Timer0, Enable);		//T0开中断使能
	TimerTriggerRegisterOperate(Timer0, Enable);	//打开T0			
	TimerExternInterruptOperate(Timer0, Enable);	//打开外部中断0使能
	
	//T1不使能ET1，只使用外部中断
	TimerTriggerRegisterOperate(Timer1, Enable);	//打开T1			
	TimerInterruptOperate(Timer1, Enable);			//允许外部中断
	TimerExternInterruptOperate(Timer1, Enable);	//打开外部中断1使能
    
	//设定脉冲使用初值
	ReversalCnt = 0;
	SettingSpeedHz = 500;				
	RotationDistance = 360;
	divFreqCnt = 0;
	CalDivFreqConst = DivFreqMaxRange / SettingSpeedHz - 1u;
	ReversalRange = 2 * RadTransferLine * RotationDistance - 1u;
	MotorStatusFlag = Stew;							//默认静置
	MotorModeFlag = LimitRun;						//默认有限运行
}

/*
	定时器0中断服务
	中断内部不要进行计算，51计算会产生不可预估的延时
*/
void Timer0Service () interrupt 1
{
	//TimerInitValueOperate(Timer0, Timer0Value, bit16);//函数不可重入
	TH0 = (65536 - Timer0Value) / 256;
	TL0 = (65536 - Timer0Value) % 256;
	
	//更新分频系数
	if (divFreqCnt == CalDivFreqConst)
    {
        divFreqCnt = 0;
        IO_MainPulse = !IO_MainPulse;	
		if (ReversalCnt == ReversalRange && MotorModeFlag == LimitRun)
		{
			//此处进行计算(不管复杂度)会干扰信号频率
			TimerTriggerRegisterOperate(Timer0, Disable);	
			IO_MainPulse = 1;	
			MotorStatusFlag = Stew;
		}
		ReversalCnt++;	
    }
	divFreqCnt++;
}

//外部中断急停(速度比矩阵键盘和红外解码要快很多)
void ExternInt0Service () interrupt 0
{
	TimerExternInterruptOperate(Timer0, Disable);
	if (IO_EmeStop == 0)
	{
		delay_ms(10);
		if (IO_EmeStop == 0)
		{
			LEDGroupCtrl(led_1, On);
			
			/*
				这里和PulseProduce_Stop()内容是一样的
				但是直接调用PulseProduce_Stop()会有函数重入的警告
			*/
			TimerTriggerRegisterOperate(Timer0, Disable);	
			IO_MainPulse = 1;						//脉冲IO口拉高
			divFreqCnt = 0;
			ReversalCnt = ReversalRange;
			MotorStatusFlag = Stew;
			LEDGroupCtrl(led_0, Off);
			
			while (!IO_EmeStop);					//等待释放
			delay_ms(50);
			LEDGroupCtrl(led_1, Off);
		}
	}
	TimerExternInterruptOperate(Timer0, Enable);
}

//电机停止转动
void PulseProduce_Stop (void)
{								
	TimerTriggerRegisterOperate(Timer0, Disable);	//失能定时器T0	
	IO_MainPulse = 1;								//脉冲IO口拉高
	divFreqCnt = 0;
	ReversalCnt = ReversalRange;
	MotorStatusFlag = Stew;
	LEDGroupCtrl(led_0, Off);
	
	if (lcd_es == dis_status)
		LCD1602_DisplayString(3, ROW1, Stop);	
}

//固有脉冲数自动完成
void PulseProduce_Start(void)					    
{				
	//判断送入值是否有效
	if (SettingSpeedHz != 0u && RotationDistance != 0u)
	{
		//计数变量初始化清零
		divFreqCnt = 0;
		ReversalCnt = 0;						
		//更新判断量，把计算放在中断外面执行
		CalDivFreqConst = DivFreqMaxRange / SettingSpeedHz - 1u;
		ReversalRange = 2 * RadTransferLine * RotationDistance - 1u;
		LEDGroupCtrl(led_0, On);
		MotorStatusFlag = Run;
		if (lcd_es == dis_status)
			LCD1602_DisplayString(3, ROW1, Start);
		//使能T0定时器，脉冲开始处理计数和发送		
		TimerTriggerRegisterOperate(Timer0, Enable);							
	}
}

//切换电机运行模式
void MotorRunModeAdjust (void)
{
	MotorModeFlag = !MotorModeFlag;	//两种模式切换简写
	
//	if (MotorModeFlag == LimitRun)
//		MotorModeFlag = UnlimitRun;
//	else
//		MotorModeFlag = LimitRun;
}

//==========================================================================
//code by </MATRIX>@Neod Anderjon
