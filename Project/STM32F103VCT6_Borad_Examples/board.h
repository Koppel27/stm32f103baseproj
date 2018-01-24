#ifndef __BOARD_H__
#define __BOARD_H__

#include "Common.h"

#define TEST_BOARD_V1        0x0001
#define RELEASE_BOARD_V1     0x1001


#define BOARD_TYPE  (TEST_BOARD_V1)


#if BOARD_TYPE == RELEASE_BOARD_V1
	#define _BUTTON_UP_GPIO           GPIOC
	#define _BUTTON_UP_PIN            GPIO_Pin_0
	#define _BUTTON_RIGHT_GPIO        GPIOC
	#define _BUTTON_RIGHT_PIN         GPIO_Pin_1
	#define _BUTTON_SET_GPIO          GPIOC
	#define _BUTTON_SET_PIN           GPIO_Pin_2
	#define _BUTTON_LEFT_GPIO         GPIOC
	#define _BUTTON_LEFT_PIN          GPIO_Pin_4
	#define _BUTTON_DOWN_GPIO         GPIOC
	#define _BUTTON_DOWN_PIN          GPIO_Pin_5
	#define _BUTTON_POWER_GPIO        GPIOC
	#define _BUTTON_POWER_PIN         GPIO_Pin_10
	#define _BUTTON_SWITCH_GPIO       GPIOC
	#define _BUTTON_SWITCH_PIN        GPIO_Pin_12
	#define _BUTTON_RESERVE_GPIO      GPIOC
	#define _BUTTON_RESERVE_PIN       GPIO_Pin_11

	#define _OUT_L1_GPIO              GPIOC
	#define _OUT_L1_PIN               GPIO_Pin_6
	#define _OUT_L2_GPIO              GPIOC
	#define _OUT_L2_PIN               GPIO_Pin_7
	#define _OUT_L3_GPIO              GPIOC
	#define _OUT_L3_PIN               GPIO_Pin_8
	#define _OUT_L4_GPIO              GPIOC
	#define _OUT_L4_PIN               GPIO_Pin_9

	#define _OUT_RWP_GPIO             GPIOD
	#define _OUT_RWP_PIN              GPIO_Pin_12
	#define _OUT_LWP_GPIO             GPIOD
	#define _OUT_LWP_PIN              GPIO_Pin_13

	#define _IN_W1_GPIO               GPIOB
	#define _IN_W1_PIN                GPIO_Pin_8
	#define _IN_W2_GPIO               GPIOB
	#define _IN_W2_PIN                GPIO_Pin_7
	#define _IN_W3_GPIO               GPIOB
	#define _IN_W3_PIN                GPIO_Pin_6
	#define _IN_W4_GPIO               GPIOB
	#define _IN_W4_PIN                GPIO_Pin_5

	#define _IN_TH_GPIO               GPIOB
	#define _IN_TH_PIN                GPIO_Pin_4

	#define _IN_PHH_GPIO              GPIOE
	#define _IN_PHH_PIN               GPIO_Pin_0
	#define _IN_PH_GPIO               GPIOE
	#define _IN_PH_PIN                GPIO_Pin_1

	#define _BEEP_GPIO                GPIOC
	#define _BEEP_PIN                 GPIO_Pin_13

	#define _FLASH_WP_GPIO            GPIOC
	#define _FLASH_WP_PIN             GPIO_Pin_3
	
	#define _LCD_BLK_GPIO             GPIOA
	#define _LCD_BLK_PIN              GPIO_Pin_15
#elif BOARD_TYPE == TEST_BOARD_V1
	#define _BUTTON_UP_GPIO           GPIOB
	#define _BUTTON_UP_PIN            GPIO_Pin_11
	#define _BUTTON_RIGHT_GPIO        GPIOB
	#define _BUTTON_RIGHT_PIN         GPIO_Pin_10
	#define _BUTTON_SET_GPIO          GPIOC
	#define _BUTTON_SET_PIN           GPIO_Pin_5
	#define _BUTTON_LEFT_GPIO         GPIOC
	#define _BUTTON_LEFT_PIN          GPIO_Pin_4
	#define _BUTTON_DOWN_GPIO         GPIOC
	#define _BUTTON_DOWN_PIN          GPIO_Pin_3
	#define _BUTTON_POWER_GPIO        GPIOC
	#define _BUTTON_POWER_PIN         GPIO_Pin_2
	#define _BUTTON_SWITCH_GPIO       GPIOC
	#define _BUTTON_SWITCH_PIN        GPIO_Pin_1
	#define _BUTTON_RESERVE_GPIO      GPIOC
	#define _BUTTON_RESERVE_PIN       GPIO_Pin_0

	#define _OUT_L1_GPIO              GPIOB             //LED1
	#define _OUT_L1_PIN               GPIO_Pin_13
	#define _OUT_L2_GPIO              GPIOB             //LED2
	#define _OUT_L2_PIN               GPIO_Pin_12
	#define _OUT_L3_GPIO              GPIOB             //LED3
	#define _OUT_L3_PIN               GPIO_Pin_14
	#define _OUT_L4_GPIO              GPIOB             //LED4
	#define _OUT_L4_PIN               GPIO_Pin_15

	#define _OUT_RWP_GPIO             GPIOC             //LED5
	#define _OUT_RWP_PIN              GPIO_Pin_6
	#define _OUT_LWP_GPIO             GPIOC             //LED6
	#define _OUT_LWP_PIN              GPIO_Pin_7

	#define _BEEP_GPIO                GPIOC
	#define _BEEP_PIN                 GPIO_Pin_13

	#define _FLASH_WP_GPIO            GPIOB
	#define _FLASH_WP_PIN             GPIO_Pin_0
	#define _FLASH_CLK_GPIO           GPIOA
	#define _FLASH_CLK_PIN            GPIO_Pin_5
	#define _FLASH_MOSI_GPIO          GPIOA
	#define _FLASH_MOSI_PIN           GPIO_Pin_7
	#define _FLASH_MISO_GPIO          GPIOA
	#define _FLASH_MISO_PIN           GPIO_Pin_6
	#define _FLASH_CS_GPIO            GPIOA
	#define _FLASH_CS_PIN             GPIO_Pin_4
	
	#define _UART1_TX_GPIO            GPIOA
	#define _UART1_TX_PIN             GPIO_Pin_9
	#define _UART1_RX_GPIO            GPIOA
	#define _UART1_RX_PIN             GPIO_Pin_10
	
	#define _ADC_CH1_GPIO             GPIOA
	#define _ADC_CH1_PIN              GPIO_Pin_1
	#define _ADC_CH2_GPIO             GPIOA
	#define _ADC_CH2_PIN              GPIO_Pin_2
	#define _ADC_CH3_GPIO             GPIOA
	#define _ADC_CH3_PIN              GPIO_Pin_3
	
	#define _MAX6675_SCK_GPIO         GPIOA
	#define _MAX6675_SCK_PIN          GPIO_Pin_11
	#define _MAX6675_CS_GPIO          GPIOA
	#define _MAX6675_CS_PIN           GPIO_Pin_12
	#define _MAX6675_SO_GPIO          GPIOA
	#define _MAX6675_SO_PIN           GPIO_Pin_15
#endif



#endif
