#ifndef __DRIVER_1602_H__
#define __DRIVER_1602_H__

#include "stm32f10x.h"

//定义数据命令选择端
#define RS_SET GPIO_SetBits(GPIOA,GPIO_Pin_11)
#define RS_CLR GPIO_ResetBits(GPIOA,GPIO_Pin_11)
//读写控制端
#define RW_CLR GPIO_ResetBits(GPIOA,GPIO_Pin_12)
#define RW_SET GPIO_SetBits(GPIOA,GPIO_Pin_12)
//使能端
#define EN_CLR GPIO_ResetBits(GPIOA,GPIO_Pin_15)
#define EN_SET GPIO_SetBits(GPIOA,GPIO_Pin_15)

#define DATAOUT GPIOB->ODR

void Delay_Ms(u32 t);	//毫秒级延时
void delay_nus(u32 n);//微秒级延时
//u32 LCD_check_busy(void);		//忙检测
void LCD_write_com(u8 com);	//写命令
void LCD_write_data(u8 data);	//写数据
void LCD_clr(void);				//清屏
void LCD_pos(u8 x,u8 y);        //显示位置
void LCD_printchar(u8 x,u8 y,u8 c);//输出字符
void LCD_printstring(u8 x,u8 y,u8 *s);//输出字符串
void LCD_init(void);			 //LCD初始化

#endif
