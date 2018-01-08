#include "stdafx.h"
//code by </MATRIX>@Neod Anderjon
//==========================================================================
//P2.2-P2.7 4x4矩阵键盘扫描，实际只使用8键

KeyValueSetting gkv = kv_none;

//矩阵键盘键值扫描
KeyValueSetting MatrixKeyboard_Scan (void) //reentrant
{
	static u8 temp;								//无状态保值
	KeyValueSetting key;
	
	//第一列扫描
	MatrixCallPort = 0xfe;						//f保值，e写入该列的对应码
    temp = MatrixCallPort;
    temp = temp & 0xf0;
    if (temp != 0xf0)
    {
		delay_ms(10);							//消抖
		if (temp != 0xf0)
		{
			temp = MatrixCallPort;
			LEDGroupCtrl(led_1, On);
			switch (temp)
			{
				//对应码e
			case 0xee: key = kv_start;  break;	//k1
			case 0xde: key = kv_stop; 	break;	//k5
			case 0xbe: key = kv_dir_l;	break;	//k9
			case 0x7e: key = kv_dir_r; 	break;	//k13
			}
			while (temp != 0xf0)
			{
				temp = MatrixCallPort;
				temp = temp & 0xf0;
				delay_ms(50);
				LEDGroupCtrl(led_1, Off);
			}
		}
    }
	
	//第二列扫描
    MatrixCallPort = 0xfd;						//f保值，d写入该列的对应码
    temp = MatrixCallPort;
    temp = temp & 0xf0;
    if (temp != 0xf0)
    {
		delay_ms(10);							//消抖
		if (temp != 0xf0)
		{
			temp = MatrixCallPort;
			LEDGroupCtrl(led_1, On);
			switch (temp)
			{
				//对应码d
			case 0xed: key = kv_ra_plus; break;	//k2
			case 0xdd: key = kv_ra_minu; break;	//k6
			case 0xbd: key = kv_rs_plus; break;	//k10
			case 0x7d: key = kv_rs_minu; break;	//k14
			}
			while (temp != 0xf0)
			{
				temp = MatrixCallPort;
				temp = temp & 0xf0;
				delay_ms(50);
				LEDGroupCtrl(led_1, Off);
			}
		}
    }
	
	//第三列扫描
	MatrixCallPort = 0xfb;						//f保值，b写入该列的对应码
    temp = MatrixCallPort;
    temp = temp & 0xf0;
	if (temp != 0xf0)
    {
		delay_ms(10);							//消抖
		if (temp != 0xf0)
		{
			temp = MatrixCallPort;
			LEDGroupCtrl(led_1, On);
			switch (temp)
			{
				//对应码b
			case 0xeb: break;	//k3
			case 0xdb: break;	//k7
			case 0xbb: break;	//k11
			case 0x7b: break;	//k15
			}
			while (temp != 0xf0)
			{
				temp = MatrixCallPort;
				temp = temp & 0xf0;
				delay_ms(50);
				LEDGroupCtrl(led_1, Off);
			}
		}
    }
	
	//第四列扫描
	MatrixCallPort = 0xf7;						//f保值，7写入该列的对应码
    temp = MatrixCallPort;
    temp = temp & 0xf0;
	if (temp != 0xf0)
    {
		delay_ms(10);							//消抖
		if (temp != 0xf0)
		{
			temp = MatrixCallPort;
			LEDGroupCtrl(led_1, On);
			switch (temp)
			{
				//对应码7
			case 0xe7: break;	//k4
			case 0xd7: break;	//k8
			case 0xb7: break;	//k12
			case 0x77: break;	//k16
			}
			while (temp != 0xf0)
			{
				temp = MatrixCallPort;
				temp = temp & 0xf0;
				delay_ms(50);
				LEDGroupCtrl(led_1, Off);
			}
		}
    }
	
	return key;
}

//矩阵键盘键值处理
void MatrixKeyValueHandler (void)
{
	KeyValueSetting kv = MatrixKeyboard_Scan();
	
	switch (kv)
	{
	case kv_start: 		PulseProduce_Start();	break;	//电机启动
	case kv_stop: 		PulseProduce_Stop(); 	break;	//急停
	case kv_dir_l: 		MotorPostiveRot;		break;	//正转											
	case kv_dir_r: 		MotorNegativeRot;		break;	//反转											
	case kv_ra_plus: 	RotationDistance += 30; break;	//增加转动角度，额度30
	case kv_ra_minu: 	RotationDistance -= 30; break;	//减少转动角度，额度30
	case kv_rs_plus: 	SettingSpeedHz += 500; 	break;	//增加转速，额度500
	case kv_rs_minu: 	SettingSpeedHz -= 500; 	break;	//减少转速，额度500
	}
}

//==========================================================================
//code by </MATRIX>@Neod Anderjon
