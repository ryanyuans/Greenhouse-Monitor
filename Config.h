/*
********************************************************************************
*                      	* �㶫ʯ�ͻ���ѧԺ *
*							* Ryan_yuans *
*						  	  * 2015.10 *
********************************************************************************
* �ļ�����config.h
* ��  ����
* �汾�ţ�v1.0.0
* ��  ע��
********************************************************************************
*/

#ifndef _CONFIG_H
#define _CONFIG_H

	/* ͨ��ͷ�ļ� */
	#include <reg52.h>
	#include <intrins.h>

	/* �������Ͷ��� */
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

	/* ȫ�����в������� */
	#define uchar unsigned char
	#define uint unsigned int
	#define Data_0_time    4
	#define SYS_MCLK  (11059200/12)		//ϵͳ��ʱ��Ƶ�ʣ�������Ƶ�ʡ�12

	/* IO���ŷ��䶨�� */

	sbit P2_0  = P2^0 ;		// ��ʪ������
	sbit DOUT = P2^1;			// �к�����Ũ������

	sbit BUZZ = P1^6;
	
	sbit ENA = P3^7;

	/*
	sbit INA = P2^7;		// �������
	sbit INB = P2^6;
	sbit ENA = P2^5;
	*/
	sbit SOIL = P2^7;
	
	sbit LCD_RS = P1^0;
	sbit LCD_RW = P1^1;
	sbit LCD_E = P1^5;

#endif
