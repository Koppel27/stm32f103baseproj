#ifndef __LED_H
#define __LED_H	 
#include "bsp_common.h"
						  
////////////////////////////////////////////////////////////////////////////////// 

#define LED1 PBout(13)	// PB13
#define LED2 PBout(12)	// PB12
#define LED3 PBout(14)	// PB14
#define LED4 PBout(15)	// PB15
#define LED5 PCout(6)	// PC6
#define LED6 PCout(7)	// PC7
#define LED7 PBout(8)	// PB8
#define LED8 PBout(9)	// PB9

#define PWRKEY PBout(9) 


#define ON          0
#define OFF         1


void LED_Init(void);//≥ı ºªØ

void LED_Scan(void);

		 				    
#endif
