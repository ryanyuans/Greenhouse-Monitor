 /* 
 *****************************************
			1602 Һ������ģ��
 *****************************************
 */
#define _LCD1602_C

#include "Config.h"
#include "LCD1602.h"

void Init1602()		// ��������LCD1602 ��LCD1602д������
{
	void WriteCmd(uchar cmd);
	
	WriteCmd(0X38);
	WriteCmd(0X0C);
	WriteCmd(0X06);
	WriteCmd(0X01);
}

void WriteCmd(U8 cmd)	 	// д������ ���� �������� �ײ�
{
	void WaitReady();
	
	WaitReady();
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_DB = cmd;
	LCD_E = 1;
	LCD_E = 0;

}

void WaitReady()		// 	�жϡ�æ���������� ���ȴ��� �� ��ִ�С�    change something.		
{
	uchar str;
	
	LCD_DB = 0XFF;
	LCD_RS = 0;
	LCD_RW = 1;
	
	do
	{
		LCD_E = 1;
		str = LCD_DB;
		LCD_E = 0;
	}while(str & 0x80);
	
}

void ShowStr(uchar x,uchar y,uchar *str)		// ����ַ + ������
{
	void SetLoc(uchar x,uchar y);
	void WriteData(uchar dat);
	
	SetLoc(x,y);
	
	while(*str != '\0')
	{
			WriteData(*str++);
	}
}

void SetLoc(uchar x,uchar y)		// д��ġ����ݡ���ַ ���� �������� �ײ�
{
	uchar addr;
	
	if(y == 0)
	{
			addr = 0x80 + 0x00 + x;
	}
	else
	{
			addr = 0x80 + 0x40 + x;
	}
	WriteCmd(addr);
}

void WriteData(uchar dat)		// �� LCD1602 ��д������ ���� �������� �ײ�
{
	
	WaitReady();
	
	LCD_RS = 1;
	LCD_RW = 0;
	LCD_DB = dat;
	LCD_E = 1;
	LCD_E = 0;
}
/*
void ClearAll()
{
	WriteCmd(0X01);
}

void ClearArea(uchar x,uchar y,uchar len)		// ָ��λ������ Area
{
	SetLoc(x,y);
	
	while(len--)
	{
			WriteData(' ');
	}
}
*/