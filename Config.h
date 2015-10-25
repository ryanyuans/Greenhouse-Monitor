/*
********************************************************************************
*                      	* 广东石油化工学院 *
*							* Ryan_yuans *
*						  	  * 2015.10 *
********************************************************************************
* 文件名：config.h
* 描  述：
* 版本号：v1.0.0
* 备  注：
********************************************************************************
*/

#ifndef _CONFIG_H
#define _CONFIG_H

	/* 通用头文件 */
	#include <reg52.h>
	#include <intrins.h>

	/* 数据类型定义 */
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

	/* 全局运行参数定义 */
	#define uchar unsigned char
	#define uint unsigned int
	#define Data_0_time    4
	#define SYS_MCLK  (11059200/12)		//系统主时钟频率，即振荡器频率÷12

	/* IO引脚分配定义 */

	sbit P2_0  = P2^0 ;		// 温湿度数据
	sbit DOUT = P2^1;			// 有害气体浓度数据

	sbit BUZZ = P1^6;
	
	sbit ENA = P3^7;

	/*
	sbit INA = P2^7;		// 电机驱动
	sbit INB = P2^6;
	sbit ENA = P2^5;
	*/
	sbit SOIL = P2^7;
	
	sbit LCD_RS = P1^0;
	sbit LCD_RW = P1^1;
	sbit LCD_E = P1^5;

#endif
