#ifndef __MISC_H__
#define __MISC_H__

#include "type.h"

void usr_gpio_init(void);
void init_systick(void);
void misc_init(void);

void soft_reset(void);
void delay_1us(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址

#endif
