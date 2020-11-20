/******************** (C) COPYRIGHT 2015 Silicone shell electronic studio **************************
 * 文件名  ：Lcd1602.c
 * 描述    : 1602液晶显示屏库函数        
 * 硬件连接： EN:P2.5
 *						RW:P2.6
 *					  RS:P2.7
 *					  D0~D7:P0口
 *
 * 实验平台：SHELL1.0开发板
 *
 * 作者    ：Silicone shell electronic studio
 * 淘宝    ：http://shop111698447.taobao.com/shop/view_shop.htm?spm=a1z0e.1.10010.5.GCUiZL
**********************************************************************************/
#include "lcd.h"
#include <intrins.h>
#include <reg52.h>

/*********************LCD1602端口定义***************************/
sbit LCD_E   			=P2^2;					//执行使能端
sbit LCD_RW  			=P2^1; 				  //读写控制端
sbit LCD_RS  			=P2^0;					//数据命令选择端

/*
 * 函数名：delay_1s
 * 描述  ：延时函数1s
 *         
 * 输入  ：unsigned int time：延时量
 * 输出  ：无
 * 说明  ：液晶显示用延时函数
 */
void delay_1s()	  //延时1s
{
	unsigned char i, j, k;

	_nop_();
	i = 8;
	j = 1;
	k = 243;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}		
/*
 * 函数名：Delay
 * 描述  ：延时函数
 *         
 * 输入  ：unsigned int time：延时量
 * 输出  ：无
 * 说明  ：
 */
void Delay(unsigned int time)
{
    unsigned int timeCounter = 0;
	for (timeCounter = time;timeCounter > 0 ;timeCounter --)
		DelayUs(255);
}
/*
 * 函数名：DelayUs
 * 描述  ：延时函数（微秒）
 *         
 * 输入  ：unsigned int time：延时量
 * 输出  ：无
 * 说明  ：
 */
void DelayUs(unsigned int time)
{
	unsigned int timeCounter = 0;
	for (timeCounter = 0;timeCounter < time;timeCounter ++)
		_nop_();
}

/****************************************************************************************\
**								   LCD驱动模块								**
\****************************************************************************************/

/*
 * 函数名：LCDInit
 * 描述  ：液晶初始化函数
 *         
 * 输入  ：无
 * 输出  ：无
 * 说明  ：
 */
void LCDInit(void)
{
    //三次显示模式设置
	LCD_Data=0;
	LCD_E=Disable;
	Delay(5);
	WriteCmd(LCD_Init);
	Delay(5);
	WriteCmd(LCD_Init);
	Delay(5);
	WriteCmd(LCD_Init);

	WriteCmd(LCD_Init);	   						//初始化
	WriteCmd(LCD_CloseCtr);							//关显示
	WriteCmd(LCD_CLS);								//清屏幕
	WriteCmd(LCD_EnterSet);							//光标移动设置
	WriteCmd(LCD_DispCtr);							//显示开以及光标设置
	
}

/*---------------------------------  LCD模式设置函数--------------------------------------*\
	
	SetWriteCmd() 	 			设置LCD为写命令模式
	SetReadCmd()   		    		设置LCD为读命令模式
	SetWriteData()				设置LCD为写数据模式
	
\*----------------------------------------------------------------------------------------*/
/*
 * 函数名：SetWriteCmd
 * 描述  ：设置LCD为写命令模式
 *         
 * 输入  ：无
 * 输出  ：无
 * 说明  ：
 */
void SetWriteCmd(void)
{
	LCD_RW=Write;
	LCD_RS=Cmd;
}
/*
 * 函数名：SetReadCmd
 * 描述  ：设置LCD为读命令模式
 *         
 * 输入  ：无
 * 输出  ：无
 * 说明  ：
 */
void SetReadCmd(void)
{
	LCD_RW=Read;
	LCD_RS=Cmd;
}
/*
 * 函数名：SetWriteData
 * 描述  ：设置LCD为写数据模式
 *         
 * 输入  ：无
 * 输出  ：无
 * 说明  ：
 */
void SetWriteData(void)
{
	LCD_RW=Write;
	LCD_RS=Data;
}

/*---------------------------------  LCD功能执行函数--------------------------------------*\
	
	WriteCmd()					写命令
	WriteData()					写数据
	ExecuteCmd()					执行命令
	SetXY()						显示定位
	DisplaySingleChar()			显示单个字符
	DisplayString()				显示一串字符
	IsBusy()						忙标志检测
\*----------------------------------------------------------------------------------------*/
/*
 * 函数名：WriteCmd
 * 描述  ：写命令
 *         
 * 输入  ：无
 * 输出  ：无
 * 说明  ：
 */
void WriteCmd(char cmd)
{
	while(IsBusy());
	LCD_Data=cmd;	
	SetWriteCmd();
	ExecuteCmd();
}
/*
 * 函数名：WriteData
 * 描述  ：写数据
 *         
 * 输入  ：无
 * 输出  ：无
 * 说明  ：
 */
void WriteData(char ddata)
{
	while(IsBusy());
	LCD_Data=ddata;
	SetWriteData();	
	ExecuteCmd();
}
/*
 * 函数名：ExecuteCmd
 * 描述  ：执行命令
 *         
 * 输入  ：无
 * 输出  ：无
 * 说明  ：
 */
void ExecuteCmd(void)
{
	LCD_E=Enable;
	LCD_E=Disable;
}
/*
 * 函数名：SetXY
 * 描述  ：显示定位
 *         
 * 输入  ：无
 * 输出  ：无
 * 说明  ：
 */
void SetXY(char x,char y)
{
	if (y)
	x|=0x40;
	x|=0x80;
	Delay(5);
	WriteCmd(x);

}
/*
 * 函数名：DisplaySingleChar
 * 描述  ：显示单个字符
 *         
 * 输入  ：无
 * 输出  ：无
 * 说明  ：
 */
void DisplaySingleChar(char x,char y,char cchar)
{
	SetXY(x,y);
	WriteData(cchar);	
}
/*
 * 函数名：DisplayString
 * 描述  ：显示字符串
 *         
 * 输入  ：无
 * 输出  ：无
 * 说明  ：
 */
void DisplayString(char x,char y,char *str)
{
	while(*str)
	{
		Delay(5);
		DisplaySingleChar(x++,y,*str);
		str++;
	}
}
/*
 * 函数名：IsBusy
 * 描述  ：忙标志检测
 *         
 * 输入  ：无
 * 输出  ：无
 * 说明  ：
 */
bit IsBusy(void)
{
	LCD_Data=0xFF;
	SetReadCmd();
	ExecuteCmd();
//	return (bit)(P0 & 0x80);	
		return (bit)(LCD_Data & 0x80);	
}