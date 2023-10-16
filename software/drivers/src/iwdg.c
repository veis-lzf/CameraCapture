#define DEBUG
#include "iwdg.h"
#include "sys_misc.h"

/**************************************
函数名称：WDT_init
功    能: 初始化看门狗,LSI=32KHz
参    数：prer , rlr
返 回 值：void
**************************************/
void WDT_init(u8 prer,u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//使能对IWDG->PR IWDG->RLR的写
	
	IWDG_SetPrescaler(prer);//设置IWDG分频系数
	
	IWDG_SetReload(rlr); //设置IWDG装载值
	
	IWDG_ReloadCounter();//reload
	
	IWDG_Enable(); //使能看门狗
}


/**************************************
函数名称：WDT_init
功    能: 初始化看门狗
参    数：prer , rlr
返 回 值：void
**************************************/
void IWDG_Feed()
{
	IWDG_ReloadCounter();//reload
}
