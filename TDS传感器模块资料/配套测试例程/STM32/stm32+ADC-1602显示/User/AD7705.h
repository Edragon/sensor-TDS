
/******************** (C) COPYRIGHT 2013 CATTRACE ********************
 * �ļ���  ��ad7705.c
 * ����    ��ad7705 Ӧ�ú�����
 *          
 * ʵ��ƽ̨��
 * Ӳ�����ӣ�-----------------
 *          |   PA2 - CS       	  |
 *          |   PA4- DRDY    		 |   
 *          |   PA5- SLCK     	 |   
 *          |   PA6 -MISO-DOUT     |
 *          |   PA7 - MOSI-DIN       |  //�޸�#define��GPIO����
 *           ----------------- 
 * ��汾  :ST3.5.0
 *
 * ����   :�ccattrace  QQ: 1111961
 * ����    : cattrace.tk
**********************************************************************************/
#include "stm32f10x.h"



//void AD7705_Init(void);
void AD7705_AllInit(void);
u16 ReadAD7705(u8 ch);
void NVIC_Configuration(void);
void TM7705_Init(int channel);
