#ifndef __DRIVER_1602_H__
#define __DRIVER_1602_H__

#include "stm32f10x.h"

//������������ѡ���
#define RS_SET GPIO_SetBits(GPIOA,GPIO_Pin_11)
#define RS_CLR GPIO_ResetBits(GPIOA,GPIO_Pin_11)
//��д���ƶ�
#define RW_CLR GPIO_ResetBits(GPIOA,GPIO_Pin_12)
#define RW_SET GPIO_SetBits(GPIOA,GPIO_Pin_12)
//ʹ�ܶ�
#define EN_CLR GPIO_ResetBits(GPIOA,GPIO_Pin_15)
#define EN_SET GPIO_SetBits(GPIOA,GPIO_Pin_15)

#define DATAOUT GPIOB->ODR

void Delay_Ms(u32 t);	//���뼶��ʱ
void delay_nus(u32 n);//΢�뼶��ʱ
//u32 LCD_check_busy(void);		//æ���
void LCD_write_com(u8 com);	//д����
void LCD_write_data(u8 data);	//д����
void LCD_clr(void);				//����
void LCD_pos(u8 x,u8 y);        //��ʾλ��
void LCD_printchar(u8 x,u8 y,u8 c);//����ַ�
void LCD_printstring(u8 x,u8 y,u8 *s);//����ַ���
void LCD_init(void);			 //LCD��ʼ��

#endif
