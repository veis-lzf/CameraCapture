#ifndef __IWDG_H__
#define	__IWDG_H__

#include "bsp.h"
#include "type.h"

/**************************************
�������ƣ�WDT_init
��    ��: ��ʼ�����Ź�
��    ����prer , rlr
�� �� ֵ��void
**************************************/
void WDT_init(u8 prer, u16 rlr);

/**************************************
�������ƣ�WDT_init
��    ��: ��ʼ�����Ź�
��    ����prer , rlr
�� �� ֵ��void
**************************************/
void IWDG_Feed(void);

#endif
