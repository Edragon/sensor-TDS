
/******************** (C) COPYRIGHT 2013 CATTRACE ********************
 * 文件名  ：ad7705.c
 * 描述    ：ad7705 应用函数库
 *          
 * 实验平台：
 * 硬件连接：-----------------
 *          |   PA2 - CS       	  |
 *          |   PA4- DRDY    		 |   
 *          |   PA5- SLCK     	 |   
 *          |   PA6 -MISO-DOUT     |
 *          |   PA7 - MOSI-DIN       |  //修改#define和GPIO定义
 *           ----------------- 
 * 库版本  :ST3.5.0
 *
 * 作者   :cattrace  QQ: 1111961
 * 博客    : cattrace.tk
**********************************************************************************/
#include "stm32f10x.h"



//void AD7705_Init(void);
void AD7705_AllInit(void);
u16 ReadAD7705(u8 ch);
void NVIC_Configuration(void);
void TM7705_Init(int channel);
