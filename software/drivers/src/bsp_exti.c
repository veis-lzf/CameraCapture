#include "bsp_exti.h"
#include "sys_misc.h"
#include "app_cfg.h"
#include "debug.h"
#include "drivers.h"

#define SDCARD_PUSHIN	0
#define SDCARD_PUSHOUT	1

//�ⲿ�ж�15�������
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
	EXTI_ClearITPendingBit(EXTI_Line15); //���LINE0�ϵ��жϱ�־λ 
}
	   
//�ⲿ�жϳ�ʼ������
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, SD_DET_SOURCE);//PA15 ���ӵ��ж���15
	
	/* ����EXTI_Line15 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // �ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // �����ش��� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SDCARD_DET_IRQ_Priority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);  
}












