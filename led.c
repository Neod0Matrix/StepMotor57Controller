#include "stdafx.h"
//code by </MATRIX>@Neod Anderjon
//==========================================================================
//LED显示控制

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
		LEDGroupCtrl(led_0, Blink);
        LEDGroupCtrl(led_1, Blink);
        delay_ms(50);
        LEDGroupCtrl(led_0, Blink);
        LEDGroupCtrl(led_1, Blink);
        delay_ms(50);
        LEDGroupCtrl(led_0, Blink);
        LEDGroupCtrl(led_1, Blink);
        delay_ms(50);
        LEDGroupCtrl(led_0, Blink);
        LEDGroupCtrl(led_1, Blink);
    }
	LEDGroupCtrl(led_0, Off);
	LEDGroupCtrl(led_0, Off);
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
