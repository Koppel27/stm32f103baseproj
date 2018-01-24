#ifndef __USART2_H
#define __USART2_H	 
#include "bsp_common.h"  
							  
////////////////////////////////////////////////////////////////////////////////// 	     
 
#define USART2_MAX_RECV_LEN		800					//�����ջ����ֽ���
#define USART2_MAX_SEND_LEN		200					//����ͻ����ֽ���
#define USART2_RX_EN 			1					//0,������;1,����.

extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		//���ջ���,���USART2_MAX_RECV_LEN�ֽ�
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
extern u16 USART2_RX_STA;   						//��������״̬

void USART2_Init(u32 bound);				//����2��ʼ�� 
void TIM4_Set(u8 sta);
void TIM4_Init(u16 arr,u16 psc);
void UART_DMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar);
void UART_DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u8 len);
void u2_printf(char* fmt, ...);
//Lawrence-20170519
#define is_uart2_recv_data()	(USART2_RX_STA & 0X8000)
#define uart2_recv_datalen()	(USART2_RX_STA & 0X7FFF)
#define uart2_start_recv()		do{memset(USART2_RX_BUF, 0, USART2_MAX_RECV_LEN); USART2_RX_STA = 0;}while(0)
#define uart2_send_string		u2_printf
#endif










                                                                                       


