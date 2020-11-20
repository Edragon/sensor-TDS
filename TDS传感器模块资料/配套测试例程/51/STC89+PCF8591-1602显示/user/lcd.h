#ifndef _LCD_H
#define _LCD_H

 /********************************************************************************\
**                         			宏定义区									**
\********************************************************************************/
/*-------------------------------  LCD模块 ------------------------------------*/
#define Busy			0x80					//忙信号
#define LCD_Data 	  P0	      //数据口定义
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

 /********************************************************************************\
**                         			函数声明									**
\********************************************************************************/
extern void LCDInit(void);								//LCD初始化
void SetWriteCmd(void);								//设置写命令模式
void SetReadCmd(void);								//设置读命令模式
void SetWriteData(void);							//设置写数据模式
extern void WriteCmd(char cmd);							//写命令
void WriteData(char ddata);							//写数据
void ExecuteCmd(void);								//执行命令
void SetXY(char x,char y);							//定位显示地址
extern void DisplaySingleChar(char x,char y,char cchar);		//显示单个字符
extern void DisplayString(char x,char y,char *str); 			//显示一段字符串
void Delay(unsigned int time);						//延时主程序
void DelayUs(unsigned int time);						//延时子程序
bit  IsBusy(void);									//判断忙标志函数
extern void delay_1s();	
#endif