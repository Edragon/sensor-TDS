/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2014-08-xx
  * @brief   WF-SIM900Aģ����������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� ISO-STM32 ������
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
	*/
  
#include <stdarg.h>
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include "sim900a.h"
#include <string.h>




/**
 * @brief  sim900a_cmd  ��������
 * @param  cmd�������ַ���ָ�룬reply��ϣ���õ���������Ӧ��waittime������ȴ���ʱ��
 * @retval �����Ƿ�õ�������Ӧ������SIM900A_TRUE����������SIM900A_FALSE
 */
uint8_t sim900a_cmd(char *cmd, char *reply,uint32_t waittime )
{    
    SIM900A_CLEAN_RX();                 //����˽��ջ���������

    SIM900A_TX(cmd);                    //��������

    if(reply == 0)                      //����Ҫ��������
    {
        return SIM900A_TRUE;
    }
    
    SIM900A_DELAY(waittime);                 //��ʱ
    
    
    return sim900a_cmd_check(reply);    //�Խ������ݽ��д���
}



/**
 * @brief  sim900a_cmd_check  ����������Ӧ
 * @param  reply������Ӧ���ַ���ָ��
 * @retval ��⵽ƥ�����ӦSIM900A_TRUE��û��⵽ƥ�����ӦSIM900A_FALSE
 */
uint8_t sim900a_cmd_check(char *reply)
{
    uint8_t len;

    char *redata;
    
    redata = SIM900A_RX(len);   //��������

    if(strstr(redata,reply) != NULL)
    {    
      return SIM900A_TRUE;        //���ҵ�ƥ�����Ӧ����
    }
    else
    {
      return SIM900A_FALSE;       

    }
    
 
}


/**
 * @brief  sim900a_waitask  �ȴ�����Ӧ�� 
 * @param  waitask_hook������¼����Ӻ������������¼����˳���������
 * @retval ���յ���Ӧ������ָ��
 */
char * sim900a_waitask(uint8_t waitask_hook(void))      //�ȴ�������Ӧ��
{
    uint8_t len;
    char *redata;
    do{
        redata = SIM900A_RX(len);   //��������
        if(waitask_hook!=0)
        {
            if(waitask_hook()==SIM900A_TRUE)           //���� SIM900A_TRUE ��ʾ��⵽�¼�����Ҫ�˳�
            {
                redata = 0;
                return redata;               
            }
        }
    }while(len==0);                 //��������Ϊ0ʱһֱ�ȴ�
    
    
    SIM900A_DELAY(20);              //��ʱ��ȷ���ܽ��յ�ȫ�����ݣ�115200�������£�ÿms�ܽ���11.52���ֽڣ�
    return redata;
}



/**
 * @brief  sim900a_cnum 
 * @param  num:�洢���������ָ��
 * @retval ����SIM900A_FALSE��ʾ��ȡʧ�ܣ�����SIM900A_TRUE��ʾ�ɹ�
 */
uint8_t sim900a_cnum(char *num)
{
    char *redata;
    uint8_t len;
    
    if(sim900a_cmd("AT+CNUM\r","OK", 1000) != SIM900A_TRUE)
    {
        return SIM900A_FALSE;
    }
    
    redata = SIM900A_RX(len);   //��������
    
    if(len == 0)
    {
        return SIM900A_FALSE;
    }
    
    //��һ�����ź��������Ϊ:"��������"
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
    *num = 0;               //�ַ�����β��Ҫ��0
    return SIM900A_TRUE;
}




/**
 * @brief  sim900a_call 
 * @param  num:Ҫ����ĵ绰����
 * @retval ��
 */
void sim900a_call(char *num)
{
    SIM900A_CLEAN_RX();                 //����˽��ջ���������
    
    sim900a_tx_printf("ATD%s;\r",num);

    //������ǲ��������ݵ�
    //�����κ�Ӧ�𣬿�ͷ����"\r\n",��������Ч�������Ǵӵ�3���ֽڿ�ʼ
    //�Է��ҵ��绰��û��ͨ�������أ�BUSY
    //�Է������˵绰��+COLP: "555",129,"",0,"9648674F98DE"   OK
    //�Է������˵绰,Ȼ��ҵ���NO CARRIER
    //�Է���ʱû�ӵ绰��NO ANSWER
}


/**
 * @brief  IsASSIC �ж��Ƿ�ASSIC����
 * @param  str: �ַ���ָ��
 * @retval ��ASSIC���뷵��TRUE���Ǵ�ASSIC���뷵��FALSE
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
 * @brief  sim900a_gbk2ucs2 ��GBK����ת����UCS2����
 * @param  ucs2: ucs2�ַ���ָ�룬gbk��GBK�ַ���ָ��
 * @retval ��
 */
//void sim900a_gbk2ucs2(char * ucs2,char * gbk)
//{
//    WCHAR   tmp;
//    
//    while(*gbk)
//    {
//        if((*gbk&0xFF) < 0x7F)      //Ӣ��
//        {
//            
//            *ucs2++ = 0;
//            *ucs2++ = *gbk++;  
//        }
//        else                        //����
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
 * @brief  sim900a_char2hex ���ַ�ת����16�����ַ�
 * @param  hex: 16�����ַ��洢��λ��ָ�룬ch���ַ�
 * @retval ��
 */
void sim900a_char2hex(char *hex,char ch)
{
    const char numhex[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    *hex++  = numhex[(ch & 0xF0)>>4];
    *hex    = numhex[ ch & 0x0F];
}

///**
// * @brief  sim900a_gbk2ucs2hex GBK����תucs2������ת��16�����ַ�
// * @param  ucs2hex: 16�������洢��λ��ָ�룬gbk���ַ�
// * @retval ��
// */
//void sim900a_gbk2ucs2hex(char * ucs2hex,char * gbk)
//{
//    WCHAR   tmp;
//    
//    while(*gbk)
//    {
//        if((*gbk&0xFF) < 0x7F)      //Ӣ��
//        {
//            
//            *ucs2hex++ = '0';
//            *ucs2hex++ = '0';
//            sim900a_char2hex(ucs2hex,*gbk);
//            ucs2hex+=2;
//            gbk++;  
//        }
//        else                        //����
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
 * @brief  sim900a_sms ���Ͷ��ţ�֧����Ӣ��,����ΪGBK�룩
 * @param  num: �绰���룬smstext����������
 * @retval ��
 */
void sim900a_sms(char *num,char *smstext)
{
//    char ucsbuff[160];
  
//  	char end[2] = {0x1A,0x00};

    SIM900A_CLEAN_RX();                 //����˽��ջ���������
    
    if(IsASSIC(smstext)==SIM900A_TRUE)
    {
        //Ӣ��
        sim900a_tx_printf("AT+CSCS=\"GSM\"\r");     //"GSM"�ַ���
        SIM900A_DELAY(100);
        
        sim900a_tx_printf("AT+CMGF=1\r");           //�ı�ģʽ
        SIM900A_DELAY(100);
        
        sim900a_tx_printf("AT+CMGS=\"%s\"\r",num);  //�绰����
        SIM900A_DELAY(100);

        sim900a_tx_printf("%s",smstext);            //��������
        //SIM900A_DELAY(100);          
    }
//    else
//    {
//        //����
//        sim900a_tx_printf("AT+CSCS=\"UCS2\"\r");    //"UCS2"�ַ���
//        SIM900A_DELAY(100);
//        
//        sim900a_tx_printf("AT+CMGF=1\r");           //�ı�ģʽ
//        SIM900A_DELAY(100);
//        
//        sim900a_tx_printf("AT+CSMP=17,167,0,8\r");  //
//        SIM900A_DELAY(100);
//        
//        sim900a_gbk2ucs2hex(ucsbuff,num);
//        sim900a_tx_printf("AT+CMGS=\"%s\"\r",ucsbuff);  //UCS2�ĵ绰����(��Ҫת�� ucs2��)
//        SIM900A_DELAY(100);
//        
//        sim900a_gbk2ucs2hex(ucsbuff,smstext);
//        sim900a_tx_printf("%s\r",ucsbuff);          //UCS2���ı�����(��Ҫת�� ucs2��)
//        //SIM900A_DELAY(100);
//    }
        SIM900A_DELAY(1);
//    sim900a_tx_printf("%c",0x1A);
//    sim900a_tx_printf("%s",end);
	     USART2->DR=(u32)0x1A;		//����ʮ����������0X1A,��Ϣ��������
}


/**
 * @brief  sim900a_gprs_init GPRS���ܳ�ʼ��
 * @param  ��
 * @retval ��
 */
void sim900a_gprs_init(void)
{
    sim900a_tx_printf("AT+CGCLASS=\"B\"\r");                       //�����ƶ�̨���Ϊ"B"
    SIM900A_DELAY(100);
    sim900a_tx_printf("AT+CGDCONT=1,\"IP\",\"CMNET\"\r");          //PDP�����ı�ʶ1��������Э�飬��������ƣ�CMNET
    SIM900A_DELAY(100);
    sim900a_tx_printf("AT+CGATT=1\r");                             //���� GPRS ����
    SIM900A_DELAY(100);
    sim900a_tx_printf("AT+CIPCSGP=1,\"CMNET\"\r");                 //����Ϊ GPRS ����ģʽ �������ΪCMNET
}


/**
 * @brief  sim900a_gprs_tcp_link ����TCP����
 * @param  ��
 * @retval ��
 */
void sim900a_gprs_tcp_link(char *localport,char * serverip,char * serverport)
{
    sim900a_tx_printf("AT+CLPORT=\"TCP\",\"%s\"\r",localport);         //��ȡ���ض˿�
    SIM900A_DELAY(100);

    //���÷�����IP�Ͷ˿�
    sim900a_tx_printf("AT+CIPSTART=\"TCP\",\"%s\",\"%s\"\r",serverip,serverport);
}


/**
 * @brief  sim900a_gprs_udp_link ����UDP����
 * @param  ��
 * @retval ��
 */
void sim900a_gprs_udp_link(char *localport,char * serverip,char * serverport)
{
    sim900a_tx_printf("AT+CLPORT=\"UDP\",\"%s\"\r",localport);              //��ȡ���ض˿�
    SIM900A_DELAY(100);

    sim900a_tx_printf("AT+CIPSTART=\"UDP\",\"%s\",\"%s\"\r",serverip,serverport);   //���÷�����IP�Ͷ˿�
}

/**
 * @brief  sim900a_gprs_send ͨ��GPRS��������
 * @param  str:����ָ��
 * @retval ��
 */
void sim900a_gprs_send(char * str)
{
    sim900a_tx_printf("AT+CIPSEND\r");
    SIM900A_DELAY(100);
        
    sim900a_tx_printf("%s%c",str,0x1A);
}


/**
 * @brief  sim900a_gprs_link_close �Ͽ���������
 * @param  ��
 * @retval ��
 */
void sim900a_gprs_link_close(void)               //IP���ӶϿ�
{
    sim900a_tx_printf("AT+CIPCLOSE=1\r");
}
