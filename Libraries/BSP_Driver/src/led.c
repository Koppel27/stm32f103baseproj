#include "led.h"
#include "delay.h"

 

//��ʼ��PB��PCΪ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PB/PC�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;				 //LED �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);		//�����趨������ʼ��GPIOA.8
	
	GPIO_SetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
//	GPIO_SetBits(GPIOB,GPIO_Pin_3);						 //PB.4 �����
//	GPIO_SetBits(GPIOB,GPIO_Pin_6);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;				 //LED0-->PA.8 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.8
	GPIO_SetBits(GPIOC,GPIO_Pin_6 | GPIO_Pin_7);						 //PB.4 �����
	//GPIO_SetBits(GPIOA,GPIO_Pin_15);	
}
 
void LED_Scan(void)
{
	LED1=0; delay_ms(300);
	LED2=0; delay_ms(300);
	LED3=0; delay_ms(300);
	LED4=0; delay_ms(300);
	LED5=0; delay_ms(300);
	LED6=0; delay_ms(300);
	LED7=0; delay_ms(300);
	LED8=0; delay_ms(300);
	LED8=1; delay_ms(300);
	LED7=1; delay_ms(300);
	LED6=1; delay_ms(300);
	LED5=1; delay_ms(300);
	LED4=1; delay_ms(300);
	LED3=1; delay_ms(300);
	LED2=1; delay_ms(300);
	LED1=1; delay_ms(300);
}