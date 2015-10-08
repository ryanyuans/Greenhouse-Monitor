//*************************************************************//
//	   				智能温室监控大棚
//				单片机 AT89S52 或 STC89C52RC 
//					Ryan_yuans	2015.10
//*************************************************************//

#include <reg51.h>
#include <intrins.h> 
//
typedef unsigned char  U8;       /* defined for unsigned 8-bits integer variable 	  无符号8位整型变量  */
typedef signed   char  S8;       /* defined for signed 8-bits integer variable		  有符号8位整型变量  */
typedef unsigned int   U16;      /* defined for unsigned 16-bits integer variable 	  无符号16位整型变量 */
typedef signed   int   S16;      /* defined for signed 16-bits integer variable 	  有符号16位整型变量 */
typedef unsigned long  U32;      /* defined for unsigned 32-bits integer variable 	  无符号32位整型变量 */
typedef signed   long  S32;      /* defined for signed 32-bits integer variable 	  有符号32位整型变量 */
typedef float          F32;      /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         F64;      /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */
//
#define uchar unsigned char
#define uint unsigned int
#define   Data_0_time    4

extern void ShowStr(uchar x,uchar y,uchar *str);
extern void Reset1602();

//----------------------------------------------//
//----------------IO口定义区--------------------//
//----------------------------------------------//
sbit P2_0  = P2^0 ;		// 温湿度数据
sbit DOUT=P2^1;			// 有害气体浓度数据

sbit BUZZ = P1^6;

sbit INA = P1^0;
sbit INB = P1^1;
sbit ENA = P1^2;

//----------------------------------------------//
//----------------定义区--------------------//
//----------------------------------------------//
U8  U8FLAG;
U8  U8count,U8temp;
U8  U8T_data_H = 0, U8T_data_L = 0, U8RH_data_H = 0, U8RH_data_L = 0, U8checkdata = 0;
U8  U8T_data_H_temp = 0, U8T_data_L_temp = 0, U8RH_data_H_temp = 0, U8RH_data_L_temp = 0, U8checkdata_temp = 0;
U8  U8comdata;
U8  count, count_r=0;
U16 U16temp1,U16temp2;
U8 T0RH = 0;
U8 T0RL = 0;
U8 T1RH = 0;
U8 T1RL = 0;
U16 flag1s = 0;
uchar str[10];
U8 enbuzz1 = 0,enbuzz2 = 0,enbuzz3 = 0;
U8 ENA_Temp1 = 0,ENA_Temp2 = 0;

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


void Delay(U16 j)
{      U8 i;
	for(;j>0;j--)
	{ 	
	for(i=0;i<27;i++);

	}
}

void  Delay_10us(void)
{
	U8 i;
	i--;
	i--;
	i--;
	i--;
	i--;
	i--;
}
	
void  COM(void)
 {

	U8 i;
	  
	for(i=0;i<8;i++)	   
	{
	
		U8FLAG=2;	
	while((!P2_0)&&U8FLAG++)
		;
	Delay_10us();
	Delay_10us();
	Delay_10us();
	U8temp=0;
		
	 if(P2_0)
		 U8temp=1;
	 
	 U8FLAG=2;
	 
	 while((P2_0)&&U8FLAG++)
		 ;
	//超时则跳出for循环		  
	 if(U8FLAG==1)
		 break;
	//判断数据位是0还是1	 
	   
	// 如果高电平高过预定0高电平值则数据位为 1 
	 
	   U8comdata <<= 1;
	   U8comdata |= U8temp;        // 0 or 1
	 }//rof
   
}

	/*
	//--------------------------------
	//-----湿度读取子程序 ------------
	//--------------------------------
	//----以下变量均为全局变量--------
	//----温度高8位== U8T_data_H------
	//----温度低8位== U8T_data_L------
	//----湿度高8位== U8RH_data_H-----
	//----湿度低8位== U8RH_data_L-----
	//----校验 8位 == U8checkdata-----
	//----调用相关子程序如下----------
	//---- Delay();, Delay_10us();,COM(); 
	//-------------------------------- ↓
	*/
void R_HT(void)
{

  //主机拉低18ms 
   P2_0=0;
   Delay(180);
   P2_0=1;
 //总线由上拉电阻拉高 主机延时20us
   Delay_10us();
   Delay_10us();
   Delay_10us();
   Delay_10us();
 //主机设为输入 判断从机响应信号 
   P2_0=1;
 //判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行	  
   if(!P2_0)		 //T !	  
   {
	   	TR0 = 0;	//停止定时器计时, 防止中断干扰温湿度数据采集
		TR1 = 0;
	
	   U8FLAG=2;
	 //判断从机是否发出 80us 的低电平响应信号是否结束	 
	   while((!P2_0)&&U8FLAG++);
	   U8FLAG=2;
	 //判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
	   while((P2_0)&&U8FLAG++);
	 //数据接收状态		 
	   COM();
	   U8RH_data_H_temp=U8comdata;
	   COM();
	   U8RH_data_L_temp=U8comdata;
	   COM();
	   U8T_data_H_temp=U8comdata;
	   COM();
	   U8T_data_L_temp=U8comdata;
	   COM();
	   U8checkdata_temp=U8comdata;
	   P2_0=1;
		   
	 //数据校验 ↓
			
	   U8temp = (U8T_data_H_temp + U8T_data_L_temp + U8RH_data_H_temp + U8RH_data_L_temp);
	   if(U8temp == U8checkdata_temp)
	   {
		  U8RH_data_H = U8RH_data_H_temp;
		  U8RH_data_L = U8RH_data_L_temp;
		  U8T_data_H = U8T_data_H_temp;
		  U8T_data_L = U8T_data_L_temp;
		  U8checkdata = U8checkdata_temp;

	   }//fi
   }//fi
   
	TR0 = 1;	// 重新打开定时器计时
	TR1 = 1;
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


void ShowTempHumi()			// 显示温湿度至液晶屏
{
	
	TrasfToString(str,U8T_data_H);
	ShowStr(6,0,str);
	TrasfToString(str,U8RH_data_H);
	ShowStr(6,1,str);
}

void Alarm_Monitor()		// 监控各指标_ 温度、湿度、有害气体浓度
{
	if(DOUT==0)		// 检测有害气体浓度是否超标
			{
				Delay(1);	// 延时再次确认
				if(DOUT == 0)	// 当有害气体浓度超标
				{
					enbuzz1 = 1;
					ENA_Temp1 = 1;
				}
			}
		 else
			{
				enbuzz1 = 0;
				ENA_Temp1 = 0;
			}
	
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
}

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
	
//----------------------------------------------
void main()
{
	
	INA = 0;
	INB = 1;
	ENA = 0;
	
	EA = 1;
	ConfigTimer0(20);		// 20 X 50 = 1 S
	ConfigTimer1(1);	// ~ BUZZ
	Reset1602();
	
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
			
			Alarm_Action();		// 风扇机
		}		
		
	}//while
	
}// main

// ---------------------------------------------
void Timer0() interrupt 1
{
	static U8 timer = 0;
	
	TH0 = T0RH;
	TL0 = T0RL;
	
	
	timer++;
	
	if(timer == 50)		// 计时 20 X 50 ==> 1 S
	{
		timer = 0;
		flag1s = 1;
	}
}

void Timer1() interrupt 3
{
	TH1 = T1RH;
	TL1 = T1RL;
	
	if((enbuzz1 == 1) || (enbuzz2 == 1) || ((enbuzz3 == 1)))	// 打开蜂鸣器 ~BUZZ
	{
		BUZZ = ~BUZZ;
	}
	else
	{
		BUZZ = 1;
	}
	
}