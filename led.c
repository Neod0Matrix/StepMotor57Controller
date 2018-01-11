#include "stdafx.h"
//code by </MATRIX>@Neod Anderjon
//==========================================================================
//LED显示控制

//LED流水灯数组
u8 code LEDFlowTable[] 	= {0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f};

//LED集群动作控制
void LEDGroupCtrl (LEDGroupNbr nbr, LEDMoveList mv)
{
	switch (nbr)
	{
	case led_0:
		switch (mv)
		{
		case On: 	LED0_On; 	break;
		case Off: 	LED0_Off;	break;
		case Blink: LED0_Blink;	break;
		}
		break;
	case led_1:
		switch (mv)
		{
		case On: 	LED1_On; 	break;
		case Off: 	LED1_Off;	break;
		case Blink: LED1_Blink;	break;
		}
		break;
	}
}

//初始化完成闪烁标志
void PreInitFinishedBlink (void)
{
    u8 bb;													
	
	//闪烁几次，直到有人发现已经完成初始化
    for (bb = 0; bb < 3; bb++)								
    {
		LEDCallPort = 0x00;
        delay_ms(50);
        LEDCallPort = 0xff;
        delay_ms(50);
        LEDCallPort = 0x00;
        delay_ms(50);
        LEDCallPort = 0xff;
    }
	LEDCallPort = 0xff;
}

//快速开场动画LED
void QuickFlowLED (void)
{
	uint8_t i, j = 0;
	static Bool_ClassType changeFlowDir = False;
	
	for (i = 0; i < 14; i++)
	{
		//左右循环跑马灯
		if (j == 7) changeFlowDir = True;
		else if (j == 0) changeFlowDir = False;
		if (!changeFlowDir) LEDCallPort = LEDFlowTable[++j];
		else LEDCallPort = LEDFlowTable[--j];
		delay_ms(50);
	}
	LEDCallPort = 0xff;
}

//更新LED显示状态
void LEDDisplayUpdate (void)
{
	if (MotorStatusFlag == Stew)
		LEDGroupCtrl(led_0, Off);
	else
		LEDGroupCtrl(led_0, On);
}

//==========================================================================
//code by </MATRIX>@Neod Anderjon
