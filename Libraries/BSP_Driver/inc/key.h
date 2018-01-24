#ifndef __KEY_H
#define __KEY_H	 
#include "bsp_common.h"
//////////////////////////////////////////////////////////////////////////////////	 
					  
//////////////////////////////////////////////////////////////////////////////////   	 


//#define KEY0 PCin(5)   	
//#define KEY1 PAin(15)	 
//#define WK_UP  PAin(0)	 
 

#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)//读取按键1
#define KEY2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)//读取按键2
#define KEY3  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)//读取按键3 
#define KEY4  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)//读取按键4 
#define KEY5  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)//读取按键5 
#define KEY6  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)//读取按键6 
#define KEY7  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)//读取按键7 
#define KEY8  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)//读取按键8 

#define KEY1_PRES	1		//KEY1  
#define KEY2_PRES	2		//KEY2 
#define KEY3_PRES	3		//KEY3 
#define KEY4_PRES	4		//KEY4 
#define KEY5_PRES	5		//KEY5 
#define KEY6_PRES	6		//KEY6 
#define KEY7_PRES	7		//KEY7 
#define KEY8_PRES	8		//KEY8 



void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8 mode);  	//按键扫描函数					    
#endif
