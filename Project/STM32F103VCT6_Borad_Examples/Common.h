#ifndef __COMMON_H__
#define __COMMON_H__


#include "stm32f10x.h"

#ifndef NULL
  #define NULL ((void*) 0)
#endif


#ifndef GPIO2RCC
  #define GPIO2RCC(GPIOx) (((uint32_t)0x01) << (((uint32_t)(GPIOx) - APB2PERIPH_BASE) >> 10))
#endif


#ifndef GPIOPinIndex2Pin
  #define GPIOPinIndex2Pin(PinIndex)  ((uint16_t)(0x01) << (PinIndex))
#endif

//#define I2C1_BASE             (APB1PERIPH_BASE + 0x5400)
#define I2C2_BASE             (APB1PERIPH_BASE + 0x5800)
#define I2C3_BASE             (APB1PERIPH_BASE + 0x5C00)
	#define RCC_APB1Periph_I2C1              ((uint32_t)0x00200000)
#define RCC_APB1Periph_I2C2              ((uint32_t)0x00400000)
#define RCC_APB1Periph_I2C3              ((uint32_t)0x00800000)




#ifndef I2C2RCC
	#define I2C2RCC(I2Cx)     ( (uint32_t)RCC_APB1Periph_I2C1 << (((uint32_t)(I2Cx) - APB1PERIPH_BASE - 0x5000) >> 11)  )
#endif





#endif
