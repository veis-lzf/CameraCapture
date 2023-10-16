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
	LED_OFF, // �ر�LED
	LED_ON,	// 25%ռ�ձȴ�
	LED_MODE1_ON, // 50%ռ�ձ�
	LED_MODE2_ON, // 75%ռ�ձ�
	LED_MODE3_ON, // 100%ռ�ձ�
} eLED_Status;

void LED_Init(void);
void Set_LED_Status(eLED_CHANNEL channel, eLED_Status status);

#endif
