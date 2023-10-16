#define DEBUG
#include "bsp_pwm.h"
#include "sys_misc.h"

#ifdef DEBUG
	#include "debug.h"
#endif

//TIM2 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM2_PWM_Init(uint32_t arr, uint32_t psc)
{		 					 
	//�˲������ֶ��޸�IO������
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // TIM2ʱ��ʹ��    
	
	GPIO_PinAFConfig(MAIN_LED_PWM_PORT_GROUP, MAIN_LED_PWM_SOURCE, MAIN_LED_PWM_AF);
	GPIO_PinAFConfig(LED_PWM_PORT_GROUP, LED_PWM_SOURCE, LED_PWM_AF);
	
	gpio_cfg((uint32_t)MAIN_LED_PWM_PORT_GROUP, MAIN_LED_PWM_PIN, GPIO_Mode_AF_PP);
	gpio_cfg((uint32_t)LED_PWM_PORT_GROUP, LED_PWM_PIN, GPIO_Mode_AF_PP);
	
	TIM_TimeBaseStructure.TIM_Prescaler = psc;  // ��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period = arr;   // �Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); // ��ʼ����ʱ��2
	
	//��ʼ��TIM2 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // �Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; // �������:TIM����Ƚϼ��Ե�
	//TIM_OCInitStructure.TIM_Pulse = 800 - 1;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  // ����Tָ���Ĳ�����ʼ������TIM2 OC2
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  // ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // �Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; // �������:TIM����Ƚϼ��Ե�
 	//TIM_OCInitStructure.TIM_Pulse = 950 - 1;
	TIM_OC3Init(TIM2, &TIM_OCInitStructure); // ����Tָ���Ĳ�����ʼ������TIM2 OC3
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable); // ʹ��TIM2��CCR3�ϵ�Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM2, ENABLE); // ARPEʹ�� 
	TIM_Cmd(TIM2, ENABLE);  // ʹ��TIM2						  
}  

void Set_PWM_Value(ePWM_CHANNEL pwm, uint8_t val)
{
	uint16_t pwm_val = 1000;
	if(pwm >= PWM_NULL)
	{
		#ifdef DEBUG
			p_dbg("pwm channel invalid!");
		#endif
		return;
	}
	if(val > 100)
	{
		#ifdef DEBUG
			p_dbg("pwm val invalid!");
		#endif
		return;
	}
	
	pwm_val = pwm_val - (float)(pwm_val * (val / 100.0)-1) ;

	switch((uint8_t)pwm)
	{
		case PWM1:
		{
			if(pwm_val == 0)
			{
				gpio_cfg((uint32_t)MAIN_LED_PWM_PORT_GROUP, MAIN_LED_PWM_PIN, GPIO_Mode_Out_PP);
				gpio_set_level((uint32_t)MAIN_LED_PWM_PORT_GROUP, MAIN_LED_PWM_PIN, LOW_LEVEL);
			}
			else
			{
				gpio_cfg((uint32_t)MAIN_LED_PWM_PORT_GROUP, MAIN_LED_PWM_PIN, GPIO_Mode_AF_PP);
				TIM_SetCompare2(TIM2, pwm_val);
			}
		}
		break;
		case PWM2:
		{
			if(pwm_val == 0)
			{
				gpio_cfg((uint32_t)LED_PWM_PORT_GROUP, LED_PWM_PIN, GPIO_Mode_Out_PP);
				gpio_set_level((uint32_t)LED_PWM_PORT_GROUP, LED_PWM_PIN, LOW_LEVEL);
			}
			else
			{
				gpio_cfg((uint32_t)LED_PWM_PORT_GROUP, LED_PWM_PIN, GPIO_Mode_AF_PP);
				TIM_SetCompare3(TIM2, pwm_val);
			}
		}
		break;
	}
}

