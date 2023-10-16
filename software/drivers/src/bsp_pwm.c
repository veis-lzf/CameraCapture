#define DEBUG
#include "bsp_pwm.h"
#include "sys_misc.h"

#ifdef DEBUG
	#include "debug.h"
#endif

//TIM2 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM2_PWM_Init(uint32_t arr, uint32_t psc)
{		 					 
	//此部分需手动修改IO口设置
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // TIM2时钟使能    
	
	GPIO_PinAFConfig(MAIN_LED_PWM_PORT_GROUP, MAIN_LED_PWM_SOURCE, MAIN_LED_PWM_AF);
	GPIO_PinAFConfig(LED_PWM_PORT_GROUP, LED_PWM_SOURCE, LED_PWM_AF);
	
	gpio_cfg((uint32_t)MAIN_LED_PWM_PORT_GROUP, MAIN_LED_PWM_PIN, GPIO_Mode_AF_PP);
	gpio_cfg((uint32_t)LED_PWM_PORT_GROUP, LED_PWM_PIN, GPIO_Mode_AF_PP);
	
	TIM_TimeBaseStructure.TIM_Prescaler = psc;  // 定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseStructure.TIM_Period = arr;   // 自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); // 初始化定时器2
	
	//初始化TIM2 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // 选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; // 输出极性:TIM输出比较极性低
	//TIM_OCInitStructure.TIM_Pulse = 800 - 1;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  // 根据T指定的参数初始化外设TIM2 OC2
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  // 使能TIM2在CCR2上的预装载寄存器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // 选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; // 输出极性:TIM输出比较极性低
 	//TIM_OCInitStructure.TIM_Pulse = 950 - 1;
	TIM_OC3Init(TIM2, &TIM_OCInitStructure); // 根据T指定的参数初始化外设TIM2 OC3
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable); // 使能TIM2在CCR3上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM2, ENABLE); // ARPE使能 
	TIM_Cmd(TIM2, ENABLE);  // 使能TIM2						  
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

