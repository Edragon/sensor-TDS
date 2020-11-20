
/********************(C) COPRIGHT 2019 Crownto electronic **************************
 * 文件名  ：ad7705.c
 * 描述    ：ad7705 应用函数库
 *          
 * 实验平台：
 * 硬件连接：-----------------
 *          |   PA0 - CS       	  |
 *          |   PA1- DRDY    		 |   
 *          |   PA5- SLCK     	 |   
 *          |   PA6 -MISO-DOUT     |
 *          |   PA7 - MOSI-DIN       |  //修改#define和GPIO定义
 *           ----------------- 
 EX:
		AD7705_Init();
 		temp = ReadAD7705();
		value = b*3.3/65535;
 * 库版本  :ST3.5.0
 * 作者    ：冠拓电子
 * 淘宝    ：https://shop376667309.taobao.com
 * 网址    ：www.crownto.cn
**********************************************************************************/
#include "stm32f10x.h"



//void AD7705_Init(void);
void AD7705_AllInit(void);
u16 ReadAD7705(u8 ch);
void NVIC_Configuration(void);
void TM7705_Init(int channel);
