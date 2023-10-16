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
*                                      定义中断向量表地址
* 因低16k地址用于bootloader，我们的代码需要从16k开始，所以需要在启动时将中断向量重新映射到16k地址
* 目前只对F205有效，F103版本不使用bootloader
*********************************************************************************************************
*/
//#define IVECTOR_ADDR 					(16*1024)
#define IVECTOR_ADDR 					(0)		//如果不使用bootloaer（同时将IROM1的start改为0x8000000）

#endif /* __APP_CFG_H__ */
