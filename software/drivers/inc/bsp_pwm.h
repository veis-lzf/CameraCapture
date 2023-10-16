#ifndef _TIMER_H
#define _TIMER_H

#include "drivers.h"

typedef enum
{
	PWM1 = 0,
	PWM2,
	PWM3,
	
	PWM_NULL,
} ePWM_CHANNEL;

void TIM2_PWM_Init(uint32_t arr, uint32_t psc);
void Set_PWM_Value(ePWM_CHANNEL pwm, uint8_t val);

#endif
