/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2014-08-xx
  * @brief   WF-SIM900A模块驱动程序
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 ISO-STM32 开发板
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
	*/
  
#include <stdarg.h>
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include "sim900a.h"
#include <string.h>




/**
 * @brief  sim900a_cmd  发送命令
 * @param  cmd：命令字符串指针，reply：希望得到的命令响应，waittime：命令等待的时间
 * @retval 命令是否得到正常响应，正常SIM900A_TRUE，不正常：SIM900A_FALSE
 */
uint8_t sim900a_cmd(char *cmd, char *reply,uint32_t waittime )
{    
    SIM900A_CLEAN_RX();                 //清空了接收缓冲区数据

    SIM900A_TX(cmd);                    //发送命令

    if(reply == 0)                      //不需要接收数据
    {
        return SIM900A_TRUE;
    }
    
    SIM900A_DELAY(waittime);                 //延时
    
    
    return sim900a_cmd_check(reply);    //对接收数据进行处理
}



/**
 * @brief  sim900a_cmd_check  检测命令的响应
 * @param  reply命令响应的字符串指针
 * @retval 检测到匹配的响应SIM900A_TRUE，没检测到匹配的响应SIM900A_FALSE
 */
uint8_t sim900a_cmd_check(char *reply)
{
    uint8_t len;

    char *redata;
    
    redata = SIM900A_RX(len);   //接收数据

    if(strstr(redata,reply) != NULL)
    {    
      return SIM900A_TRUE;        //查找到匹配的响应内容
    }
    else
    {
      return SIM900A_FALSE;       

    }
    
 
}


/**
 * @brief  sim900a_waitask  等待数据应答 
 * @param  waitask_hook，检测事件勾子函数，若产生事件，退出本函数。
 * @retval 接收到的应答数据指针
 */
char * sim900a_waitask(uint8_t waitask_hook(void))      //等待有数据应答
{
    uint8_t len;
    char *redata;
    do{
        redata = SIM900A_RX(len);   //接收数据
        if(waitask_hook!=0)
        {
            if(waitask_hook()==SIM900A_TRUE)           //返回 SIM900A_TRUE 表示检测到事件，需要退出
            {
                redata = 0;
                return redata;               
            }
        }
    }while(len==0);                 //接收数据为0时一直等待
    
    
    SIM900A_DELAY(20);              //延时，确保能接收到全部数据（115200波特率下，每ms能接收11.52个字节）
    return redata;
}



/**
 * @brief  sim900a_cnum 
 * @param  num:存储本机号码的指针
 * @retval 返回SIM900A_FALSE表示获取失败，返回SIM900A_TRUE表示成功
 */
uint8_t sim900a_cnum(char *num)
{
    char *redata;
    uint8_t len;
    
    if(sim900a_cmd("AT+CNUM\r","OK", 1000) != SIM900A_TRUE)
    {
        return SIM900A_FALSE;
    }
    
    redata = SIM900A_RX(len);   //接收数据
    
    if(len == 0)
    {
        return SIM900A_FALSE;
    }
    
    //第一个逗号后面的数据为:"本机号码"
    while(*redata != ',')
    {
        len--;
        if(len==0)
        {
            return SIM900A_FALSE;
        }
        redata++;
    }
    redata+=2;
    
    while(*redata != '"')
    {
        *num++ = *redata++;
    }
    *num = 0;               //字符串结尾需要清0
    return SIM900A_TRUE;
}




/**
 * @brief  sim900a_call 
 * @param  num:要拨打的电话号码
 * @retval 无
 */
void sim900a_call(char *num)
{
    SIM900A_CLEAN_RX();                 //清空了接收缓冲区数据
    
    sim900a_tx_printf("ATD%s;\r",num);

    //拨打后是不返回数据的
    //不管任何应答，开头都是"\r\n",即真正有效的数据是从第3个字节开始
    //对方挂掉电话（没接通），返回：BUSY
    //对方接听了电话：+COLP: "555",129,"",0,"9648674F98DE"   OK
    //对方接听了电话,然后挂掉：NO CARRIER
    //对方超时没接电话：NO ANSWER
}


/**
 * @brief  IsASSIC 判断是否纯ASSIC编码
 * @param  str: 字符串指针
 * @retval 纯ASSIC编码返回TRUE，非纯ASSIC编码返回FALSE
 */
uint8_t IsASSIC(char * str)
{
    while(*str)
    {
        if(*str>0x7F)
        {
            return SIM900A_FALSE;
        }
        str++;
    }
    
    return SIM900A_TRUE;
}


/**
 * @brief  sim900a_gbk2ucs2 把GBK编码转换成UCS2编码
 * @param  ucs2: ucs2字符串指针，gbk：GBK字符串指针
 * @retval 无
 */
//void sim900a_gbk2ucs2(char * ucs2,char * gbk)
//{
//    WCHAR   tmp;
//    
//    while(*gbk)
//    {
//        if((*gbk&0xFF) < 0x7F)      //英文
//        {
//            
//            *ucs2++ = 0;
//            *ucs2++ = *gbk++;  
//        }
//        else                        //中文
//        {
//            tmp = SIM900A_SWAP16(*(WCHAR *)gbk);
//            
//            *(WCHAR *)ucs2 = SIM900A_SWAP16(ff_convert(tmp,1));
//            gbk+=2;
//            ucs2+=2;
//        }
//    }
//}

/**
 * @brief  sim900a_char2hex 把字符转换成16进制字符
 * @param  hex: 16进制字符存储的位置指针，ch：字符
 * @retval 无
 */
void sim900a_char2hex(char *hex,char ch)
{
    const char numhex[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    *hex++  = numhex[(ch & 0xF0)>>4];
    *hex    = numhex[ ch & 0x0F];
}

///**
// * @brief  sim900a_gbk2ucs2hex GBK编码转ucs2编码再转成16进制字符
// * @param  ucs2hex: 16进制数存储的位置指针，gbk：字符
// * @retval 无
// */
//void sim900a_gbk2ucs2hex(char * ucs2hex,char * gbk)
//{
//    WCHAR   tmp;
//    
//    while(*gbk)
//    {
//        if((*gbk&0xFF) < 0x7F)      //英文
//        {
//            
//            *ucs2hex++ = '0';
//            *ucs2hex++ = '0';
//            sim900a_char2hex(ucs2hex,*gbk);
//            ucs2hex+=2;
//            gbk++;  
//        }
//        else                        //中文
//        {
//            tmp = SIM900A_SWAP16(*(WCHAR *)gbk);
//            tmp = ff_convert(tmp,1);
//            sim900a_char2hex(ucs2hex,(char)(tmp>>8));
//            ucs2hex+=2;
//            sim900a_char2hex(ucs2hex,(char)tmp);
//            ucs2hex+=2;
//            gbk+=2;
//        }
//    }
//    *ucs2hex=0;
//}


/**
 * @brief  sim900a_sms 发送短信（支持中英文,中文为GBK码）
 * @param  num: 电话号码，smstext：短信内容
 * @retval 无
 */
void sim900a_sms(char *num,char *smstext)
{
//    char ucsbuff[160];
  
//  	char end[2] = {0x1A,0x00};

    SIM900A_CLEAN_RX();                 //清空了接收缓冲区数据
    
    if(IsASSIC(smstext)==SIM900A_TRUE)
    {
        //英文
        sim900a_tx_printf("AT+CSCS=\"GSM\"\r");     //"GSM"字符集
        SIM900A_DELAY(100);
        
        sim900a_tx_printf("AT+CMGF=1\r");           //文本模式
        SIM900A_DELAY(100);
        
        sim900a_tx_printf("AT+CMGS=\"%s\"\r",num);  //电话号码
        SIM900A_DELAY(100);

        sim900a_tx_printf("%s",smstext);            //短信内容
        //SIM900A_DELAY(100);          
    }
//    else
//    {
//        //中文
//        sim900a_tx_printf("AT+CSCS=\"UCS2\"\r");    //"UCS2"字符集
//        SIM900A_DELAY(100);
//        
//        sim900a_tx_printf("AT+CMGF=1\r");           //文本模式
//        SIM900A_DELAY(100);
//        
//        sim900a_tx_printf("AT+CSMP=17,167,0,8\r");  //
//        SIM900A_DELAY(100);
//        
//        sim900a_gbk2ucs2hex(ucsbuff,num);
//        sim900a_tx_printf("AT+CMGS=\"%s\"\r",ucsbuff);  //UCS2的电话号码(需要转成 ucs2码)
//        SIM900A_DELAY(100);
//        
//        sim900a_gbk2ucs2hex(ucsbuff,smstext);
//        sim900a_tx_printf("%s\r",ucsbuff);          //UCS2的文本内容(需要转成 ucs2码)
//        //SIM900A_DELAY(100);
//    }
        SIM900A_DELAY(1);
//    sim900a_tx_printf("%c",0x1A);
//    sim900a_tx_printf("%s",end);
	     USART2->DR=(u32)0x1A;		//发送十六进制数：0X1A,信息结束符号
}


/**
 * @brief  sim900a_gprs_init GPRS功能初始化
 * @param  无
 * @retval 无
 */
void sim900a_gprs_init(void)
{
    sim900a_tx_printf("AT+CGCLASS=\"B\"\r");                       //设置移动台类别为"B"
    SIM900A_DELAY(100);
    sim900a_tx_printf("AT+CGDCONT=1,\"IP\",\"CMNET\"\r");          //PDP上下文标识1，互联网协议，接入点名称：CMNET
    SIM900A_DELAY(100);
    sim900a_tx_printf("AT+CGATT=1\r");                             //附着 GPRS 网络
    SIM900A_DELAY(100);
    sim900a_tx_printf("AT+CIPCSGP=1,\"CMNET\"\r");                 //设置为 GPRS 连接模式 ，接入点为CMNET
}


/**
 * @brief  sim900a_gprs_tcp_link 建立TCP连接
 * @param  无
 * @retval 无
 */
void sim900a_gprs_tcp_link(char *localport,char * serverip,char * serverport)
{
    sim900a_tx_printf("AT+CLPORT=\"TCP\",\"%s\"\r",localport);         //获取本地端口
    SIM900A_DELAY(100);

    //设置服务器IP和端口
    sim900a_tx_printf("AT+CIPSTART=\"TCP\",\"%s\",\"%s\"\r",serverip,serverport);
}


/**
 * @brief  sim900a_gprs_udp_link 建立UDP连接
 * @param  无
 * @retval 无
 */
void sim900a_gprs_udp_link(char *localport,char * serverip,char * serverport)
{
    sim900a_tx_printf("AT+CLPORT=\"UDP\",\"%s\"\r",localport);              //获取本地端口
    SIM900A_DELAY(100);

    sim900a_tx_printf("AT+CIPSTART=\"UDP\",\"%s\",\"%s\"\r",serverip,serverport);   //设置服务器IP和端口
}

/**
 * @brief  sim900a_gprs_send 通过GPRS发送数据
 * @param  str:数据指针
 * @retval 无
 */
void sim900a_gprs_send(char * str)
{
    sim900a_tx_printf("AT+CIPSEND\r");
    SIM900A_DELAY(100);
        
    sim900a_tx_printf("%s%c",str,0x1A);
}


/**
 * @brief  sim900a_gprs_link_close 断开网络连接
 * @param  无
 * @retval 无
 */
void sim900a_gprs_link_close(void)               //IP链接断开
{
    sim900a_tx_printf("AT+CIPCLOSE=1\r");
}
