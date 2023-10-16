#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "drivers.h"

typedef enum
{
	LED1 = 0,
	LED2,
	
	LED_NULL,
} eLED_CHANNEL;

typedef enum
{
	LED_OFF, // 关闭LED
	LED_ON,	// 25%占空比打开
	LED_MODE1_ON, // 50%占空比
	LED_MODE2_ON, // 75%占空比
	LED_MODE3_ON, // 100%占空比
} eLED_Status;

void LED_Init(void);
void Set_LED_Status(eLED_CHANNEL channel, eLED_Status status);

#endif
