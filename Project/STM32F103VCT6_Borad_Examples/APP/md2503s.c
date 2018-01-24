/*************************************************
 Copyright (C) 2015, ��������ŵ��ƿƼ����޹�˾.
 *  �ļ����ƣ�md2503s.c
 *  �ļ����ܣ����MD2503Sģ��ĸ��ֲ���ʵ���ļ�
 *  �ļ����ߣ�Lawrence
 *  ����ʱ�䣺2015-12-4
 *  �޸�ʱ�䣺2016-09-08
 *  �ļ��汾��V0.3
 *************************************************/
#include <string.h>
#include "md2503s.h"
//#include "delay.h"

#include "usart.h"		
#include "usart2.h"	
#include "led.h"

//u8 USART1_TX_BUF[USART2_MAX_RECV_LEN]; 					//����1,���ͻ�����


char *AT_ESLP = "AT+ESLP=0\r\n"; 			//��ֹģ���Զ�����
char *AT_CSQ = "AT+CSQ\r\n"; 				//��ѯ�ֻ��ź�ǿ��
char *AT_CREG = "AT+CREG?\r\n"; 				//����ֻ���ע��״̬
char *AT_CMGR = "AT+CGMR\r\n";				//��ѯMD2503Sģ��̼��İ汾��Ϣ
char *AT_IMEI = "AT+EGMR=0,7\r\n";			//��ѯMD2503Sģ��IMEI����
char *AT_SET_IMEI = "AT+EGMR=1,7,\"";		//����MD2503Sģ��IMEI����	


//�����ƶ����硣CMNET,�й��ƶ�����; UNINET,�й���ͨ����
char *AT_EGDCONT = "AT+EGDCONT=0,\"IP\",\"UNINET\"\r\n";

//����������������
char *AT_ETCPIP = "AT+ETCPIP=1,0\r\n";

//����IP��ַ���˿ںš�ע�������IP��ַ�Ͷ˿ںŶ���ͨ��������������ɵģ���ϸ�뿴������������GPRS������⡷		
char *AT_ETL = "AT+ETL=1,0,0,\"122.228.19.57\",36175\r\n";
//char *AT_ETL = "AT+ETL=1,0,0,\"120.76.100.197\",10002\r\n";		//�������Է�����

//�����׽���socket��͸��ģʽ ��AT+ETLTS=socket��
char *AT_ETLTS = "AT+ETLTS=";

//����+++�˳�͸��ģʽ(quit Transparent transmission)
char *QUIT_TT = "+++";

//�ر��׽��� ��AT+ETL=0,socket��
char *AT_CLOSE_SOCKET = "AT+ETL=0,";

char *MESSAGE = "I love you too!\r\n";				

char *POWERON = "PowerOnSuccess";					//�����ɹ�
char *SIMCARD = "+EUSIM: 1";						//ʶ��SIM��
char *ACK_OK = "OK\r\n";							//�ظ�OK
char *ACK_CREG = "+CREG:";							//
char *ACK_ETL = "+ETL:";
char *END = "\r\n";
char *ETLTS_END = "+ETLTSEND:";



/*
ע�⣺
����MD2503Sģ���PWRKEY���Ž�����MCU��GPIO���ţ�����PWRKEY  2��ģ�鼴�ɿ�����
����ʵ������У��᲻ͣ���ϵ�ϵ������MCU��GPIO���ſ����Ѿ�����PWRKY����ģ�鿪���ˣ������û��Ѿ��ð�����ģ�鿪���ˣ�
���Ե�MD2503S_poweron��������ʱ��ģ����ܴ��ڹػ�״̬�������Ѿ�������
(1). ���ģ�鴦�ڹػ�״̬������PWRKEY  2��ģ�鼴�ɿ�����ͬʱģ��Ĵ������������Ϣ���£�
F1: 0000 0000

V0: 0000 0000 

00: 0000 0000

U0: 0000 0001 

G0: 0002 0000 

T0: 0000 00BB

Jump to BL


PowerOnSuccess  (˵�������ɹ�) 
PTS OK!

+EUSIM: 1 (˵��ʶ��SIM��)

Ȼ��Ӧ����ģ�鷢�� "AT+ESLP=0"	��ֹ�������ģ���ظ�"OK\r\n"��

(2). ���ģ�鴦�ڿ�����������״̬������PWRKEY�ỽ��ģ�飬ͬʱ��ģ�鷢�� "AT+ESLP=0"	��ֹ�������
ģ���ظ�"OK\r\n"��

(3). ���ģ�鴦�ڿ����ҷ�����״̬������PWRKEY��ͬʱ(��ʱģ�鲻��ػ���)����ģ�鷢�� "AT+ESLP=0"	��ֹ�������
ģ��Ҳ��ظ�"OK\r\n"��

=======����������������Σ����Ƕ���������PWRKEY��Ȼ����ģ�鷢�� "AT+ESLP=0" ��ֹ��������ȴ��ظ�"OK\r\n"
*/

/*******************************************************
 Copyright (C) 2015, ��������ŵ��ƿƼ����޹�˾.
 *  �������ƣ�MD2503S_poweron
 *  �������ܣ�ʹ��MD2503Sģ�飨�����������߻��ѣ�
 *  ��ڲ�����
 *  ���ڲ�����
 *  ����˵����
 ******************************************************/
void MD2503S_poweron(void)
{
	//��������ʹ��STM32F103C8T6�Ĵ���2���ӵ�ģ��Ĵ���, STM32F103C8T6�Ĵ���1���������ӡ��Ϣ
	DEBUG("\r\n\r\n======================start Power on MD2503S===================\r\n");
	

	PWRKEY = 0;			//����PWRKEY��MD2503Sģ�鿪��
	
	while(1) 
	{		
		uart2_start_recv();
		
		uart2_send_string(AT_ESLP);					//ʹ�ô���2���ͽ�ֹ����ָ���MD2503Sģ��
		delay_ms(50);
		uart2_send_string(AT_ESLP);					//ʹ�ô���2���ͽ�ֹ����ָ���MD2503Sģ��
		delay_ms(50);
		uart2_send_string(AT_ESLP);					//ʹ�ô���2���ͽ�ֹ����ָ���MD2503Sģ��
		delay_ms(50);
		
		if(is_uart2_recv_data())						//���յ�һ��������
		{
		
			//DEBUG("\r\n%s\r\n", USART2_RX_BUF);			//���ͽ��յ������ݵ�����1

			if(strstr((char *)USART2_RX_BUF, POWERON) != NULL)
			{
				DEBUG("MD2503S Power On Successful!--------------\r\n");
			}

			//"+EUSIM: 1"��ʱ�ȽϾã���Ҫ����ʱ�ȴ�һ���
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
	
	PWRKEY = 1;		//������ɣ��ָ�PWRKEY��ƽ		

	DEBUG("End of MD2503S Power on(or wake up): successful!--------------");
	DEBUG("\r\n\r\n");
}



/*******************************************************
 Copyright (C) 2015, ��������ŵ��ƿƼ����޹�˾.
 *  �������ƣ�MD2503S_print_version
 *  �������ܣ���ѯMD2503Sģ��̼��İ汾��Ϣ
 *  ��ڲ�����
 *  ���ڲ�����
 *  ����˵����
 ******************************************************/
void MD2503S_print_version(void)
{
	//��������ʹ��STM32F103C8T6�Ĵ���2���ӵ�ģ��Ĵ���, STM32F103C8T6�Ĵ���1���������ӡ��Ϣ
	uart2_start_recv();
	
	uart2_send_string(AT_CMGR);				
	delay_ms(800);
	
	if(is_uart2_recv_data())					//���յ�һ��������
	{
		
		DEBUG("------------Version--------------\r\n%s\r\n---------------------------\r\n\r\n", USART2_RX_BUF);
	}	
}


/*******************************************************
 Copyright (C) 2015, ��������ŵ��ƿƼ����޹�˾.
 *  �������ƣ�MD2503S_print_CSQ
 *  �������ܣ���ѯMD2503Sģ�� 2G�����ź�ǿ��
 *  ��ڲ�����
 *  ���ڲ�����
 *  ����˵����
 ******************************************************/
void MD2503S_print_CSQ(void)
{
	//��������ʹ��STM32F103C8T6�Ĵ���2���ӵ�ģ��Ĵ���, STM32F103C8T6�Ĵ���1���������ӡ��Ϣ

	uart2_start_recv();
	
	uart2_send_string(AT_CSQ);				
	delay_ms(800);
	
	if(is_uart2_recv_data())					//���յ�һ��������
	{
		
		DEBUG("------------CSQ--------------\r\n%s\r\n----------------------------------", USART2_RX_BUF);
	}
	
}





/*
�ɹ�����: "+CREG: 0,1"����ĵڶ���������1����5��˵���豸�Ѿ�ע�ᵽ����
AT+CREG?

+CREG: 0,1

OK
*/
/*******************************************************
 Copyright (C) 2015, ��������ŵ��ƿƼ����޹�˾.
 *  �������ƣ�MD2503S_wait_creg
 *  �������ܣ��ȴ��豸ע�ᵽ��Ӫ������
 *  ��ڲ�����
 *  ���ڲ�����
 *  ����˵����
 ******************************************************/
void MD2503S_wait_creg(void)
{
	char *str_creg = NULL;

	DEBUG("Start MD2503S: AT+CREG? --------------\r\n");

	while(1)
	{
		delay_ms(1000);
		//��������ʹ��STM32F103C8T6�Ĵ���2���ӵ�ģ��Ĵ���, STM32F103C8T6�Ĵ���1���������ӡ��Ϣ
		//��������2�Ľ���
		uart2_start_recv();
	
		uart2_send_string(AT_CREG);
		delay_ms(30);
		//�෢����
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
 Copyright (C) 2015, ��������ŵ��ƿƼ����޹�˾.
 *  �������ƣ�MD2503S_make_call
 *  �������ܣ�����绰
 *  ��ڲ�����
 *  ���ڲ�����
 *  ����˵����
 ******************************************************/
void MD2503S_make_call(void)
{
	
}


/*******************************************************
 Copyright (C) 2015, ��������ŵ��ƿƼ����޹�˾.
 *  �������ƣ�MD2503S_send_msg
 *  �������ܣ�����Ӣ�Ķ���
 *  ��ڲ�����
 *  ���ڲ�����
 *  ����˵����
 ******************************************************/
void MD2503S_send_msg(void)
{

}

/*
  GPRS��������(���忴��GPRS���������ʼ�.txt���͡�һ���������.txt��)
  
AT+EGDCONT=0,"IP","CMNET"

OK
AT+ETCPIP=1,0

OK
AT+ETL=1,0,0,"122.228.19.57",29329

+ETL: 0

OK
AT+ETLTS=0	

...��ʱ���ӳɹ������Է�����


�Һ��淢��+++ �����˳���
+ETLTSEND: 0

+ESOCK: 0 CLOSE
*/
/*******************************************************
 Copyright (C) 2015, ��������ŵ��ƿƼ����޹�˾.
 *  �������ƣ�MD2503S_GPRS_send_msg
 *  �������ܣ�ʹ��GPRS���緢�����ݸ�������
 *  ��ڲ�����
 *  ���ڲ�����
 *  ����˵����
 ******************************************************/
void MD2503S_GPRS_send_msg(void)
{
	int i = 0;
	char *ack_socket = NULL;
	char socket = '0';
	char send_buf[16] = {0};

	DEBUG("Start MD2503S: use GPRS to send messages -----------\r\n");	

	//��������ʹ��STM32F103C8T6�Ĵ���2���ӵ�ģ��Ĵ���, STM32F103C8T6�Ĵ���1���������ӡ��Ϣ
	//��������2�Ľ���
	uart2_start_recv();

	//1. �����ƶ����硣CMNET,�й��ƶ�����; UNINET,�й���ͨ����
	uart2_send_string(AT_EGDCONT);
	delay_ms(30); 
	DEBUG("%s\r\n", (char *)USART2_RX_BUF);
	
	//2. ����������������
	uart2_start_recv();
	uart2_send_string(AT_ETCPIP);
	delay_ms(30);
	DEBUG("%s\r\n", (char *)USART2_RX_BUF);	

	//3. ����IP��ַ���˿ں�
	uart2_start_recv();
	uart2_send_string(AT_ETL);
   	
	//4. �ȴ������������׽���	
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
			
			//�˴����Խ�STM32F103C8T6��MD2503Sģ�鶼��λ����һ��
			
			return;
		}
	}

	//5. ��ȡ�׽���
	DEBUG("MD2503S: ack socket is '%s' \r\n", ack_socket);
	socket = *(ack_socket + 6);

	memset(send_buf, 0, 16);
	strcpy(send_buf, AT_ETLTS);
	send_buf[9] = socket;
	strcat(send_buf, END);
	DEBUG("MD2503S: send_buf is '%s' \r\n", send_buf);

	uart2_start_recv();

	//6. �������׽��֣�����͸��ģʽ
	/*
	������6��������͸��ģʽ��Ȼ�����������ݣ����ŷ���+++����͸��ģʽ��
	Ȼ����������飬����һ�ε�6�����ֿ��Իص�͸��ģʽ�������ݡ�

	����Ҫע����ǣ�һ���ƶ����磬50������û������ͨ�ţ��ƶ����ػ������Ͽ��������ӣ�
	��ʱҪ������һ�飬1 2 3 4 5 6�������衣
	*/
	uart2_send_string(send_buf);
	delay_ms(20);

	DEBUG("MD2503S: sending message to server \r\n");
	//7. ------------------------������������ַ���100��----------------------------
	for(i = 0; i < 100; i++)
	{
		uart2_send_string(MESSAGE);
		delay_ms(200);		//**ע�ⷢ�����ݵ�ʱ��������̫�̣�������
	}
	uart2_send_string("No.You can't!\r\n");
	//------------------------------------------------------------------------------	
	DEBUG("MD2503S: sending message to server end\r\n");
	
	//8. ����+++ �˳�͸��ģʽ	
	for(;;)
	{	
		DEBUG("MD2503S: trying quit transparent transmition...\r\n");
		uart2_start_recv();
		delay_ms(1000);					//**ע����ʱʱ�䣬�����˳�����͸��
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
		
		
		//���Դ�ʱ�Ƿ��˳�͸��ģʽ��������ATָ��ģʽ
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
	
	//9. �ر��׽���
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
		
	//10. �������µ�ǰ�Ƿ���ATָ��ģʽ��������ATָ���ܷ�õ���ȷ�ظ�
	uart2_start_recv();
	delay_ms(50);
	uart2_send_string(AT_ESLP);
	delay_ms(50);
	DEBUG("\r\n\r\nMD2503S: recv buf is '%s' \r\n", USART2_RX_BUF);

	DEBUG("End MD2503S: use GPRS to send messages -----------");
	DEBUG("\r\n\r\n");

}



