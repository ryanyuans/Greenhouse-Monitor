//*************************************************************//
//	   				�������Ҽ�ش���
//				��Ƭ�� AT89S52 �� STC89C52RC 
//					Ryan_yuans	2015.10
//*************************************************************//

#define _MAIN_C

#include "CONFIG.H"
#include "DHT11.H"
#include "LCD1602.H"
#include "GSM.H"




//----------------------------------------------//
//----------------������--------------------//
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

void TrasfToString(U8 *str,U8 dat)		// �Ѷ�ȡ������ת�����ַ���
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


void ConfigTimer0(U16 ms)	// ���ö�ʱ��T0 �ײ�����
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
void ConfigTimer1(U16 ms)	// ���ö�ʱ��T1 �ײ�����
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

void ConfigTimer2(U16 baud)		// ������ ����
{
    //����T2��Ϊ��̬ɨ�趨ʱ
    T2CON = 0x00;  //����T2������16λ�Զ����ض�ʱ��ģʽ
    RCAP2H = ((65536 - SYS_MCLK/baud) >> 8);  //��������ֵ��ÿ�����1500���жϣ�  			SYS_MCLK   (11059200/12)
    RCAP2L = (65536 - SYS_MCLK/baud);       //��ʹˢ���ʴﵽ100Hz����˸��Ч��				ϵͳ��ʱ��Ƶ�ʣ�������Ƶ�ʡ�12
    TH2 = RCAP2H;  //���ó�ֵ��������ֵ
    TL2 = RCAP2L;
    ET2 = 1;       //ʹ��T2�ж�
    PT2 = 1;       //����T2�ж�Ϊ�����ȼ�
    TR2 = 1;       //����T2
}

void ShowTempHumi()			// ��ʾ��ʪ����Һ����
{
	
	TrasfToString(str,U8T_data_H);
	ShowStr(6,0,str);
	TrasfToString(str,U8RH_data_H);
	ShowStr(6,1,str);
}

void Alarm_Monitor()		// ��ظ�ָ��_ �¶ȡ�ʪ�ȡ��к�����Ũ��
{
	
		if(U8T_data_H >= 35)	// ���¶� >= 35��C ʱ
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
		
		if(U8RH_data_H >= 60)	// ��ʪ�� >= 60% ʱ
		{
			ShowStr(14,1,"!~");
			enbuzz3 = 1;
		}
		else
		{
			ShowStr(14,1,"  ");
			enbuzz3 = 0;
		}
		
		if(DOUT==0)		// ����к�����Ũ���Ƿ񳬱�
			{
				Delay(1);	// ��ʱ�ٴ�ȷ��
				if(DOUT == 0)	// ���к�����Ũ�ȳ���
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
void Alarm_Action()			// ָ���쳣 ִ�в���
{
	if((ENA_Temp1 == 1) || (ENA_Temp2 == 1))	// �������� ��� ����
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
	ConfigTimer2(120);		// ÿ�� 1000��
	Init1602();
	SerialInti();
	
	ShowStr(0,0,"Temp:");
	ShowStr(0,1,"Humi:");
	
	Delay(1);         //��ʱ100US��12M����)
	
	while(1)
	{
		
/*  ������ʪ�ȶ�ȡ�ӳ��� ��ȡ��ʪ�ȣ� */
		R_HT();
			
			
		if(flag1s == 1)		
		{
			flag1s = 0;
			
			ShowTempHumi();		// ����ʪ����ʾ��Һ������
			
			Alarm_Monitor();	// ��ظ�ָ��
			
			//Alarm_Action();		// ���Ȼ�  , �����жϿ��ƣ�~
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
	
	if(timer1 == 50)		// ��ʱ 20 X 50 ==> 1 S
	{
		timer1 = 0;
		flag1s = 1;
	}
	
	if((ENA_Temp1 == 1) || (ENA_Temp2 == 1))	// �������� ��� ����***************************
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
	TF2 = 0;  //����T2�жϱ�־
	
	if((enbuzz1 == 1) || (enbuzz2 == 1) || ((enbuzz3 == 1)))	// �򿪷����� ~BUZZ
	{
		BUZZ = ~BUZZ;
	}
	else
	{
		BUZZ = 1;
	}
	
}