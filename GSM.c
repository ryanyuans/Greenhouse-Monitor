/************************************************************
程序说明：
本程序运行后如果gprs模块找到服务商信号，就拨打指定电话。
1.将自己的51单片机的串口1连接到GSM 232上
2.找到程序中前面的#define处，根据说明修改好自己的单片机配置，编译程序
2.下载程序
3.等待发送成功


*************************************************************/
#define _GSM_C

#include "Config.h"

//以下是开机后发送到手机的内容，发送的号码在程序中修改。
U8 sms_text[] = "Warning!";

   
//注意，无论接收到信号还是发送完信号，都会进中断服务程序的
/*初始化程序（必须使用，否则无法收发），次程序将会使用定时器1*/
void SerialInti()//初始化程序（必须使用，否则无法收发）
{
	TMOD=0x20;			//定时器1操作模式2:8位自动重载定时器
	TH1=0xfd;			//装入初值，波特率9600
	TL1=0xfd;
	ET1 = 0;	// 禁止T1中断
	TR1=1;//打开定时器
	SM0=0;//设置串行通讯工作模式，（10位异步发送，波特率可变，由定时器1的溢出率控制）
	SM1=1;//(同上)在此模式下，定时器溢出一次就发送一个位的数据
	REN=1;//串行接收允许位（要先设置sm0sm1再开串行允许）
	ES=1;//开串行口中断	
}

/*串行通讯中断，收发完成将进入该中断*/
void Serial_interrupt() interrupt 4 
{
	U8 temps;
//	a=SBUF;
	temps = SBUF;
	RI = 0;		//接收中断信号清零，表示将继续接收
//	flag=1;		//进入中断的标志符号
}


void Uart1Send(U8 c)
{
	SBUF = c;
	while(!TI) ;//等待发送完成信号（TI=1）出现
	TI = 0;	
}

//串行口连续发送char型数组，遇到终止号/0将停止
void Uart1Sends(U8 *str)
{
	while(*str!='\0')
	{
		SBUF=*str;
		while(!TI);//等待发送完成信号（TI=1）出现
		TI=0;
		str++;
	}
}

/* 延时函数大概是1s钟，不过延时大的话不准... */
void DelaySec(U16 sec)
{
	U16 i , j= 0;

	for(i=0; i<sec; i++)
	{
		for(j=0; j<65535; j++)
		{	
		}
	}
}

void Sendit()
{
	
	Uart1Sends("AT+CSCS=\"GSM\"\r\n");
	DelaySec(3);//延时3秒
	Uart1Sends("AT+CMGF=1\r\n");
	DelaySec(3);//延时3秒
	Uart1Sends("AT+CMGS=\"13902540358\"\r\n");	// 接收方的电话号码
	DelaySec(5);//延时3秒
	Uart1Sends(sms_text);	// 短信内容
	Uart1Send(0x1a);
	DelaySec(10);//延时10秒
	
/*
	Uart1Sends("ATD13902540358;\r\n");			// 拨打指定电话~
	DelaySec(15);//延时15秒
	Uart1Sends("ATH\r\n");
	DelaySec(3);//延时10秒
*/
}
