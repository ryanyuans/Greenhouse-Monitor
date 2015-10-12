//*************************************************************//
//	   				智能温室监控大棚
//				单片机 AT89S52 或 STC89C52RC 
//					Ryan_yuans	2015.10
//*************************************************************//

#define _MAIN_C

#include "CONFIG.H"
#include "DHT11.H"
#include "LCD1602.H"
#include "GSM.H"




//----------------------------------------------//
//----------------定义区--------------------//
//----------------------------------------------//

U8 T0RH = 0;
U8 T0RL = 0;
U8 T1RH = 0;
U8 T1RL = 0;
U16 flag1s = 0;
U16 flag3s = 0;
uchar str[10];
U8 enbuzz1 = 0,enbuzz2 = 0,enbuzz3 = 0;
U8 ENA_Temp1 = 0,ENA_Temp2 = 0;
U8 SendOn = 2;

void TrasfToString(U8 *str,U8 dat)		// 把读取的数据转化成字符串
{
	signed char i = 0;
	uchar len = 0;
	uchar buf[6];
	
	if(dat < 0)
	{
		dat = -dat;
		*str++ = '-';
		len++;
	}
	
	do
	{
		buf[i++] = dat % 10;
		dat /= 10;
	}while(dat > 0);
	
	len += i;
	
	while(i-- > 0)
	{
		*str++ = buf[i] + '0';
	}
	
	*str = '\0';

}


void ConfigTimer0(U16 ms)	// 设置定时器T0 底层驱动
{
	U32 tmp;
	
	tmp = 11059200 / 12;
	tmp = (tmp * ms) / 1000;
	tmp = 65536 - tmp;
	tmp = tmp + 13;
	T0RH = (uchar)(tmp >> 8);
	T0RL = (uchar)tmp;
	TH0 = T0RH;
	TL0 = T0RL;
	TMOD &= 0XF0;
	TMOD |= 0X01;
	ET0 = 1;
	TR0 = 1;
}

/*
void ConfigTimer1(U16 ms)	// 设置定时器T1 底层驱动
{
	U32 tmp;
	
	tmp = 11059200 / 12;
	tmp = (tmp * ms) / 1000;
	tmp = 65536 - tmp;
	tmp = tmp + 2;
	T1RH = (uchar)(tmp >> 8);
	T1RL = (uchar)tmp;
	TH1 = T0RH;
	TL1 = T0RL;
	TMOD &= 0X0F;
	TMOD |= 0X10;
	ET1 = 1;
	TR1 = 1;
}
*/

void ConfigTimer2(U16 baud)		// 蜂鸣器 控制
{
    //配置T2作为动态扫描定时
    T2CON = 0x00;  //配置T2工作在16位自动重载定时器模式
    RCAP2H = ((65536 - SYS_MCLK/baud) >> 8);  //配置重载值，每秒产生1500次中断，  			SYS_MCLK   (11059200/12)
    RCAP2L = (65536 - SYS_MCLK/baud);       //以使刷新率达到100Hz无闪烁的效果				系统主时钟频率，即振荡器频率÷12
    TH2 = RCAP2H;  //设置初值等于重载值
    TL2 = RCAP2L;
    ET2 = 1;       //使能T2中断
    PT2 = 1;       //设置T2中断为高优先级
    TR2 = 1;       //启动T2
}

void ShowTempHumi()			// 显示温湿度至液晶屏
{
	
	TrasfToString(str,U8T_data_H);
	ShowStr(6,0,str);
	TrasfToString(str,U8RH_data_H);
	ShowStr(6,1,str);
}

void Alarm_Monitor()		// 监控各指标_ 温度、湿度、有害气体浓度
{
	
		if(U8T_data_H >= 35)	// 当温度 >= 35°C 时
		{
			ShowStr(14,0,"!~");
			ENA_Temp2 = 1;
			enbuzz2 = 1;
		}
		else
		{
			ShowStr(14,0,"  ");
			ENA_Temp2 = 0;
			enbuzz2 = 0;
		}
		
		if(U8RH_data_H >= 60)	// 当湿度 >= 60% 时
		{
			ShowStr(14,1,"!~");
			enbuzz3 = 1;
		}
		else
		{
			ShowStr(14,1,"  ");
			enbuzz3 = 0;
		}
		
		if(DOUT==0)		// 检测有害气体浓度是否超标
			{
				Delay(1);	// 延时再次确认
				if(DOUT == 0)	// 当有害气体浓度超标
				{
					enbuzz1 = 1;
					ENA_Temp1 = 1;
					
					if(SendOn == 2)
					{
						 SendOn = 1; 
					}
					
				}
			}
		 else
			{
				enbuzz1 = 0;
				ENA_Temp1 = 0;
			}
			
}
/*
void Alarm_Action()			// 指标异常 执行操作
{
	if((ENA_Temp1 == 1) || (ENA_Temp2 == 1))	// 启动风扇 抽风 降温
	{
		ENA = 1;
	}
	else
	{
		ENA = 0;
	}
}
*/
//----------------------------------------------
void main()
{
	P1 = 0xff;
	INA = 0;
	INB = 1;
	ENA = 0;
	
	EA = 1;
	ConfigTimer0(20);		// 20 X 50 = 1 S
	ConfigTimer2(120);		// 每秒 1000次
	Init1602();
	SerialInti();
	
	ShowStr(0,0,"Temp:");
	ShowStr(0,1,"Humi:");
	
	Delay(1);         //延时100US（12M晶振)
	
	while(1)
	{
		
/*  调用温湿度读取子程序 读取温湿度！ */
		R_HT();
			
			
		if(flag1s == 1)		
		{
			flag1s = 0;
			
			ShowTempHumi();		// 将温湿度显示在液晶屏上
			
			Alarm_Monitor();	// 监控各指标
			
			//Alarm_Action();		// 风扇机  , 下面中断控制！~
		}		
		
	}//while
	
}// main

// ---------------------------------------------
void Timer0() interrupt 1
{
	static U8 timer1 = 0;
	
	TH0 = T0RH;
	TL0 = T0RL;
	
	
	timer1++;
	
	if(timer1 == 50)		// 计时 20 X 50 ==> 1 S
	{
		timer1 = 0;
		flag1s = 1;
	}
	
	if((ENA_Temp1 == 1) || (ENA_Temp2 == 1))	// 启动风扇 抽风 降温***************************
	{
		ENA = 1;
	}
	else
	{
		ENA = 0;
	}
	
	if(SendOn == 1)
	{
		SendOn = 0;
		Sendit();
	}
}

void Timer2() interrupt 5
{
	TF2 = 0;  //清零T2中断标志
	
	if((enbuzz1 == 1) || (enbuzz2 == 1) || ((enbuzz3 == 1)))	// 打开蜂鸣器 ~BUZZ
	{
		BUZZ = ~BUZZ;
	}
	else
	{
		BUZZ = 1;
	}
	
}