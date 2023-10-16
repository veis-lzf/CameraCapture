#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "type.h"

enum IRQ_PRIORITY{
	
	TIM2_IRQn_Priority = 0,
	OTG_HS_IRQn_Priority=1,
	USART1_IRQn_Priority = 2,
	USART3_IRQn_Priority = 2,
	RTC_IRQn_Priority = 3,
	RTC_WakeupIRQn_Priority = 2,
	SDIO_IRQn_Priority,
	SDCARD_DET_IRQ_Priority = 1,
	DMA2_Stream1_IRQn_Priority = 12,	
	DCMI_IRQn_Priority = 11,
	TIM3_IRQn_Priority = 13,
	SysTick_IRQ_Priority = 15	//
};


/*
*********************************************************************************************************
*                                      �����ж��������ַ
* ���16k��ַ����bootloader�����ǵĴ�����Ҫ��16k��ʼ��������Ҫ������ʱ���ж���������ӳ�䵽16k��ַ
* Ŀǰֻ��F205��Ч��F103�汾��ʹ��bootloader
*********************************************************************************************************
*/
//#define IVECTOR_ADDR 					(16*1024)
#define IVECTOR_ADDR 					(0)		//�����ʹ��bootloaer��ͬʱ��IROM1��start��Ϊ0x8000000��

#endif /* __APP_CFG_H__ */
