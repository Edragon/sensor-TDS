
#ifndef _1602_H
#define _1602_H
#include <reg52.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char
#define Busy			0x80					//æ�ź�
#define  LCD_Data 	    P2	
#define Write				0x00					//�͵�ƽд��
#define Read				0x01					//�ߵ�ƽ����
#define Data				0x01					//�ߵ�ƽѡ������
#define Cmd				    0x00					//�͵�ƽѡ������

#define Enable		    	0x00					//Ծ�䵽�͵�ƽʱִ������
#define Disable		    	0x01    
#define True				0x01
#define False				0x00
#define LCD_Init			0x38					//��ʼ��ģʽ							
#define LCD_DispCtr    		0x0C					//����ʾ���������
#define LCD_CloseCtr		0x08					//����ʾ
#define LCD_CLS        		0x01					//����Ļ
#define LCD_EnterSet   		0x06					//��ʾ��� 

sbit LCD_RS  			=P0^0;					//��������ѡ���
sbit LCD_RW  			=P0^1; 				  //��д���ƶ�
sbit LCD_E   			=P0^2;					//ִ��ʹ�ܶ�


/*-------------------------------  LCDģ�� -------------------------------------*/
void LCDInit(void);								//LCD��ʼ��
void SetWriteCmd(void);								//����д����ģʽ
void SetReadCmd(void);								//���ö�����ģʽ
void SetWriteData(void);							//����д����ģʽ
void WriteCmd(char cmd);							//д����
void WriteData(char ddata);							//д����
void ExecuteCmd(void);								//ִ������
void SetXY(char x,char y);							//��λ��ʾ��ַ
void DisplaySingleChar(char x,char y,char cchar);		//��ʾ�����ַ�
void DisplayString(char x,char y,char *str); 			//��ʾһ���ַ���
void Delay(unsigned int time);						//��ʱ������
void DelayUs(unsigned int time);						//��ʱ�ӳ���
//void delay500ms(void) ;
bit  IsBusy(void);									//�ж�æ��־����
void DisplayTime(void);								//��ʾʱ��
void DisplayAVGSpeed(void);							//��ʾƽ���ٶ�
void DisplayDistance(void);							//��ʾ·��
void DelayUs(unsigned int time);	//��ʱ����



#endif