#include "bsp_exti.h"
#include "sys_misc.h"
#include "app_cfg.h"
#include "debug.h"
#include "drivers.h"

#define SDCARD_PUSHIN	0
#define SDCARD_PUSHOUT	1

//外部中断15服务程序
void EXTI15_10_IRQHandler(void)
{
	delay_ms(10);
	if(GPIO_ReadInputDataBit(SD_DET_PORT, SD_DET_PIN) == SDCARD_PUSHIN)
	{
		p_info("sd card push in!");
		Init_SDCARD();
	}
	else
	{
		p_info("sd card push out!");
	}
	EXTI_ClearITPendingBit(EXTI_Line15); //清除LINE0上的中断标志位 
}
	   
//外部中断初始化程序
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, SD_DET_SOURCE);//PA15 连接到中断线15
	
	/* 配置EXTI_Line15 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // 中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // 上升沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SDCARD_DET_IRQ_Priority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);  
}












