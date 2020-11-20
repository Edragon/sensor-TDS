
#ifndef _1602_H
#define _1602_H
#include <reg52.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char
#define Busy			0x80					//忙信号
#define  LCD_Data 	    P2	
#define Write				0x00					//低电平写入
#define Read				0x01					//高电平读出
#define Data				0x01					//高电平选择数据
#define Cmd				    0x00					//低电平选择命令

#define Enable		    	0x00					//跃变到低电平时执行命令
#define Disable		    	0x01    
#define True				0x01
#define False				0x00
#define LCD_Init			0x38					//初始化模式							
#define LCD_DispCtr    		0x0C					//开显示及光标设置
#define LCD_CloseCtr		0x08					//关显示
#define LCD_CLS        		0x01					//清屏幕
#define LCD_EnterSet   		0x06					//显示光标 

sbit LCD_RS  			=P0^0;					//数据命令选择端
sbit LCD_RW  			=P0^1; 				  //读写控制端
sbit LCD_E   			=P0^2;					//执行使能端


/*-------------------------------  LCD模块 -------------------------------------*/
void LCDInit(void);								//LCD初始化
void SetWriteCmd(void);								//设置写命令模式
void SetReadCmd(void);								//设置读命令模式
void SetWriteData(void);							//设置写数据模式
void WriteCmd(char cmd);							//写命令
void WriteData(char ddata);							//写数据
void ExecuteCmd(void);								//执行命令
void SetXY(char x,char y);							//定位显示地址
void DisplaySingleChar(char x,char y,char cchar);		//显示单个字符
void DisplayString(char x,char y,char *str); 			//显示一段字符串
void Delay(unsigned int time);						//延时主程序
void DelayUs(unsigned int time);						//延时子程序
//void delay500ms(void) ;
bit  IsBusy(void);									//判断忙标志函数
void DisplayTime(void);								//显示时间
void DisplayAVGSpeed(void);							//显示平均速度
void DisplayDistance(void);							//显示路程
void DelayUs(unsigned int time);	//延时函数



#endif