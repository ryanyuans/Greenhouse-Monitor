//*************************************************************//
//	   				�������Ҽ�ش���
//				��Ƭ�� AT89S52 �� STC89C52RC 
//					Ryan_yuans	2015.10
//*************************************************************//

#include <reg51.h>
#include <intrins.h> 
//
typedef unsigned char  U8;       /* defined for unsigned 8-bits integer variable 	  �޷���8λ���ͱ���  */
typedef signed   char  S8;       /* defined for signed 8-bits integer variable		  �з���8λ���ͱ���  */
typedef unsigned int   U16;      /* defined for unsigned 16-bits integer variable 	  �޷���16λ���ͱ��� */
typedef signed   int   S16;      /* defined for signed 16-bits integer variable 	  �з���16λ���ͱ��� */
typedef unsigned long  U32;      /* defined for unsigned 32-bits integer variable 	  �޷���32λ���ͱ��� */
typedef signed   long  S32;      /* defined for signed 32-bits integer variable 	  �з���32λ���ͱ��� */
typedef float          F32;      /* single precision floating point variable (32bits) �����ȸ�������32λ���ȣ� */
typedef double         F64;      /* double precision floating point variable (64bits) ˫���ȸ�������64λ���ȣ� */
//
#define uchar unsigned char
#define uint unsigned int
#define   Data_0_time    4

extern void ShowStr(uchar x,uchar y,uchar *str);
extern void Reset1602();

//----------------------------------------------//
//----------------IO�ڶ�����--------------------//
//----------------------------------------------//
sbit P2_0  = P2^0 ;		// ��ʪ������
sbit DOUT=P2^1;			// �к�����Ũ������

sbit BUZZ = P1^6;

sbit INA = P1^0;
sbit INB = P1^1;
sbit ENA = P1^2;

//----------------------------------------------//
//----------------������--------------------//
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
	//��ʱ������forѭ��		  
	 if(U8FLAG==1)
		 break;
	//�ж�����λ��0����1	 
	   
	// ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1 
	 
	   U8comdata <<= 1;
	   U8comdata |= U8temp;        // 0 or 1
	 }//rof
   
}

	/*
	//--------------------------------
	//-----ʪ�ȶ�ȡ�ӳ��� ------------
	//--------------------------------
	//----���±�����Ϊȫ�ֱ���--------
	//----�¶ȸ�8λ== U8T_data_H------
	//----�¶ȵ�8λ== U8T_data_L------
	//----ʪ�ȸ�8λ== U8RH_data_H-----
	//----ʪ�ȵ�8λ== U8RH_data_L-----
	//----У�� 8λ == U8checkdata-----
	//----��������ӳ�������----------
	//---- Delay();, Delay_10us();,COM(); 
	//-------------------------------- ��
	*/
void R_HT(void)
{

  //��������18ms 
   P2_0=0;
   Delay(180);
   P2_0=1;
 //������������������ ������ʱ20us
   Delay_10us();
   Delay_10us();
   Delay_10us();
   Delay_10us();
 //������Ϊ���� �жϴӻ���Ӧ�ź� 
   P2_0=1;
 //�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������	  
   if(!P2_0)		 //T !	  
   {
	   	TR0 = 0;	//ֹͣ��ʱ����ʱ, ��ֹ�жϸ�����ʪ�����ݲɼ�
		TR1 = 0;
	
	   U8FLAG=2;
	 //�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����	 
	   while((!P2_0)&&U8FLAG++);
	   U8FLAG=2;
	 //�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
	   while((P2_0)&&U8FLAG++);
	 //���ݽ���״̬		 
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
		   
	 //����У�� ��
			
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
   
	TR0 = 1;	// ���´򿪶�ʱ����ʱ
	TR1 = 1;
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


void ShowTempHumi()			// ��ʾ��ʪ����Һ����
{
	
	TrasfToString(str,U8T_data_H);
	ShowStr(6,0,str);
	TrasfToString(str,U8RH_data_H);
	ShowStr(6,1,str);
}

void Alarm_Monitor()		// ��ظ�ָ��_ �¶ȡ�ʪ�ȡ��к�����Ũ��
{
	if(DOUT==0)		// ����к�����Ũ���Ƿ񳬱�
			{
				Delay(1);	// ��ʱ�ٴ�ȷ��
				if(DOUT == 0)	// ���к�����Ũ�ȳ���
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
}

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
			
			Alarm_Action();		// ���Ȼ�
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
	
	if(timer == 50)		// ��ʱ 20 X 50 ==> 1 S
	{
		timer = 0;
		flag1s = 1;
	}
}

void Timer1() interrupt 3
{
	TH1 = T1RH;
	TL1 = T1RL;
	
	if((enbuzz1 == 1) || (enbuzz2 == 1) || ((enbuzz3 == 1)))	// �򿪷����� ~BUZZ
	{
		BUZZ = ~BUZZ;
	}
	else
	{
		BUZZ = 1;
	}
	
}