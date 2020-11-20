
/******************** (C) COPYRIGHT 2013 CATTRACE ********************
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
 *
 * 作者   :cattrace  QQ: 1111961
 * 博客    : cattrace.tk
**********************************************************************************/
#include "stm32f10x.h"
#include "AD7705.h"
#include "stdio.h"
#include "delay.h"
#include "stm32f10x_spi.h"


u8 SPIx_ReadWriteByte(u8 TxData);
void RCC_Configuration(void);
void SPIx_Init(void);

void Delay(vu32 nCount)
{
        for(; nCount != 0; nCount--);
}

//void delay_ms(u16 nms)
//{	 		  	  
//	u32 temp;		   
//	SysTick->LOAD=(u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
//	SysTick->VAL =0x00;           //清空计数器
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数  
//	do
//	{
//		temp=SysTick->CTRL;
//	}
//	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
//	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
//	SysTick->VAL =0X00;       //清空计数器	  	    
//} 

void AD7705_GPIO_Configuration(void)//DRDY
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
///*********系统中断管理**********/
//void NVIC_Configuration(void)
//{ 
//        /* Configure the NVIC Preemption Priority Bits */  
//        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);        //设置优先级分组：先占优先级0位,从优先级4位
//}

/********配置系统时钟,使能各外设时钟***********/
void RCC_Configuration(void)
{
                
        RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA 
                |RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO 
                |RCC_APB2Periph_SPI1, ENABLE );
}
        
/********AD7705初始化函数***********/
//void AD7705_Init(void)
//{         
//	unsigned char i ;
//	for(i = 0; i < 100; i++)
//	{ 
//		SPIx_ReadWriteByte(0xFF);      //持续DIN高电平写操作，恢复AD7705接口
//	}
//	SPIx_ReadWriteByte(0x20) ;       //通道1 ,下一个写时钟寄存器
//	Delay(1000); 
//	SPIx_ReadWriteByte(0x02) ;       //写时钟寄存器设置更新速率为200Hz
//	Delay(1000);
//	SPIx_ReadWriteByte(0x10) ;      //通道1 ,下一个写设置寄存器
//	Delay(1000);
//	SPIx_ReadWriteByte(0x44) ;       //写设置寄存器 ,设置成双极性、无缓冲、增益为0、滤波器工作、自校准
//	Delay(1000000);
//}
void TM7705_Init(int channel)
{         
        unsigned char i ;
        for(i = 0; i < 100; i++)
				{ 
					
						SPIx_ReadWriteByte(0xFF);      //持续DIN高电平写操作，恢复AD7705接口
					
				}
		
		if(channel==1)
		{
        SPIx_ReadWriteByte(0x20) ;     //通道1 ,下一个写时钟寄存器
        Delay(1000); 
        SPIx_ReadWriteByte(0x02) ;    //写时钟寄存器设置更新速率为200Hz
        Delay(1000);
        SPIx_ReadWriteByte(0x10) ;     // SPIx_ReadWriteByte(0x10) ; //通道1 ,下一个写设置寄存器
        Delay(1000);
        SPIx_ReadWriteByte(0x44) ;    //SPIx_ReadWriteByte(0x44) ;  //写设置寄存器 ,设置成双极性、无缓冲、增益为0、滤波器工作、自校准
        Delay(1000000);
		}
   else
	 {		 
				
					  SPIx_ReadWriteByte(0x21) ;     //通道2 ,下一个写时钟寄存器
						Delay(1000); 
						SPIx_ReadWriteByte(0x02) ;    //写时钟寄存器设置更新速率为200Hz
						Delay(1000);
						SPIx_ReadWriteByte(0x11) ;     // SPIx_ReadWriteByte(0x10) ; //通道1 ,下一个写设置寄存器
						Delay(1000);
						SPIx_ReadWriteByte(0x44) ;    //SPIx_ReadWriteByte(0x44) ;  //写设置寄存器 ,设置成双极性、无缓冲、增益为0、滤波器工作、自校准
						Delay(1000000);
	 }

        
}

/********读16位数据************/
u16 ReadAD7705_16BitValue(void)
{ 
	unsigned long DataL = 0;
	unsigned long DataH = 0;
	unsigned long Ret = 0;

	DataH = SPIx_ReadWriteByte(0xFF);
	DataH = DataH << 8; 
	DataL = SPIx_ReadWriteByte(0xFF); 
	Ret          = DataH | DataL; 

	return(Ret) ;                             
}

/********读取AD7705第一通道数据************/
u16 ReadAD7705(u8 ch)
{
	unsigned long Ret = 0;
	u8 i;
	for(i = 0; i < 2; i++)
	{
	if(ch==1)
	{	
	SPIx_ReadWriteByte(0x38) ;       //设置读通道1数据
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1 == 1));
		delay_ms(100);
		Ret = ReadAD7705_16BitValue();
	}
	else if(ch==2)
	{
	 	SPIx_ReadWriteByte(0x39) ;       //设置读通道2数据
			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1 == 1));
		  delay_ms(100);
	    Ret = ReadAD7705_16BitValue();
	}
	} 
	return Ret;
}
/****************************SPI初始化*************************/
                                 
//SPI口初始化
//这里针是对SPI1的初始化
void SPIx_Init(void)
{         
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure SPI1 pins: SCK, MISO and MOSI */ //SCLK,DIN,DOUT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure I/O for Flash Chip select */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  //SPI CS
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Deselect the FLASH: Chip Select high */
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);

	SPI_Cmd(SPI1, DISABLE); //不使能SPI外设	自己加上


	/* SPI1 configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                //设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                //设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                //选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;        //数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;                //定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;            //CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);             //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

	//SPI1->CR1|=1<<6; //SPI设备使能
	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设

	SPIx_ReadWriteByte(0xff);//启动传输                 
}  
         

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPIx_ReadWriteByte(u8 TxData)
{                
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); //检查指定的SPI标志位设置与否:发送缓存空标志位
	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据
	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); //检查指定的SPI标志位设置与否:接受缓存非空标志
	/* Return the byte read from the SPI bus */
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据                                    
}

/********AD7705完整初始化函数***********/
void AD7705_AllInit(void)
{
	RCC_Configuration();
//		NVIC_Configuration();
  AD7705_GPIO_Configuration();
	SPIx_Init();
//	AD7705_Init();
}



