#define DEBUG
#include "bsp_pwm.h"
#include "bsp_led.h"
#include "sys_misc.h"

#ifdef DEBUG
	#include "debug.h"
#endif


void LED_Init(void)
{
	Set_LED_Status(LED1, LED_OFF);
	Set_LED_Status(LED2, LED_OFF);
}

void Set_LED_Status(eLED_CHANNEL channel, eLED_Status status)
{
	if(channel >= LED_NULL)
	{
	#ifdef DEBUG
		p_dbg("led channel invalid!");
	#endif
		return;
	}
	
	switch((uint8_t)channel)
	{
		case LED1:
		{
			switch((uint8_t)status)
			{
				case LED_OFF:
					Set_PWM_Value(PWM1, 0);
					break;
				case LED_ON:
					Set_PWM_Value(PWM1, 25);
					break;
				case LED_MODE1_ON:
					Set_PWM_Value(PWM1, 50);
					break;
				case LED_MODE2_ON:
					Set_PWM_Value(PWM1, 75);
					break;
				case LED_MODE3_ON:
					Set_PWM_Value(PWM1, 100);
					break;
			}
		}
		break;
		
		case LED2:
		{
			switch((uint8_t)status)
			{
				case LED_OFF:
					Set_PWM_Value(PWM2, 0);
					break;
				case LED_ON:
					Set_PWM_Value(PWM2, 25);
					break;
				case LED_MODE1_ON:
					Set_PWM_Value(PWM2, 50);
					break;
				case LED_MODE2_ON:
					Set_PWM_Value(PWM2, 75);
					break;
				case LED_MODE3_ON:
					Set_PWM_Value(PWM2, 100);
					break;
			}
		}
		break;
	}
}
