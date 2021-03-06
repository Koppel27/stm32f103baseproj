//#include "delay.h"
#include "usart2.h"
#include <stdarg.h> 	 
#include "stdio.h"	 	 
#include "string.h"	   
#include "dma.h"
							  
////////////////////////////////////////////////////////////////////////////////// 	   

//串口发送缓存区 	
__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 	//发送缓冲,最大USART2_MAX_SEND_LEN字节

#ifdef USART2_RX_EN   								//如果使能了接收   

//串口接收缓存区 	
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//接收缓冲,最大USART2_MAX_RECV_LEN个字节.


//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
u16 USART2_RX_STA=0;   	 
void USART2_IRQHandler(void)
{
	u8 res;	

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		res =USART_ReceiveData(USART2);	//读取接收到的数据
		USART_SendData(USART2, (u16)res);
//		if((USART2_RX_STA&0x8000)==0)//接收未完成
//			{
//			if(USART2_RX_STA&0x4000)//接收到了0x0d
//				{
//				if(res!=0x0a)USART2_RX_STA=0;//接收错误,重新开始
//				else USART2_RX_STA|=0x8000;	//接收完成了 
//				}
//			else //还没收到0X0D
//				{	
//				if(res==0x0d)USART2_RX_STA|=0x4000;
//				else
//					{
//					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=res ;
//					USART2_RX_STA++;
//					if(USART2_RX_STA>(USART2_MAX_RECV_LEN-1))USART2_RX_STA=0;//接收数据错误,重新开始接收	  
//					}		 
//				}
//			}   		 
     }     
//	if(USART2->SR & (1<<5))//接收到数据
//	{	 
//		res=USART2->DR; 			 
//		if(USART2_RX_STA < USART2_MAX_RECV_LEN)		//还可以接收数据
//		{
//			TIM4->CNT=0;         					//计数器清空
//			if(USART2_RX_STA==0)TIM4_Set(1);	 	//使能定时器4的中断 
//			USART2_RX_BUF[USART2_RX_STA++]=res;		//记录接收到的值	 
//		}else 
//		{
//			USART2_RX_STA |= 1<<15;					//强制标记接收完成
//		} 
//	}  											 
}   


//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	  
void USART2_Init(u32 bound)
{ 
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART1，GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART2, &USART_InitStructure); //初始化串口1
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART2, ENABLE);                    //使能串口1 
	
	//MYDMA_Config(DMA1_Channel7,(u32)&USART2->DR,(u32)USART2_TX_BUF,0);
	UART2_DMA_Config(DMA1_Channel7,(u32)&USART2->DR,(u32)USART2_TX_BUF);

//#ifdef USART2_RX_EN		  	//如果使能了接收
//	//使能接收中断
//	USART2->CR1|=1<<8;    	//PE中断使能
//	USART2->CR1|=1<<5;    	//接收缓冲区非空中断使能	    	
//	TIM4_Init(99,7199);		//10ms中断
//	USART2_RX_STA=0;		//清零
//	TIM4_Set(0);			//关闭定时器4
//#endif										  	
}


//串口2,printf 函数
//确保一次发送数据不超过USART2_MAX_SEND_LEN字节
const u8 TEXT_TO_SEND1[]={"SEC-CHIP STM32F103 DMA UART TEST"};
#define TEXT_LENTH sizeof(TEXT_TO_SEND1)-1
extern u8 SendBuff[(TEXT_LENTH+2)*100];

void u2_printf(char* fmt, ...)  
{  
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	    
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE); //enable uart dma request
	UART2_DMA_Enable(DMA1_Channel7,strlen((const char*)USART2_TX_BUF));//sent out by dma
	while(DMA_GetFlagStatus(DMA1_FLAG_TC7)==RESET);//等待通道 7 传输完成
	DMA_ClearFlag(DMA1_FLAG_TC7);//清除通道7 传输完成标志
}

//定时器4中断服务程序		    
void TIM4_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		USART2_RX_STA |= 1<<15;	//标记接收完成
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		TIM4_Set(0);			//关闭TIM4  
	}   
}


//设置TIM4的开关
//sta:0，关闭;1,开启;
void TIM4_Set(u8 sta)
{
	if(sta)
	{
    	TIM_SetCounter(TIM4, 0);//计数器清空
		TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设	
	}
	else
	{
		TIM_Cmd(TIM4, DISABLE);  //使能TIMx外设
	}
}


//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM4_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM4, //TIM2
		TIM_IT_Update ,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设				 
}
#endif	





















