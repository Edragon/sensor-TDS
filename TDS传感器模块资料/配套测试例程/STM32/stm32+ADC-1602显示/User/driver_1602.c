#include "driver_1602.h"

//延时nms
void Delay_Ms(u32 t)
{
	while(t--)
	delay_nus(1100);
}

//延时nus
void delay_nus(u32 n)
{
	u8 j;
	while(n--)
	{
		j=8;
		while(j--);
	}
}
/*
//判断是否忙
u32 LCD_check_busy(void)
{
	RS_CLR;
	delay_nus(10);
	RW_SET;
	delay_nus(10);
	do
	{
		EN_CLR;
		delay_nus(200);
		EN_SET;
		delay_nus(200);
	}
	while(GPIOB->IDR&0X8000);	   //
	EN_CLR;	
	return (u32)0;
}
*/
//写命令
void LCD_write_cmd(u8 cmd)
{
//	while(LCD_check_busy()); //忙则等待
	EN_CLR;
	delay_nus(1);
	RS_CLR;
	delay_nus(1);
	RW_CLR;
	delay_nus(300);
//	
//GPIO_WriteBit(GPIOB, GPIO_Pin_10,(cmd & 0x80) >> 10 );
//GPIO_WriteBit(GPIOB, GPIO_Pin_9,(cmd & 0x40) >> 9 );
//GPIO_WriteBit(GPIOB, GPIO_Pin_8,(cmd & 0x20) >> 8 );
//GPIO_WriteBit(GPIOB, GPIO_Pin_7,(cmd & 0x10) >> 7 );
//GPIO_WriteBit(GPIOB, GPIO_Pin_6,(cmd & 0x08) >> 6 ) ;
//GPIO_WriteBit(GPIOB, GPIO_Pin_5,(cmd & 0x04) >> 5 );
//GPIO_WriteBit(GPIOB, GPIO_Pin_4,(cmd & 0x02) >> 4 );
//GPIO_WriteBit(GPIOB, GPIO_Pin_3,(cmd & 0x01)>>3);
	
//	GPIOB->BSRR = cmd<<3 & 0xff00;  //将数据送到P0口 
//  GPIOB->BRR = ((~cmd)<<3) & 0xff00;
//	DATAOUT &=~0x07F8; //0000 0111 1111 1000
//	DATAOUT |= cmd;
//	DATAOUT = 0xff00&(cmd);	 //根据所用管脚修改
	//GPIOA->ODR&=~0x01FF; //清零PA0~PA8  0000 0001 1111 1111
//GPIOA->ODR|=z;
	DATAOUT = cmd <<3;	 //根据所用管脚修改
	delay_nus(300);
	EN_SET;
	delay_nus(300);
	EN_CLR;
	delay_nus(300);
	EN_CLR;
}

//写数据
void LCD_write_data(u8 data)
{
//	while(LCD_check_busy()); //忙则等待
	Delay_Ms(10);
	EN_CLR;
	delay_nus(1);
	RS_SET;
	delay_nus(1);
	RW_CLR;
	delay_nus(1);
	
//	GPIO_WriteBit(GPIOB, GPIO_Pin_10,(data & 0x80) >> 10 );
//GPIO_WriteBit(GPIOB, GPIO_Pin_9,(data & 0x40) >> 9 );
//GPIO_WriteBit(GPIOB, GPIO_Pin_8,(data & 0x20) >> 8 );
//GPIO_WriteBit(GPIOB, GPIO_Pin_7,(data & 0x10) >> 7 );
//GPIO_WriteBit(GPIOB, GPIO_Pin_6,(data & 0x08) >> 6 ) ;
//GPIO_WriteBit(GPIOB, GPIO_Pin_5,(data & 0x04) >> 5 );
//GPIO_WriteBit(GPIOB, GPIO_Pin_4,(data & 0x02) >> 4 );
//GPIO_WriteBit(GPIOB, GPIO_Pin_3,(data & 0x01)>>3);
	
//	GPIOB->BSRR = data<<3 & 0xff00;    ////P0 = dat
//  GPIOB->BRR = ((~data)<<3) & 0xff00;
	
	//GPIOB_ODR &= ~(1<<0);
	//GPIOB_ODR |= (0<<0);
//	DATAOUT =data;
//	DATAOUT =(data<<3) & 0xff00;//置GPIOD->BSRR低16位的某位为'1'，则对应的I/O端口置'1'；
//		DATAOUT &=~0x07F8; //0000 0111 1111 1000
	  DATAOUT = data<<3;
//    //根据所用引脚不同修改      //而置GPIOD->BSRR低16位的某位为'0'，则对应的I/O端口不变。
	delay_nus(1);
	EN_SET;
	Delay_Ms(1);
	EN_CLR;
}
//清屏
void LCD_clr(void)
{
	LCD_write_cmd(0x01);
}

void LCD_pos(u8 x,u8 y) //显示位置
{
	if(y)
		LCD_write_cmd(x|0xc0);
	else
		LCD_write_cmd(x|0x80);
}

//显示字符
void LCD_printchar(u8 x,u8 y,u8 c)
{
	LCD_pos(x,y);
	LCD_write_data(c);
}

//显示字符串
void LCD_printstring(u8 x,u8 y,u8 *s)
{
	LCD_pos(x,y);
	while(*s!='\0')
	{
		LCD_write_data(*s);
		s++;
		Delay_Ms(1);
	}
}

//初始化
void LCD_init(void)
{
	Delay_Ms(50);
	LCD_write_cmd(0x38);  //八位数据接口  两行显示   5X7  点阵
	Delay_Ms(10);
	LCD_write_cmd(0x38);  //八位数据接口  两行显示   5X7  点阵
	Delay_Ms(10);
	LCD_write_cmd(0x38);  //八位数据接口  两行显示   5X7  点阵
	Delay_Ms(10);
	LCD_write_cmd(0x08);  //先开显示，后关显示
	Delay_Ms(10);
	LCD_write_cmd(0x01); //显示清屏
	Delay_Ms(10);
	LCD_write_cmd(0x06); //显示光标移动设置
	Delay_Ms(10);
	LCD_write_cmd(0x0c); //显示打开及光标设置
	Delay_Ms(10);	 
}
