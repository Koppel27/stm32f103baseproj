/*************************************************
 Copyright (C) 2015, 深圳市朗诺汇科科技有限公司.
 *  文件名称：md2503s.c
 *  文件功能：针对MD2503S模块的各种操作实现文件
 *  文件作者：Lawrence
 *  创建时间：2015-12-4
 *  修改时间：2016-09-08
 *  文件版本：V0.3
 *************************************************/
#include <string.h>
#include "md2503s.h"
//#include "delay.h"

#include "usart.h"		
#include "usart2.h"	
#include "led.h"

//u8 USART1_TX_BUF[USART2_MAX_RECV_LEN]; 					//串口1,发送缓存区


char *AT_ESLP = "AT+ESLP=0\r\n"; 			//禁止模块自动休眠
char *AT_CSQ = "AT+CSQ\r\n"; 				//查询手机信号强度
char *AT_CREG = "AT+CREG?\r\n"; 				//获得手机的注册状态
char *AT_CMGR = "AT+CGMR\r\n";				//查询MD2503S模块固件的版本信息
char *AT_IMEI = "AT+EGMR=0,7\r\n";			//查询MD2503S模块IMEI号码
char *AT_SET_IMEI = "AT+EGMR=1,7,\"";		//设置MD2503S模块IMEI号码	


//设置移动网络。CMNET,中国移动网络; UNINET,中国联通网络
char *AT_EGDCONT = "AT+EGDCONT=0,\"IP\",\"UNINET\"\r\n";

//设置网络连接类型
char *AT_ETCPIP = "AT+ETCPIP=1,0\r\n";

//设置IP地址、端口号。注意下面的IP地址和端口号都是通过花生壳软件生成的，详细请看《花生壳域名GPRS入网详解》		
char *AT_ETL = "AT+ETL=1,0,0,\"122.228.19.57\",36175\r\n";
//char *AT_ETL = "AT+ETL=1,0,0,\"120.76.100.197\",10002\r\n";		//公共测试服务器

//进入套接字socket的透传模式 “AT+ETLTS=socket”
char *AT_ETLTS = "AT+ETLTS=";

//发送+++退出透传模式(quit Transparent transmission)
char *QUIT_TT = "+++";

//关闭套接字 “AT+ETL=0,socket”
char *AT_CLOSE_SOCKET = "AT+ETL=0,";

char *MESSAGE = "I love you too!\r\n";				

char *POWERON = "PowerOnSuccess";					//开机成功
char *SIMCARD = "+EUSIM: 1";						//识别到SIM卡
char *ACK_OK = "OK\r\n";							//回复OK
char *ACK_CREG = "+CREG:";							//
char *ACK_ETL = "+ETL:";
char *END = "\r\n";
char *ETLTS_END = "+ETLTSEND:";



/*
注意：
由于MD2503S模块的PWRKEY引脚接入了MCU的GPIO引脚，拉低PWRKEY  2秒模块即可开机；
而在实验过程中，会不停的上电断电操作，MCU的GPIO引脚可能已经拉低PWRKY——模块开机了，或者用户已经用按键把模块开机了；
所以当MD2503S_poweron函数运行时，模块可能处于关机状态，或者已经开机。
(1). 如果模块处于关机状态，拉低PWRKEY  2秒模块即可开机，同时模块的串口输出开机信息如下：
F1: 0000 0000

V0: 0000 0000 

00: 0000 0000

U0: 0000 0001 

G0: 0002 0000 

T0: 0000 00BB

Jump to BL


PowerOnSuccess  (说明开机成功) 
PTS OK!

+EUSIM: 1 (说明识别到SIM卡)

然后应该向模块发送 "AT+ESLP=0"	禁止休眠命令，模块会回复"OK\r\n"。

(2). 如果模块处于开机并且休眠状态，拉低PWRKEY会唤醒模块，同时向模块发送 "AT+ESLP=0"	禁止休眠命令，
模块会回复"OK\r\n"。

(3). 如果模块处于开机且非休眠状态，拉低PWRKEY的同时(这时模块不会关机！)，向模块发送 "AT+ESLP=0"	禁止休眠命令，
模块也会回复"OK\r\n"。

=======综上所述：不论如何，我们都是先拉低PWRKEY，然后向模块发送 "AT+ESLP=0" 禁止休眠命令，等待回复"OK\r\n"
*/

/*******************************************************
 Copyright (C) 2015, 深圳市朗诺汇科科技有限公司.
 *  函数名称：MD2503S_poweron
 *  函数功能：使能MD2503S模块（包括开机或者唤醒）
 *  入口参数：
 *  出口参数：
 *  函数说明：
 ******************************************************/
void MD2503S_poweron(void)
{
	//我们这里使用STM32F103C8T6的串口2连接到模块的串口, STM32F103C8T6的串口1用来输出打印信息
	DEBUG("\r\n\r\n======================start Power on MD2503S===================\r\n");
	

	PWRKEY = 0;			//拉低PWRKEY，MD2503S模块开机
	
	while(1) 
	{		
		uart2_start_recv();
		
		uart2_send_string(AT_ESLP);					//使用串口2发送禁止休眠指令给MD2503S模块
		delay_ms(50);
		uart2_send_string(AT_ESLP);					//使用串口2发送禁止休眠指令给MD2503S模块
		delay_ms(50);
		uart2_send_string(AT_ESLP);					//使用串口2发送禁止休眠指令给MD2503S模块
		delay_ms(50);
		
		if(is_uart2_recv_data())						//接收到一次数据了
		{
		
			//DEBUG("\r\n%s\r\n", USART2_RX_BUF);			//发送接收到的数据到串口1

			if(strstr((char *)USART2_RX_BUF, POWERON) != NULL)
			{
				DEBUG("MD2503S Power On Successful!--------------\r\n");
			}

			//"+EUSIM: 1"耗时比较久，需要多延时等待一会儿
			if(strstr((char *)USART2_RX_BUF, SIMCARD) != NULL)
			{
				DEBUG("SIM Card ready!-------------\r\n");
			}

			if(strstr((char *)USART2_RX_BUF, ACK_OK) != NULL)
			{
				DEBUG("Disable auto sleep!-------------\r\n");
				break;
			}						
 		}
		
		DEBUG("Power on MD2503S ..........................\r\n");

	}
	
	PWRKEY = 1;		//开机完成，恢复PWRKEY电平		

	DEBUG("End of MD2503S Power on(or wake up): successful!--------------");
	DEBUG("\r\n\r\n");
}



/*******************************************************
 Copyright (C) 2015, 深圳市朗诺汇科科技有限公司.
 *  函数名称：MD2503S_print_version
 *  函数功能：查询MD2503S模块固件的版本信息
 *  入口参数：
 *  出口参数：
 *  函数说明：
 ******************************************************/
void MD2503S_print_version(void)
{
	//我们这里使用STM32F103C8T6的串口2连接到模块的串口, STM32F103C8T6的串口1用来输出打印信息
	uart2_start_recv();
	
	uart2_send_string(AT_CMGR);				
	delay_ms(800);
	
	if(is_uart2_recv_data())					//接收到一次数据了
	{
		
		DEBUG("------------Version--------------\r\n%s\r\n---------------------------\r\n\r\n", USART2_RX_BUF);
	}	
}


/*******************************************************
 Copyright (C) 2015, 深圳市朗诺汇科科技有限公司.
 *  函数名称：MD2503S_print_CSQ
 *  函数功能：查询MD2503S模块 2G网络信号强度
 *  入口参数：
 *  出口参数：
 *  函数说明：
 ******************************************************/
void MD2503S_print_CSQ(void)
{
	//我们这里使用STM32F103C8T6的串口2连接到模块的串口, STM32F103C8T6的串口1用来输出打印信息

	uart2_start_recv();
	
	uart2_send_string(AT_CSQ);				
	delay_ms(800);
	
	if(is_uart2_recv_data())					//接收到一次数据了
	{
		
		DEBUG("------------CSQ--------------\r\n%s\r\n----------------------------------", USART2_RX_BUF);
	}
	
}





/*
成功案例: "+CREG: 0,1"里面的第二个参数是1或者5，说明设备已经注册到网络
AT+CREG?

+CREG: 0,1

OK
*/
/*******************************************************
 Copyright (C) 2015, 深圳市朗诺汇科科技有限公司.
 *  函数名称：MD2503S_wait_creg
 *  函数功能：等待设备注册到运营商网络
 *  入口参数：
 *  出口参数：
 *  函数说明：
 ******************************************************/
void MD2503S_wait_creg(void)
{
	char *str_creg = NULL;

	DEBUG("Start MD2503S: AT+CREG? --------------\r\n");

	while(1)
	{
		delay_ms(1000);
		//我们这里使用STM32F103C8T6的串口2连接到模块的串口, STM32F103C8T6的串口1用来输出打印信息
		//开启串口2的接收
		uart2_start_recv();
	
		uart2_send_string(AT_CREG);
		delay_ms(30);
		//多发几次
		uart2_send_string(AT_CREG);
		delay_ms(50);
		


		if((is_uart2_recv_data()) && ((str_creg = strstr((char *)USART2_RX_BUF, ACK_CREG)) != NULL))
		{
			DEBUG("------------AT+CREG?--------------\r\n%s\r\n---------------------------\r\n\r\n", USART2_RX_BUF);
			if((str_creg[9] == '1') || (str_creg[9] == '5'))
			{
				DEBUG("MD2503S is already register to CMNET/UNINET--------------\r\n");
				break;
			}
			
		}

		DEBUG("MD2503S: AT+CREG? .........................\r\n");
	}

	DEBUG("End MD2503S: AT+CREG? --------------");
	DEBUG("\r\n\r\n");

}

/*******************************************************
 Copyright (C) 2015, 深圳市朗诺汇科科技有限公司.
 *  函数名称：MD2503S_make_call
 *  函数功能：拨打电话
 *  入口参数：
 *  出口参数：
 *  函数说明：
 ******************************************************/
void MD2503S_make_call(void)
{
	
}


/*******************************************************
 Copyright (C) 2015, 深圳市朗诺汇科科技有限公司.
 *  函数名称：MD2503S_send_msg
 *  函数功能：发送英文短信
 *  入口参数：
 *  出口参数：
 *  函数说明：
 ******************************************************/
void MD2503S_send_msg(void)
{

}

/*
  GPRS入网步骤(具体看《GPRS入网操作笔记.txt》和《一般操作步骤.txt》)
  
AT+EGDCONT=0,"IP","CMNET"

OK
AT+ETCPIP=1,0

OK
AT+ETL=1,0,0,"122.228.19.57",29329

+ETL: 0

OK
AT+ETLTS=0	

...此时连接成功，可以发数据


我后面发送+++ 主动退出的
+ETLTSEND: 0

+ESOCK: 0 CLOSE
*/
/*******************************************************
 Copyright (C) 2015, 深圳市朗诺汇科科技有限公司.
 *  函数名称：MD2503S_GPRS_send_msg
 *  函数功能：使用GPRS网络发送数据给服务器
 *  入口参数：
 *  出口参数：
 *  函数说明：
 ******************************************************/
void MD2503S_GPRS_send_msg(void)
{
	int i = 0;
	char *ack_socket = NULL;
	char socket = '0';
	char send_buf[16] = {0};

	DEBUG("Start MD2503S: use GPRS to send messages -----------\r\n");	

	//我们这里使用STM32F103C8T6的串口2连接到模块的串口, STM32F103C8T6的串口1用来输出打印信息
	//开启串口2的接收
	uart2_start_recv();

	//1. 设置移动网络。CMNET,中国移动网络; UNINET,中国联通网络
	uart2_send_string(AT_EGDCONT);
	delay_ms(30); 
	DEBUG("%s\r\n", (char *)USART2_RX_BUF);
	
	//2. 设置网络连接类型
	uart2_start_recv();
	uart2_send_string(AT_ETCPIP);
	delay_ms(30);
	DEBUG("%s\r\n", (char *)USART2_RX_BUF);	

	//3. 设置IP地址、端口号
	uart2_start_recv();
	uart2_send_string(AT_ETL);
   	
	//4. 等待服务器返回套接字	
	while(1)
	{
		delay_ms(50);
		
		if(is_uart2_recv_data())
		{
			DEBUG("%s\r\n", (char *)USART2_RX_BUF);
		
			if(((ack_socket = strstr((char *)USART2_RX_BUF, ACK_ETL)) != NULL) && 
				(strstr((char *)USART2_RX_BUF, ACK_OK) != NULL))
			{
				DEBUG("MD2503S: get a socket!\r\n");
				break;
			}
			
			uart2_start_recv();
		}
		
		if(i++ > 600)
		{
			DEBUG("MD2503S: Timeout to wait for server!\r\n");
			
			//此处可以将STM32F103C8T6和MD2503S模块都复位重启一次
			
			return;
		}
	}

	//5. 获取套接字
	DEBUG("MD2503S: ack socket is '%s' \r\n", ack_socket);
	socket = *(ack_socket + 6);

	memset(send_buf, 0, 16);
	strcpy(send_buf, AT_ETLTS);
	send_buf[9] = socket;
	strcat(send_buf, END);
	DEBUG("MD2503S: send_buf is '%s' \r\n", send_buf);

	uart2_start_recv();

	//6. 连接上套接字，进入透传模式
	/*
	做完这6步，进入透传模式，然后发送您的数据，接着发送+++结束透传模式，
	然后干其他事情，再做一次第6步，又可以回到透传模式传输数据。

	但是要注意的是：一般移动网络，50秒左右没有数据通信，移动网关会主动断开您的链接，
	这时要重新来一遍，1 2 3 4 5 6几个步骤。
	*/
	uart2_send_string(send_buf);
	delay_ms(20);

	DEBUG("MD2503S: sending message to server \r\n");
	//7. ------------------------向服务器发送字符串100次----------------------------
	for(i = 0; i < 100; i++)
	{
		uart2_send_string(MESSAGE);
		delay_ms(200);		//**注意发送数据的时间间隔不能太短，否则卡死
	}
	uart2_send_string("No.You can't!\r\n");
	//------------------------------------------------------------------------------	
	DEBUG("MD2503S: sending message to server end\r\n");
	
	//8. 发送+++ 退出透传模式	
	for(;;)
	{	
		DEBUG("MD2503S: trying quit transparent transmition...\r\n");
		uart2_start_recv();
		delay_ms(1000);					//**注意延时时间，否则退出不了透传
		uart2_send_string(QUIT_TT);
		delay_ms(1000);	
		
		if(is_uart2_recv_data())
		{
			DEBUG("MD2503S: recv buf is '%s' \r\n", USART2_RX_BUF);
			if(strstr((char *)USART2_RX_BUF, ETLTS_END) != NULL)
			{
				DEBUG("MD2503S: successful quit transparent transmition\r\n");
				break;
			}
		}
		
		
		//测试此时是否退出透传模式，进入了AT指令模式
		uart2_start_recv();
		delay_ms(100);
		uart2_send_string(AT_ESLP);
		delay_ms(2000);
		DEBUG("MD2503S: recv buf is '%s' \r\n", USART2_RX_BUF);
		if(is_uart2_recv_data())
		{
			
			if(strstr((char *)USART2_RX_BUF, ACK_OK) != NULL)
			{
				DEBUG("MD2503S: successful quit transparent transmition\r\n");
				break;
			}
		}

	}
	DEBUG("MD2503S: quit transparent transmition end\r\n");
	
	//9. 关闭套接字
	memset(send_buf, 0, 16);
	strcpy(send_buf, AT_CLOSE_SOCKET);
	send_buf[9] = socket;
	strcat(send_buf, END);
	DEBUG("MD2503S: send_buf is '%s' \r\n", send_buf);	
	for(;;)
	{
		uart2_start_recv();
		delay_ms(50);
		uart2_send_string(send_buf);
		delay_ms(50);
		
		if(is_uart2_recv_data())
		{
			DEBUG("MD2503S: recv buf is '%s' \r\n", USART2_RX_BUF);
			if(strstr((char *)USART2_RX_BUF, ACK_OK) != NULL)
			{
				DEBUG("MD2503S: successful close socket\r\n");
				break;
			}
		}
	}
		
	//10. 最后测试下当前是否处于AT指令模式，即发送AT指令能否得到正确回复
	uart2_start_recv();
	delay_ms(50);
	uart2_send_string(AT_ESLP);
	delay_ms(50);
	DEBUG("\r\n\r\nMD2503S: recv buf is '%s' \r\n", USART2_RX_BUF);

	DEBUG("End MD2503S: use GPRS to send messages -----------");
	DEBUG("\r\n\r\n");

}



