
/********************(C) COPRIGHT 2019 Crownto electronic **************************
 * �ļ���  ��ad7705.c
 * ����    ��ad7705 Ӧ�ú�����
 *          
 * ʵ��ƽ̨��
 * Ӳ�����ӣ�-----------------
 *          |   PA0 - CS       	  |
 *          |   PA1- DRDY    		 |   
 *          |   PA5- SLCK     	 |   
 *          |   PA6 -MISO-DOUT     |
 *          |   PA7 - MOSI-DIN       |  //�޸�#define��GPIO����
 *           ----------------- 
 EX:
		AD7705_Init();
 		temp = ReadAD7705();
		value = b*3.3/65535;
 * ��汾  :ST3.5.0
 * ����    �����ص���
 * �Ա�    ��https://shop376667309.taobao.com
 * ��ַ    ��www.crownto.cn
**********************************************************************************/
#include "stm32f10x.h"



//void AD7705_Init(void);
void AD7705_AllInit(void);
u16 ReadAD7705(u8 ch);
void NVIC_Configuration(void);
void TM7705_Init(int channel);
