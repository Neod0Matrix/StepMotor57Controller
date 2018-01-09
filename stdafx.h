#ifndef __STDAFX_H__
#define __STDAFX_H__
#include <reg52.h>
#include <math.h>
#include <string.h>
//code by </MATRIX>@Neod Anderjon
//==========================================================================
//Windows编程stdafx.h全局声明

//工程声明
//MCU资源
#define __MCU_Model__			"STC89C52RC"			//主控芯片型号
#define __MCU_Flash_Size__		"64"					//Flash
#define __MCU_SRAM_Size__		"2"						//SRAM

//工程声明
#define __Project_Type__		"PMC"					//工程类型
#define __AbbrProjectName__		"StepMotor57Controller"	//名称缩写
#define __Code_Version__ 		"OS_v0p3_LTE"			//长期演进版
#define __Laboratory__			"T.WKVER"				//实验室
#define __Logo__				"Absolute Liberal"		//logo
#define __Developer__			"Neod Anderjon"			//开发者
#define __AbbrDeveloper__		"NA"					//开发者缩写
#define __Organization__		"</MATRIX>"				//组织
#define __Topple_Over__			"_(:з」∠)_ _(┐「ε:)_"	//趴下颜文字
#define __FunnyWord__			"(ಡωಡ)"					//滑稽颜文字

//数据类型声明
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
#define u8 						uint8_t
#define u16 					uint16_t
#define u32 					uint32_t
#define u64 					uint64_t

//IO映射
//LED
sbit LED0 			= P1^0;
sbit LED1 			= P1^1;
//外部中断红外接收管
sbit IO_EmeStop 	= P3^2;
sbit IO_Remote 		= P3^3;
//LCD1602
sbit IO_LCD1602_en 	= P3^4;
sbit IO_LCD1602_rs 	= P3^5;
//步进电机参数控制
sbit IO_MainPulse 	= P3^6;
sbit IO_Direction 	= P3^7;

//LCD1602 P口
#define LCD1602CallPort			P0
//矩阵键盘P口
#define MatrixCallPort			P2

//数字值转ASCII码显示
#ifndef AsciiTransfer
#define AsciiTransfer(nbr) 		(0x30 + nbr)
#endif

/*
	计算出数组宽度(整个数组存储宽度/一个元素的宽度)
	不支持字符串数组，字符串数组直接使用strlen()标准函数
*/
#ifndef Get_Array_Size  									
#define Get_Array_Size(array) 	(sizeof(array) / sizeof((array)[0]))
#endif 

//定时时基
#define OscMainFreqConst		0.9216		//11.0592/12
#define Timer0Value 			50			//此处设定的值越小误差越大，太大计算会超值
#define Timer1Value				0xFD
#define DivFreqMaxRange			(500000L / Timer0Value)

//步进电机细分
#define DivisionConst			16			//细分
#define OneCircultPulse 		200.0f * DivisionConst		
#define OneCircult360			360.0f		//360度
#define RadTransferLine			(OneCircultPulse / OneCircult360)

//bool型数据
typedef enum {True = 1, False = !True} Bool_ClassType;

//使能/失能
typedef enum {Enable = 1, Disable = !Enable} FunctionStatus;

//正反转设置(正向是正对电机轴顺时针转动)
typedef enum {posrot = 0, negrot = !posrot} MotorDirection;
#define MotorPostiveRot 		{IO_Direction = posrot;}
#define MotorNegativeRot 		{IO_Direction = negrot;}

//LED共阳设计，低电平有效
typedef enum {LED_On = 0, LED_Off = !LED_On} LED_Status;

//电机运行状态
typedef enum {Run = 1, Stew = !Run} MotorRunStatus;

//电机运行模式，有限运行(正常)，无限运行(测试脉冲频率使用)
typedef enum {LimitRun = 0, UnlimitRun = 1} MotorRunMode;

//定时器编号，Timer2选项不支持51
typedef enum {Timer0 = 0, Timer1 = 1, Timer2 = 2} TimerNumber;

//定时器模式控制
typedef enum {bit8 = 0, bit13 = 1, bit16 = 2, bitNone = 3} TimerBit;

//封装
#define LED0_On 				(LED0 = LED_On)
#define LED0_Off				(LED0 = LED_Off)
#define LED0_Blink				(LED0 = !LED0)		
#define LED1_On 				(LED1 = LED_On)
#define LED1_Off				(LED1 = LED_Off)
#define LED1_Blink				(LED1 = !LED1)

//LED集群编号
typedef enum
{
	led_0 = 0,
	led_1 = 1,
	led_2 = 2,
	led_3 = 3,
} LEDGroupNbr;

//LED动作列表
typedef enum 
{
	On 		= 1,
	Off 	= 2,
	Blink 	= 3,
} LEDMoveList;

//LCD1602行号
typedef enum
{
	ROW1 = 1,
	ROW2 = 2,
} LCD1602_Row;

//矩阵键盘键值设定
typedef enum
{
	kv_none 	= 0,
	kv_start 	= 1,
	kv_stop 	= 2,
	kv_dir_l 	= 3,
	kv_dir_r 	= 4,
	kv_ra_plus	= 5,
	kv_ra_minu	= 6,
	kv_rs_plus 	= 7,
	kv_rs_minu 	= 8,
} KeyValueSetting;

//红外遥控器码表(AlienTek NEC)
typedef enum 
{
	rnone 		= 0x0ff,
	rpower		= 0x45,
	rup 		= 0x46,
	rplay 		= 0x40,
	rlogo 		= 0x47,
	rright 		= 0x43,
	rleft 		= 0x44,
	rvolminu 	= 0x07,
	rdown 		= 0x15,
	rvolplus 	= 0x09,
	r1 			= 0x16,
	r2 			= 0x19,
	r3 			= 0x0d,
	r4 			= 0x0c,
	r5 			= 0x18,
	r6 			= 0x5e,
	r7 			= 0x08,
	r8 			= 0x1c,
	r9 			= 0x5a,
	r0 			= 0x42,
	rdelete 	= 0x4a,
} RemoteTelecontrollerCode;

//LCD1602显示屏切换
typedef enum 
{
	dis_logo 	= 1,
	dis_status 	= 2,
	dis_slide	= 3,
} LCD1602EnumScreen;

//全局变量，申请整个工程的作用域
extern u8 code MotorStatus[];
extern u8 code Start[];
extern u8 code Stop[];
extern u8 code Direction[];
extern u8 code Postive[];
extern u8 code Negative[];
extern u8 code RotationAngle[];
extern u8 code RotationSpeed[];
extern u8 code WkverLogo[];
extern u8 code MinusSymbol16[];
extern u8 code MatrixLogo[];
extern u8 code codeBy[];
extern u8 code ArthorName[];
extern u8 code ProjectLogo[];
extern u32 SettingSpeedHz;
extern u32 RotationDistance;
extern u32 ReversalCnt;
extern u32 divFreqCnt;
extern u32 CalDivFreqConst;
extern u8 RemoteReceiveCache[7];
extern KeyValueSetting gkv;
extern RemoteTelecontrollerCode gdv;
extern LCD1602EnumScreen lcd_es;
extern MotorRunStatus MotorStatusFlag;
extern MotorRunMode MotorModeFlag;

//函数声明
//延时系列函数
extern void delay_us (uint16_t x);  
extern void delay_ms (uint16_t i);
extern void delay_0p14ms (uint8_t x);
//LED操作函数
extern void LEDGroupCtrl (LEDGroupNbr nbr, LEDMoveList mv);
extern void Aft_PeriInit_Blink (void);
//LCD1602操作函数
extern void LCD1602_WriteCommand (u8 com);	      
extern void LCD1602_WriteData (u8 dat);
extern void LCD1602_Init (void);
extern void LCD1602_DisplayString (u8 addr, LCD1602_Row row, u8 array[]);	
extern void LCD1602_DisplayNumber (u8 addr, u32 dat, LCD1602_Row row);
extern void LCD1602DisplayLogo (void);
extern void LCD1602DisplayStatus (void);
extern void LCD1602_ClearScreen (void);
extern void LCD1602DisplaySlideLogo (void);
//电机控制、定时器配置函数
extern void TimerEnableAllOperate (FunctionStatus flag);
extern void TimerModeRegisterOperate (char order);
extern void TimerInterruptOperate (TimerNumber nbr, FunctionStatus flag);
extern void TimerEnableTimerOperate (TimerNumber nbr, FunctionStatus flag);
extern void TimerTriggerRegisterOperate (TimerNumber nbr, FunctionStatus flag);
extern void TimerExternInterruptOperate (TimerNumber nbr, FunctionStatus flag);
extern void TimerInitValueOperate (TimerNumber nbr, u32 value, TimerBit bt);
extern void PulseProduce_Start(void);
extern void PulseProduce_Stop(void);
extern void MotorRunModeAdjust (void);
//外设控制类函数
extern KeyValueSetting MatrixKeyboard_Scan (void);
extern void MatrixKeyValueHandler (void);
extern void RemoteDecodeValueHandler (void);
//主函数初始化调用
extern void Timer_InitConfig (void);
extern void PreSetUpHardware (void);
extern void LCD1602DisplayUpdate (void);

#endif

//==========================================================================
//code by </MATRIX>@Neod Anderjon
