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

//����Ϊ��ຯ��
void WFI_SET(void);		//ִ��WFIָ��
void INTX_DISABLE(void);//�ر������ж�
void INTX_ENABLE(void);	//���������ж�
void MSR_MSP(u32 addr);	//���ö�ջ��ַ

#endif
