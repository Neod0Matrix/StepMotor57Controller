#include "stdafx.h"
//code by </MATRIX>@Neod Anderjon
//==========================================================================
//LCD1602 UI界面

//LCD1602显示字符串
u8 code MotorStatus[] 	= "MS:";				//显示电机状态
u8 code Start[] 		= "Work";				//起始转动显示
u8 code Stop[] 			= "Stew";				//停止转动显示
u8 code Direction[] 	= "DIR:";				//显示转动方向
u8 code Postive[] 		= "Pos";				//正转
u8 code Negative[] 		= "Neg";				//反转
u8 code RotationAngle[] = "RA:";				//转动角度
u8 code RotationSpeed[] = "RS:";				//转动速度
//Logo & UI
u8 code WkverLogo[] 	= "T.WKVER";
u8 code MinusSymbol16[] = "--------------->";	//特效线
u8 code MatrixLogo[]	= "</MATRIX>";
u8 code codeBy[]		= "Code By";
u8 code ArthorName[]	= "Neod Anderjon";
u8 code ProjectLogo[]	= "StepMotorControl";

//显示logo屏内容
void LCD1602DisplayLogo (void)
{
	LCD1602_ClearScreen();
	lcd_es = dis_logo;
	LCD1602_DisplayString(5, ROW1, WkverLogo);
	LCD1602_DisplayString(0, ROW2, ProjectLogo);
	delay_ms(500);
	LCD1602_ClearScreen();
	LCD1602_DisplayString(4, ROW1, MatrixLogo);
	delay_ms(500);
	LCD1602_ClearScreen();
	LCD1602_DisplayString(0, ROW1, codeBy);
	LCD1602_DisplayString(3, ROW2, ArthorName);
}

//显示status屏内容
void LCD1602DisplayStatus (void)
{
	LCD1602_ClearScreen();
	lcd_es = dis_status;
	//显示电机状态，默认停止
	LCD1602_DisplayString(0, ROW1, MotorStatus);
	LCD1602_DisplayString(3, ROW1, Stop);
	//显示方向，默认正转
	LCD1602_DisplayString(9, ROW1, Direction);
	LCD1602_DisplayString(13, ROW1, Postive);
	//显示转速和转动角度
	LCD1602_DisplayString(0, ROW2, RotationAngle);
	LCD1602_DisplayNumber(3, RotationDistance, ROW2);
	LCD1602_DisplayString(9, ROW2, RotationSpeed);
	LCD1602_DisplayNumber(12, SettingSpeedHz, ROW2);
}

//滑动logo
void LCD1602DisplaySlideLogo (void)
{
	uint8_t i;
	
	LCD1602_ClearScreen();
	lcd_es = dis_slide;
	while (True)
	{
		for (i = 0; i < 16; i++)
		{
			LCD1602_DisplayString(0, ROW1, MinusSymbol16);
			LCD1602_DisplayString(i, ROW1, WkverLogo);
			LCD1602_DisplayString(0, ROW2, ProjectLogo);
			delay_ms(500);
			if (gdv != rplay) break;	//退出显示循环
		}
		if (gdv != rplay) break;		//退出显示循环
	}
}

//对输入变量限定取值
void LCD1602DisplayUpdate (void)
{
	//转动角度数值显示限位
	if (RotationDistance <= 0) RotationDistance = 0;
	else if (RotationDistance > 9999) RotationDistance = 0;
	//转速计算限制
	if (SettingSpeedHz <= 0) SettingSpeedHz = 0;
	else if (SettingSpeedHz > 4000) SettingSpeedHz = 0;		
	
	if (lcd_es == dis_status)
	{
		//更新电机运行状态
		if (MotorStatusFlag == Stew)
			LCD1602_DisplayString(3, ROW1, Stop);
		else
			LCD1602_DisplayString(3, ROW1, Start);
		//更新方向
		if (IO_Direction == posrot)
			LCD1602_DisplayString(13, ROW1, Postive);
		else 
			LCD1602_DisplayString(13, ROW1, Negative);
		//更新变量
		LCD1602_DisplayNumber(3, RotationDistance, ROW2);
		LCD1602_DisplayNumber(12, SettingSpeedHz, ROW2);
	}		
}

//==========================================================================
//code by </MATRIX>@Neod Anderjon
