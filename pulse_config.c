#include "stdafx.h"
//code by </MATRIX>@Neod Anderjon
//==========================================================================
//脉冲配置发生器

//全局变量声明
u32 SettingSpeedHz;						//转速设定，直接关系到实际转速和显示值
u32 RotationDistance;					//转动角度，直接关系到实际转速和显示值
u32 divFreqCnt;							//分频计数变量
u32 CalDivFreqConst;					//分频设定常数，影响速度				
u32 ReversalCnt;						//脉冲回收计数变量
u32 ReversalRange;						//脉冲回收设定常数，影响行距
MotorRunStatus MotorStatusFlag;			//电机运行状态标志
MotorRunMode MotorModeFlag;				//电机运行模式标志
LineRadSelect lrs_flag;					//线度角度切换标志

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
	if (nbr == Timer0) IT0 = flag;
	else if (nbr == Timer1) IT1 = flag;
}

//ETx寄存器操作
void TimerEnableTimerOperate (TimerNumber nbr, FunctionStatus flag)
{
	if (nbr == Timer0) ET0 = flag;
	else if (nbr == Timer1) ET1 = flag;
}

//TRx寄存器操作
void TimerTriggerRegisterOperate (TimerNumber nbr, FunctionStatus flag)
{
	if (nbr == Timer0) TR0 = flag;
	else if (nbr == Timer1) TR1 = flag;
}

//EXx寄存器操作
void TimerExternInterruptOperate (TimerNumber nbr, FunctionStatus flag)
{
	if (nbr == Timer0) EX0 = flag;
	else if (nbr == Timer1) EX1 = flag;
}

//THx，TLx寄存器操作
void TimerInitValueOperate (TimerNumber nbr, u32 value, TimerBit bt)
{
	uint32_t hBitValue, lBitValue;
	
	switch (bt)
	{
	case bit8: 		hBitValue = (256 - value); lBitValue = (256 - value); 					break;
	case bit13: 	hBitValue = (8192 - value) / 256; lBitValue = (8192 - value) % 256; 	break;
	case bit16: 	hBitValue = (65536 - value) / 256; lBitValue = (65536 - value) % 256; 	break;
	case bitNone: 	hBitValue = value; lBitValue = value; 									break;
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

//更新行距计算
void DistanceAlgoUpdate (void)
{
	switch (lrs_flag)
	{
	case RadUnit: 	ReversalRange = 2 * RadUnitConst * RotationDistance - 1u; 	break;
	case LineUnit: 	ReversalRange = 2 * LineUnitConst * RotationDistance - 1u; 	break;
	}
}

//定时器初始配置
void Timer_InitConfig (void)
{
	/*
		定时器0用于脉冲发生器及急停外部中断
		定时器1开启外部中断用于红外解码接收、并用作串口波特率发生器
	*/
	TimerModeRegisterOperate(0x21);		
	TimerInitValueOperate(Timer0, Timer0Value, bit16);
	
	SerialControlRegisterConfig(0x50);				//9600波特率
	TimerInitValueOperate(Timer1, Timer1Value, bitNone);    
	
	TimerEnableAllOperate(Enable);					//打开定时器总使能
	
	//脉冲发生器
	TimerInterruptOperate(Timer0, Enable);
	TimerEnableTimerOperate(Timer0, Enable);		//T0开中断使能
	TimerTriggerRegisterOperate(Timer0, Disable);	//初始关闭T0
	TimerExternInterruptOperate(Timer0, Enable);	//打开外部中断0使能
	
	//红外解码
	//T1不使能ET1，只使用外部中断
	TimerTriggerRegisterOperate(Timer1, Enable);	//打开T1			
	TimerInterruptOperate(Timer1, Enable);			//允许外部中断
	TimerExternInterruptOperate(Timer1, Enable);	//打开外部中断1使能
	
	SerialModeRegisterConfig(0, 1, 0, 0x00, 0);		//串口模式配置
    
	//设定脉冲使用初值
	SettingSpeedHz = 0;				
	RotationDistance = 0;
	divFreqCnt = 0;
	CalDivFreqConst = 0;
	ReversalCnt = 0;
	ReversalRange = 0;
	MotorStatusFlag = Stew;							//默认静置
	MotorModeFlag = LimitRun;						//默认有限运行
}

//电机停止转动
void PulseProduce_Stop (void)
{								
	TimerTriggerRegisterOperate(Timer0, Disable);	//失能定时器T0	
	IO_MainPulse = 1;								//脉冲IO口拉高
	divFreqCnt = 0;
	ReversalCnt = ReversalRange;
	MotorStatusFlag = Stew;							//该标志位直接控制LCD和LED的状态更新
}

//固有脉冲数自动完成
void PulseProduce_Start(void)					    
{				
	//计数变量初始化清零
	divFreqCnt = 0;
	ReversalCnt = 0;						
	CalDivFreqConst = DivFreqMaxRange / SettingSpeedHz - 1;//更新分频数
	DistanceAlgoUpdate();							//更新脉冲数
	//判断送入值是否有效
	if (SettingSpeedHz != 0u && RotationDistance != 0u)
	{
		MotorStatusFlag = Run;						//该标志位直接控制LCD和LED的状态更新
		TimerTriggerRegisterOperate(Timer0, Enable);//使能T0定时器，脉冲开始处理计数和发送					
	}
}

/*
	定时器0中断服务
	中断内部不要进行计算，51计算会产生不可预估的延时
*/
void Timer0Service () interrupt 1
{
	TH0 = (65536 - Timer0Value) / 256;
	TL0 = (65536 - Timer0Value) % 256;
	
	//脉冲自动完成
	if (ReversalCnt == ReversalRange && MotorModeFlag == LimitRun)
	{
		TimerTriggerRegisterOperate(Timer0, Disable);	
		IO_MainPulse = 1;	
		MotorStatusFlag = Stew;
		return;											//直接跳出
	}
	//分频
	if (++divFreqCnt == CalDivFreqConst)
    {
        divFreqCnt = 0;
        IO_MainPulse = !IO_MainPulse;	
		ReversalCnt++;									//计数脉冲
    }
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
			
			while (!IO_EmeStop);					//等待释放
			delay_ms(50);
			LEDGroupCtrl(led_1, Off);
		}
	}
	TimerExternInterruptOperate(Timer0, Enable);
}

//切换电机运行模式
void MotorRunModeAdjust (void)
{
	//双值切换
	MotorModeFlag = !MotorModeFlag;					
}

//切换输入量的度量单位
void LineRadUnitAdjust (void)
{
	//在status总显示里也需要单独更新
	switch (lrs_flag)
	{
	case RadUnit: 	lrs_flag = LineUnit; LCD1602_DisplayString(0, ROW2, RotationMeter); break;
	case LineUnit: 	lrs_flag = RadUnit; LCD1602_DisplayString(0, ROW2, RotationAngle); 	break;
	}
}

//变量限位
void VariableRangeLimit (void)
{
	//转动角度数值显示限位
	if (RotationDistance <= 0) RotationDistance = 0;
	else if (RotationDistance > 9999) RotationDistance = 0;
	//转速计算限制
	if (SettingSpeedHz <= 0) SettingSpeedHz = 0;
	else if (SettingSpeedHz > 4000) SettingSpeedHz = 0;		
}

//==========================================================================
//code by </MATRIX>@Neod Anderjon
