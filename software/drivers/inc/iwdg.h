#ifndef __IWDG_H__
#define	__IWDG_H__

#include "bsp.h"
#include "type.h"

/**************************************
函数名称：WDT_init
功    能: 初始化看门狗
参    数：prer , rlr
返 回 值：void
**************************************/
void WDT_init(u8 prer, u16 rlr);

/**************************************
函数名称：WDT_init
功    能: 初始化看门狗
参    数：prer , rlr
返 回 值：void
**************************************/
void IWDG_Feed(void);

#endif
