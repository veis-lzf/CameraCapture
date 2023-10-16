#ifndef __DRIVER_MISC_H__
#define __DRIVER_MISC_H__

#include "type.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define GPIO_SET(port,pin) (port->BSRRL = pin)
#define GPIO_CLR(port,pin) (port->BSRRH = pin)
#define GPIO_STAT(port,pin)(!!(port->IDR & pin))

typedef enum 
{
	LOW_LEVEL = 0,
	HIGH_LEVEL,
} GPIO_Level;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define SD_CARD_STATUS_ERROR	0
#define SD_CARD_STATUS_OK		1

//define UART PORT
// 用于调试日志输出
#define UART1_RX_PORT_GROUP 		GPIOA
#define UART1_TX_PORT_GROUP 		GPIOA
#define UART1_TX_PIN 				GPIO_Pin_9
#define UART1_RX_PIN 				GPIO_Pin_10
#define UART1_TX_PIN_SOURSE			GPIO_PinSource9
#define UART1_RX_PIN_SOURSE			GPIO_PinSource10

// 用于语音模块
#define UART3_RX_PORT_GROUP 		GPIOD
#define UART3_TX_PORT_GROUP 		GPIOD
#define UART3_TX_PIN 				GPIO_Pin_8
#define UART3_RX_PIN 				GPIO_Pin_9
#define UART3_TX_PIN_SOURSE			GPIO_PinSource8
#define UART3_RX_PIN_SOURSE			GPIO_PinSource9

//define USB PORT

#define USB_DP_PORT_GROUP 		GPIOA
#define USB_DM_PORT_GROUP 		GPIOA

#define USB_DP_PIN 				GPIO_Pin_12
#define USB_DM_PIN 				GPIO_Pin_11

#define USB_DP_PIN_SOURSE		GPIO_PinSource12
#define USB_DM_PIN_SOURSE		GPIO_PinSource11

//define sdio PORT

#define SDIO_CLK_PORT				GPIOC
#define SDIO_CLK_PIN				GPIO_Pin_12
#define SDIO_CLK_SOURCE				GPIO_PinSource12

#define SDIO_D0_PORT				GPIOC
#define SDIO_D0_PIN					GPIO_Pin_8
#define SDIO_D0_SOURCE				GPIO_PinSource8

#define SDIO_D1_PORT				GPIOC
#define SDIO_D1_PIN					GPIO_Pin_9
#define SDIO_D1_SOURCE				GPIO_PinSource9

#define SDIO_D2_PORT				GPIOC
#define SDIO_D2_PIN					GPIO_Pin_10
#define SDIO_D2_SOURCE				GPIO_PinSource10

#define SDIO_D3_PORT				GPIOC
#define SDIO_D3_PIN					GPIO_Pin_11
#define SDIO_D3_SOURCE				GPIO_PinSource11

#define SDIO_CMD_PORT				GPIOD
#define SDIO_CMD_PIN				GPIO_Pin_2
#define SDIO_CMD_SOURCE				GPIO_PinSource2

#define SD_DET_PORT					GPIOA
#define SD_DET_PIN					GPIO_Pin_15
#define SD_DET_SOURCE				GPIO_PinSource15
//end define sdio PORT

//define DCMI PORT
#define DCMI_HS_PORT_GROUP 			GPIOA
#define DCMI_HS_PIN 				GPIO_Pin_4
#define DCMI_HS_SOURCE 				GPIO_PinSource4

#define DCMI_VS_PORT_GROUP 			GPIOB
#define DCMI_VS_PIN 				GPIO_Pin_7
#define DCMI_VS_SOURCE 				GPIO_PinSource7

#define DCMI_PCLK_PORT_GROUP 		GPIOA
#define DCMI_PCLK_PIN 				GPIO_Pin_6
#define DCMI_PCLK_SOURCE 			GPIO_PinSource6

#define DCMI_D0_PORT_GROUP 			GPIOC
#define DCMI_D0_PIN 				GPIO_Pin_6
#define DCMI_D0_SOURCE 				GPIO_PinSource6

#define DCMI_D1_PORT_GROUP 			GPIOC
#define DCMI_D1_PIN 				GPIO_Pin_7
#define DCMI_D1_SOURCE 				GPIO_PinSource7
		
#define DCMI_D2_PORT_GROUP 			GPIOE
#define DCMI_D2_PIN 				GPIO_Pin_0
#define DCMI_D2_SOURCE 				GPIO_PinSource0
		
#define DCMI_D3_PORT_GROUP 			GPIOE
#define DCMI_D3_PIN 				GPIO_Pin_1
#define DCMI_D3_SOURCE 				GPIO_PinSource1
		
#define DCMI_D4_PORT_GROUP 			GPIOE
#define DCMI_D4_PIN 				GPIO_Pin_4
#define DCMI_D4_SOURCE 				GPIO_PinSource4
		
#define DCMI_D5_PORT_GROUP 			GPIOB
#define DCMI_D5_PIN 				GPIO_Pin_6
#define DCMI_D5_SOURCE 				GPIO_PinSource6
		
#define DCMI_D6_PORT_GROUP 			GPIOB
#define DCMI_D6_PIN 				GPIO_Pin_8
#define DCMI_D6_SOURCE 				GPIO_PinSource8
		
#define DCMI_D7_PORT_GROUP 			GPIOE
#define DCMI_D7_PIN 				GPIO_Pin_6
#define DCMI_D7_SOURCE 				GPIO_PinSource6

// 摄像头采用外部晶振，不配置此时钟输出
#define MCO_PORT_GROUP 		 		NULL
#define MCO_PIN 					NULL
#define MCO_PIN_SOURCE 				NULL

// 闪光灯控制引脚（预留）
#define STROBE_PORT_GROUP 		 	GPIOB
#define STROBE_PIN 					GPIO_Pin_0

#define DCMI_RST_PORT_GROUP 		GPIOB
#define DCMI_RST_PIN 				GPIO_Pin_1
	
#define DCMI_PWD_PORT_GROUP 		GPIOE
#define DCMI_PWD_PIN				GPIO_Pin_5
	
#define SIO_C_PORT_GROUP 			GPIOE
#define SIO_C_PIN 					GPIO_Pin_2
	
#define SIO_D_PORT_GROUP 			GPIOE
#define SIO_D_PIN 					GPIO_Pin_3

#define SIO_D_OUT					gpio_cfg((uint32_t)SIO_D_PORT_GROUP, SIO_D_PIN, GPIO_Mode_Out_PP);
#define SIO_D_IN					gpio_cfg((uint32_t)SIO_D_PORT_GROUP, SIO_D_PIN, GPIO_Mode_IPU);

//end define DCMI PORT

//define power PORT

// 摄像头电源使能，高开低关
#define CAMERA_PWR_PORT_GROUP 		GPIOC
#define CAMERA_PWR_PIN 				GPIO_Pin_0

// TF卡电源使能，高开低关
#define VSD_POW_PORT_GROUP			GPIOD
#define VSD_POW_PIN 				GPIO_Pin_6

// SU-03T语音识别模块电源使能，高开低关
#define VOICE_POW_PORT_GROUP		GPIOA
#define VOICE_POW_PIN 				GPIO_Pin_8

//end define power PORT

//define pwm PORT
#define MAIN_LED_PWM_PORT_GROUP		GPIOA
#define MAIN_LED_PWM_PIN			GPIO_Pin_1
#define MAIN_LED_PWM_SOURCE 		GPIO_PinSource1
#define MAIN_LED_PWM_AF				GPIO_AF_TIM2

#define LED_PWM_PORT_GROUP			GPIOA
#define LED_PWM_PIN					GPIO_Pin_2
#define LED_PWM_SOURCE 				GPIO_PinSource2
#define LED_PWM_AF					GPIO_AF_TIM2

//end define pwm PORT

void gpio_cfg(uint32_t group, uint32_t pin, uint8_t mode);
void gpio_set_level(uint32_t group, uint32_t pin, GPIO_Level level);
void usr_gpio_init(void);
int check_rst_stat(void);
void driver_misc_init(void);
uint32_t get_random(void);
void driver_gpio_init(void);
void _mem_init(void);
uint32_t get_us_count(void);

#endif
