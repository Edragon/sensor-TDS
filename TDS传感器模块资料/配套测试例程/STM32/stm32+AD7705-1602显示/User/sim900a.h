#ifndef _SIM900A_H_
#define _SIM900A_H_

#include "stm32f10x.h"
#include "bsp_usart2.h"

static __IO u32 TimingDelay;

//#include "ff.h"

typedef enum{
    SIM900A_TRUE,
    SIM900A_FALSE,
    
}sim900a_res_e;

typedef enum
{
    SIM900A_NULL                = 0,
    SIM900A_CMD_SEND            = '\r',         
    SIM900A_DATA_SEND           = 0x1A,         //发送数据(ctrl + z)
    SIM900A_DATA_CANCLE         = 0x1B,         //发送数据(Esc)
    
    
}sim900a_cmd_end_e;




/*模块控制配置，移植到其它芯片时请修改这些宏*/

#define     SIM900A_TX(cmd)                 sim900a_tx_printf("%s",cmd)   //发送字符串
#define     SIM900A_IS_RX()                 (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET) //获取串口数据寄存器状态

#define     SIM900A_RX(len)                 ((char *)get_rebuff(&(len)))  //获取串口接收到的数据长度


#define     SIM900A_DELAY(time)             Delay_ms(time)                 //延时

#define     SIM900A_SWAP16(data)            __REVSH(data)                  //交换16位高低字节


//                  指令             正常返回
//本机号码          AT+CNUM\r         +CNUM: "","13265xxxxxx",129,7,4            //很多SIM卡默认都是没设置本机号码的，解决方法如下 http://www.multisilicon.com/blog/a21234642.html
//SIM营运商         AT+COPS?\r        +COPS: 0,0,"CHN-UNICOM"   OK
//SIM卡状态         AT+CPIN?\r        +CPIN: READY   OK
//SIM卡信号强度     AT+CSQ\r          +CSQ: 8,0   OK

extern  uint8_t     sim900a_cmd         (char *cmd, char *reply,uint32_t waittime );
extern  uint8_t     sim900a_cmd_check   (char *reply);

#if 1
#define     sim900a_tx_printf(fmt, ...)         USART2_printf(USART2,fmt,##__VA_ARGS__)     //printf格式发送命令（纯发送，不进行任何接收数据处理）
#else
#define     sim900a_tx_printf(fmt, ...)         printf(fmt,##__VA_ARGS__)     //printf格式发送命令（纯发送，不进行任何接收数据处理）
#endif

#define     SIM900A_CLEAN_RX()                  clean_rebuff()
#define     sim900a_ate0()                      sim900a_cmd("ATE0\r","OK",100)              //关闭回显

/*************************** 电话 功能 ***************************/
extern  uint8_t     sim900a_cnum                (char *num);                        //获取本机号码
extern  void        sim900a_call                (char *num);                        //发起拨打电话（不管接不接通）
extern  void        sim900a_sms                 (char *num,char *smstext);          //发送短信（支持中英文,中文为GBK码）

extern  char *      sim900a_waitask             (uint8_t waitask_hook(void));       //等待有数据应答，返回接收缓冲区地址

extern  void        sim900a_gbk2ucs2            (char * ucs2,char * gbk);           



/*************************** GPRS 功能 ***************************/
extern  void        sim900a_gprs_init           (void);                                                 //GPRS初始化环境
extern  void        sim900a_gprs_tcp_link       (char *localport,char * serverip,char * serverport);    //TCP连接
extern  void        sim900a_gprs_udp_link       (char *localport,char * serverip,char * serverport);    //UDP连接
extern  void        sim900a_gprs_send           (char *str);                                            //发送数据
extern  void        sim900a_gprs_link_close     (void);                                                 //IP链接断开

#endif

