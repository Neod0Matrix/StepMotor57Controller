#include "stdafx.h"
//code by </MATRIX>@Neod Anderjon
//==========================================================================
/*
	AlienTek STM32 Mini开发板红外解码，改写到51上，红外接收默认接到P3.3外部中断1上
	红外编码主要有NEC和PPM两种方式，本代码采用NEC协议方式解码
*/

u8 RemoteReceiveCache[7];			    //遥控解码缓存数组
RemoteTelecontrollerCode gdv = rnone;	//有效识别码提取

//外部中断1服务函数
void ExternInt1Service () interrupt 2 
{
	uint8_t j, k, remote_cnt = 0u;              
   
	TimerExternInterruptOperate(Timer1, Disable);
	
	delay_0p14ms(15);                   //红外消抖
	//判断红外信号是否消失
	if (IO_Remote == 1)                        
	{  
		TimerExternInterruptOperate(Timer1, Enable);		
		return;                            
	} 
      
	//等待输出变为高电平，跳过9ms的前导低电平信号
	while (!IO_Remote)                         
		delay_0p14ms(1);                       

	//采集红外遥控器数据
	for (j = 0; j < 4; j++)                     
	{ 
		//分次采集8位数据
		for (k = 0; k < 8; k++)         
		{
			//等待输入变为低电平，跳过4.5ms高电平信号
//			while (IO_Remote)                     
//				delay_0p14ms(1);  
//			while (!IO_Remote)                    
//				delay_0p14ms(1);   
			while (IO_Remote);                    
			while (!IO_Remote);                      
			
			//计算IR高电平时长
			while (IO_Remote)                     
			{
				delay_0p14ms(1);                   
				remote_cnt++;                   	
				//判断计数器累加值，这里设定成30就可以了
				if (remote_cnt >= 30)                     
				{ 
					TimerExternInterruptOperate(Timer1, Enable);       
					return;                      
				}                   
			}
			//进行数据位移操作并自动补零   
			RemoteReceiveCache[j] >>= 1; 
			//判断数据长度，这里写6或8都可以
			if (remote_cnt >= 8)                         
				RemoteReceiveCache[j] |= 0x80;    //数据最高位补1
			remote_cnt = 0;                             
		}
	}
	//判断地址码是否相同(第三元素和第四元素相加得到0xff)
	if (RemoteReceiveCache[2] != ~RemoteReceiveCache[3])              
	{ 
		TimerExternInterruptOperate(Timer1, Enable);                  
		return;                           
	}
	gdv = RemoteReceiveCache[2];
	TimerExternInterruptOperate(Timer1, Enable);                    
} 

//红外解码键值处理
void RemoteDecodeValueHandler (void)
{
	RemoteTelecontrollerCode dv = rnone;
	
	if (dv != gdv)
	{
		dv = gdv;
		LEDGroupCtrl(led_1, On);
		switch (dv)
		{
		case rlogo: 	LCD1602DisplayLogo();					break;	//显示logo
		case rdelete: 	LCD1602DisplayStatus();					break;	//退回到正常模式
		case rplay: 	LCD1602DisplaySlideLogo();				break;	//滑动logo
		case rpower: 	MotorRunModeAdjust();					break;	//切换运行模式
		case rup: 		LineRadUnitAdjust();					break;	//切换输入行程单位
		case r1: 		PulseProduce_Start();					break;	//电机启动
		case r2: 		PulseProduce_Stop(); 					break;	//急停(红外解码的延时会干扰急停)
		case r3: 		MotorPostiveRot;						break;	//正转
		case r4: 		MotorNegativeRot; 						break;	//反转
		case r5: 		RotationDistance += DValueSetting(); 	break;	//增加转动角度，额度30
		case r6: 		RotationDistance -= DValueSetting(); 	break;	//减少转动角度，额度30
		case r7: 		SettingSpeedHz += 500; 					break;	//增加转速，额度500
		case r8: 		SettingSpeedHz -= 500; 					break;	//减少转速，额度500
		}
		//解码复位
		dv = rnone;
		gdv = rnone;
		delay_ms(50);
		LEDGroupCtrl(led_1, Off);
	}
}

//==========================================================================
//code by </MATRIX>@Neod Anderjon
