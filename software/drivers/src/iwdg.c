#define DEBUG
#include "iwdg.h"
#include "sys_misc.h"

/**************************************
�������ƣ�WDT_init
��    ��: ��ʼ�����Ź�,LSI=32KHz
��    ����prer , rlr
�� �� ֵ��void
**************************************/
void WDT_init(u8 prer,u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//ʹ�ܶ�IWDG->PR IWDG->RLR��д
	
	IWDG_SetPrescaler(prer);//����IWDG��Ƶϵ��
	
	IWDG_SetReload(rlr); //����IWDGװ��ֵ
	
	IWDG_ReloadCounter();//reload
	
	IWDG_Enable(); //ʹ�ܿ��Ź�
}


/**************************************
�������ƣ�WDT_init
��    ��: ��ʼ�����Ź�
��    ����prer , rlr
�� �� ֵ��void
**************************************/
void IWDG_Feed()
{
	IWDG_ReloadCounter();//reload
}
