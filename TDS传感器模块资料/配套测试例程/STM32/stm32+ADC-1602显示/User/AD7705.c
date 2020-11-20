
/******************** (C) COPYRIGHT 2013 CATTRACE ********************
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
 *
 * ����   :�ccattrace  QQ: 1111961
 * ����    : cattrace.tk
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
//	SysTick->LOAD=(u32)nms*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
//	SysTick->VAL =0x00;           //��ռ�����
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ����  
//	do
//	{
//		temp=SysTick->CTRL;
//	}
//	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
//	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
//	SysTick->VAL =0X00;       //��ռ�����	  	    
//} 

void AD7705_GPIO_Configuration(void)//DRDY
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
///*********ϵͳ�жϹ���**********/
//void NVIC_Configuration(void)
//{ 
//        /* Configure the NVIC Preemption Priority Bits */  
//        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);        //�������ȼ����飺��ռ���ȼ�0λ,�����ȼ�4λ
//}

/********����ϵͳʱ��,ʹ�ܸ�����ʱ��***********/
void RCC_Configuration(void)
{
                
        RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA 
                |RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO 
                |RCC_APB2Periph_SPI1, ENABLE );
}
        
/********AD7705��ʼ������***********/
//void AD7705_Init(void)
//{         
//	unsigned char i ;
//	for(i = 0; i < 100; i++)
//	{ 
//		SPIx_ReadWriteByte(0xFF);      //����DIN�ߵ�ƽд�������ָ�AD7705�ӿ�
//	}
//	SPIx_ReadWriteByte(0x20) ;       //ͨ��1 ,��һ��дʱ�ӼĴ���
//	Delay(1000); 
//	SPIx_ReadWriteByte(0x02) ;       //дʱ�ӼĴ������ø�������Ϊ200Hz
//	Delay(1000);
//	SPIx_ReadWriteByte(0x10) ;      //ͨ��1 ,��һ��д���üĴ���
//	Delay(1000);
//	SPIx_ReadWriteByte(0x44) ;       //д���üĴ��� ,���ó�˫���ԡ��޻��塢����Ϊ0���˲�����������У׼
//	Delay(1000000);
//}
void TM7705_Init(int channel)
{         
        unsigned char i ;
        for(i = 0; i < 100; i++)
				{ 
					
						SPIx_ReadWriteByte(0xFF);      //����DIN�ߵ�ƽд�������ָ�AD7705�ӿ�
					
				}
		
		if(channel==1)
		{
        SPIx_ReadWriteByte(0x20) ;     //ͨ��1 ,��һ��дʱ�ӼĴ���
        Delay(1000); 
        SPIx_ReadWriteByte(0x02) ;    //дʱ�ӼĴ������ø�������Ϊ200Hz
        Delay(1000);
        SPIx_ReadWriteByte(0x10) ;     // SPIx_ReadWriteByte(0x10) ; //ͨ��1 ,��һ��д���üĴ���
        Delay(1000);
        SPIx_ReadWriteByte(0x44) ;    //SPIx_ReadWriteByte(0x44) ;  //д���üĴ��� ,���ó�˫���ԡ��޻��塢����Ϊ0���˲�����������У׼
        Delay(1000000);
		}
   else
	 {		 
				
					  SPIx_ReadWriteByte(0x21) ;     //ͨ��2 ,��һ��дʱ�ӼĴ���
						Delay(1000); 
						SPIx_ReadWriteByte(0x02) ;    //дʱ�ӼĴ������ø�������Ϊ200Hz
						Delay(1000);
						SPIx_ReadWriteByte(0x11) ;     // SPIx_ReadWriteByte(0x10) ; //ͨ��1 ,��һ��д���üĴ���
						Delay(1000);
						SPIx_ReadWriteByte(0x44) ;    //SPIx_ReadWriteByte(0x44) ;  //д���üĴ��� ,���ó�˫���ԡ��޻��塢����Ϊ0���˲�����������У׼
						Delay(1000000);
	 }

        
}

/********��16λ����************/
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

/********��ȡAD7705��һͨ������************/
u16 ReadAD7705(u8 ch)
{
	unsigned long Ret = 0;
	u8 i;
	for(i = 0; i < 2; i++)
	{
	if(ch==1)
	{	
	SPIx_ReadWriteByte(0x38) ;       //���ö�ͨ��1����
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1 == 1));
		delay_ms(100);
		Ret = ReadAD7705_16BitValue();
	}
	else if(ch==2)
	{
	 	SPIx_ReadWriteByte(0x39) ;       //���ö�ͨ��2����
			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1 == 1));
		  delay_ms(100);
	    Ret = ReadAD7705_16BitValue();
	}
	} 
	return Ret;
}
/****************************SPI��ʼ��*************************/
                                 
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
void SPIx_Init(void)
{         
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure SPI1 pins: SCK, MISO and MOSI */ //SCLK,DIN,DOUT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure I/O for Flash Chip select */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  //SPI CS
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Deselect the FLASH: Chip Select high */
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);

	SPI_Cmd(SPI1, DISABLE); //��ʹ��SPI����	�Լ�����


	/* SPI1 configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                //����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                //ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;        //���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;                //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;        //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;            //CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);             //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

	//SPI1->CR1|=1<<6; //SPI�豸ʹ��
	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����

	SPIx_ReadWriteByte(0xff);//��������                 
}  
         

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPIx_ReadWriteByte(u8 TxData)
{                
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������
	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); //���ָ����SPI��־λ�������:���ܻ���ǿձ�־
	/* Return the byte read from the SPI bus */
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����                                    
}

/********AD7705������ʼ������***********/
void AD7705_AllInit(void)
{
	RCC_Configuration();
//		NVIC_Configuration();
  AD7705_GPIO_Configuration();
	SPIx_Init();
//	AD7705_Init();
}



