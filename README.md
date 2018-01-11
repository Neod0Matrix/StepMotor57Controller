Title
===============

	2018单片机实习 
	基于51单片机的57步进电机运动控制器
	StepMotor57Controller

Copyright
===============

	Copyright @2017 T.WKVER </MATRIX>
	Code by </MATRIX>@Neod Anderjon(LeaderN)
	Thank to fork or watch my project

Update
===============

	最后更新时间: 2018/1/11pm2330

Project Manual
===============

	</MATRIX>提供嵌入式开发维护支持

	MCU:			STC89C52RC
	Developer: 		Neod Anderjon 
	Organization: 	</MATRIX>
	Laboratory: 	T.WKVER
	Project_Verson: OS_v0p7_LTE 长期演进版(什么时候TMD才能跑出个stable来啊)_(:з」∠)_ _(┐「ε:)_
	
Use Manual
===============

	编码提示：本工程采用UTF-8编码，请自行在软件设置中修改
	统一包含头文件：stdafx.h 命名源自Windows编程标准
	之后详细阅读README.md，并且保证自己很清醒
	
I/O Port Map
===============

	引脚		用途			I/O		

	P0-P7		LCD1602驱动		O
	P1.0-P1.7	LED组			O
	P2.0-P2.7	矩阵键盘		I
	P3.2		外部中断急停	I
	P3.3		红外接收管		I
	P3.4-P3.5	LCD1602位设		O
	P3.6		脉冲输出		O
	P3.7		方向控制		O
	
	