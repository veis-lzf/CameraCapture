#define DEBUG

#include "debug.h"
#include "drivers.h"
#include "app.h"
#include "api.h"

#define RTCPWD_VALUE	(0x32F2)

/* ѡ��RTC��ʱ��Դ */
//#define RTC_CLOCK_SOURCE_LSE       /* LSE */
#define RTC_CLOCK_SOURCE_LSI     /* LSI */ 
//#define RTC_Debug

RTC_TimeTypeDef  RTC_TimeStructure;
RTC_InitTypeDef  RTC_InitStructure;
RTC_AlarmTypeDef RTC_AlarmStructure;
RTC_DateTypeDef  RTC_DateStructure;

__IO uint32_t uwAsynchPrediv = 0;
__IO uint32_t uwSynchPrediv = 0;
uint8_t aShowTime[50] = {0};


/*
*********************************************************************************************************
*	�� �� ��: RTC_Config
*	����˵��: 1. ѡ��ͬ��RTCʱ��ԴLSI����LSE��
*             2. ����RTCʱ�ӡ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RTC_Config(void)
{
	/* ʹ��PWRʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* �������RTC */
	PWR_BackupAccessCmd(ENABLE);

	/* ѡ��LSI��Ϊʱ��Դ */
#if defined (RTC_CLOCK_SOURCE_LSI)  
	
	/* Enable the LSI OSC */ 
	RCC_LSICmd(ENABLE);

	/* Wait till LSI is ready */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{
	}

	/* ѡ��RTCʱ��Դ */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	
	/* ck_spre(1Hz) = RTCCLK(LSE) /(uwAsynchPrediv + 1)*(uwSynchPrediv + 1)*/
	uwSynchPrediv = 0xFF;
	uwAsynchPrediv = 0x7F;
	
	/* ѡ��LSE��ΪRTCʱ�� */
#elif defined (RTC_CLOCK_SOURCE_LSE)
	/* ʹ��LSE����  */
	RCC_LSEConfig(RCC_LSE_ON);

	/* �ȴ����� */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{
	}

	/* ѡ��RTCʱ��Դ */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	uwSynchPrediv = 0xFF;
	uwAsynchPrediv = 0x7F;

#else
#error Please select the RTC Clock source inside the main.c file
#endif 

	/* ʹ��RTCʱ�� */
	RCC_RTCCLKCmd(ENABLE);

	/* �ȴ�RTC APB�Ĵ���ͬ�� */
	RTC_WaitForSynchro();

	/* ����RTC���ݼĴ����ͷ�Ƶ��  */
	RTC_InitStructure.RTC_AsynchPrediv = uwAsynchPrediv;
	RTC_InitStructure.RTC_SynchPrediv = uwSynchPrediv;
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	RTC_Init(&RTC_InitStructure);

	/* ���������պ����� */
	RTC_DateStructure.RTC_Year = 0x13;
	RTC_DateStructure.RTC_Month = RTC_Month_January;
	RTC_DateStructure.RTC_Date = 0x01;
	RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Saturday;
	RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

	/* ����ʱ���룬�Լ���ʾ��ʽ */
	RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
	RTC_TimeStructure.RTC_Hours   = 0x00;
	RTC_TimeStructure.RTC_Minutes = 0x00;
	RTC_TimeStructure.RTC_Seconds = 0x00; 
	RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);   

	/* ���ñ��ݼĴ�������ʾ�Ѿ����ù�RTC */
	RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);

	/* �������� BCD��ʽ��05Сʱ��20���ӣ�30�� */
	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = 0x05;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0x20;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0x30;
	RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;

	/* ���� RTC Alarm A �Ĵ��� */
	RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);

	/* ʹ�� RTC Alarm A �ж� */
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);

	/* ʹ������ */
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

	/* ���RTC���ӱ�־ */
	RTC_ClearFlag(RTC_FLAG_ALRAF);
}

/* 
*********************************************************************************************************
*  �� �� ��: bsp_InitRTC 
*  ����˵��: ��ʼ��RTC 
*  �� �Σ���  *  �� �� ֵ: ��   
*********************************************************************************************************
*/ 
void bsp_InitRTC(void) 
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
	
	/* ���ڼ���Ƿ��Ѿ����ù�RTC��������ù��Ļ����������ý���ʱ 
	����RTC���ݼĴ���Ϊ0x32F2��������RTC���ݼĴ�������0x32F2   ��ô��ʾû�����ù�����Ҫ����RTC.   */

	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != RTCPWD_VALUE) 
	{ 
		/* RTC ���� */ 
		RTC_Config(); 
		/* ��ӡ������Ϣ */ 
		#ifdef RTC_Debug 
			p_dbg("The First time to use RTC!"); 
		#endif
		
		/* ����ϵ縴λ��־�Ƿ����� */ 
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET) 
		{
		} 
	} 
	else 
	{
		/* ����ϵ縴λ��־�Ƿ����� */ 
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET) 
		{ 
			/* �����ϵ縴λ */ 
			#if defined (RTC_CLOCK_SOURCE_LSI) // �����ʹ��LSI����Ҫÿ�θ�λ/�ϵ綼���ڲ�LSI����ʹ�ܣ���ȻRTC������ʱ
				RCC_LSICmd(ENABLE);
			#endif
		} 
		/* ������Ÿ�λ��־�Ƿ����� */ 
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET) 
		{
		} 
		
		/* ʹ��PWRʱ�� */ 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); 
		/* �������RTC */ 
		PWR_BackupAccessCmd(ENABLE); 
		/* �ȴ� RTC APB �Ĵ���ͬ�� */ 
		RTC_WaitForSynchro(); 
		/* ���RTC���ӱ�־ */ 
		RTC_ClearFlag(RTC_FLAG_ALRAF); 
		/* ���RTC�����жϹ����־ */ 
		EXTI_ClearITPendingBit(EXTI_Line17); 
	}
		
	/* RTC�����ж����� */ 
	/* EXTI ���� */ 
	EXTI_ClearITPendingBit(EXTI_Line17); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line17; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; 
	EXTI_Init(&EXTI_InitStructure); 
	
	/* ʹ��RTC�����ж� */ 
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn; 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = RTC_IRQn_Priority; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
}

void RTC_Time_Set(int y, int m, int d, int wd, int hour, int min, int sec)
{
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;
	//RTC_AlarmTypeDef  RTC_AlarmStructure;

	time.RTC_Hours = hour;
	time.RTC_Minutes = min;
	time.RTC_Seconds = sec;

	if(RTC_SetTime(RTC_Format_BIN, &time) == ERROR)
	{
		p_err("\n\r>> !! RTC Set Time failed. !! <<\n\r");
	} 

	date.RTC_Year = y;
	date.RTC_Month = m;
	date.RTC_Date = d;
	date.RTC_WeekDay = wd;

	if(RTC_SetDate(RTC_Format_BIN, &date) == ERROR)
	{
		p_err("\n\r>> !! RTC Set Date failed. !! <<\n\r");
	}
	
	/* Indicator for the RTC configuration */
	RTC_WriteBackupRegister(RTC_BKP_DR0, RTCPWD_VALUE);
}


/**
  * @brief  Returns the time entered by user, using Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_TimeRegulate(void)
{
	uint32_t tmp_hh = 0, tmp_mm = 0, tmp_ss = 0;
	RTC_TimeTypeDef RTC_TimeStructure;

	RTC_TimeStructure.RTC_Hours = tmp_hh;
	RTC_TimeStructure.RTC_Minutes = tmp_mm;
	RTC_TimeStructure.RTC_Seconds = tmp_ss;

	if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
	{
		p_info("\n\r>> !! RTC Set Time failed. !! <<\n\r");
	} 
	else
	{
		p_info("\n\r>> !! RTC Set Time success. !! <<\n\r");
		RTC_TimeShow();
		/* Indicator for the RTC configuration */
		RTC_WriteBackupRegister(RTC_BKP_DR0, RTCPWD_VALUE);
	}
}

/**
  * @brief  Display the current time on the Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_TimeShow(void)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	/* Get the current Time */
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	p_info("The current time is :  %0.2d:%0.2d:%0.2d", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
}

void RTC_GetTimeString(uint8_t * buf)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	/* Get the current Time */
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	sprintf((char *)buf, "%0.2d%0.2d%0.2d", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
}

uint32_t RTC_GetTraceTick(void)
{
	uint32_t tick = 0;
	RTC_TimeTypeDef RTC_TimeStructure;
	/* Get the current Time */
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	tick = (RTC_TimeStructure.RTC_Hours * 60 * 60) + (RTC_TimeStructure.RTC_Minutes * 60) + RTC_TimeStructure.RTC_Seconds;	
	return ((tick * 1000));
}

/**
  * @brief  Display the current time on the Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_AlarmShow(void)
{
	RTC_AlarmTypeDef  RTC_AlarmStructure;
	/* Get the current Alarm */
	RTC_GetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
	p_info("\n\r  The current alarm is :  %0.2d:%0.2d:%0.2d \n\r", RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours,
		RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes, RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds);
}

// �����жϺ������ж�22��
void RTC_Set_WakeUp(uint16_t cnt)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RTC_WakeUpCmd(DISABLE);
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits); // ���û���ʱ��Դ����1Hz����
	RTC_SetWakeUpCounter(cnt);   // ���������Ի����Զ���װ��ֵ

	EXTI_ClearITPendingBit(EXTI_Line22);  // �����ж�ǰ����ж���
	RTC_ClearITPendingBit(RTC_IT_WUT);   // ����RTCǰ���RTC�����жϱ�־λ

	RTC_ITConfig(RTC_IT_WUT,ENABLE);  // RTC�ж�����
	RTC_WakeUpCmd( ENABLE);           // ����wakeup


	EXTI_InitStructure.EXTI_Line=EXTI_Line22;   // �ⲿ�ж���22
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;   //�����ش���
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn; // RTC_WKUP�ж�ͨ��
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = RTC_WakeupIRQn_Priority; // �����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			// IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	// ����ָ���Ĳ�����ʼ��VIC�Ĵ���
}

// �����ж�(�ⲿ�ж�17)
void RTC_Set_Alarm(uint8_t hour, uint8_t min, uint8_t sec, uint8_t ampm, uint8_t week)    
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_AlarmTypeDef RTC_AlarmStructure;

	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);

	RTC_TimeStructure.RTC_Hours = hour;
	RTC_TimeStructure.RTC_Minutes = min;
	RTC_TimeStructure.RTC_Seconds = sec;
	RTC_TimeStructure.RTC_H12 = ampm;

	RTC_AlarmStructure.RTC_AlarmDateWeekDay = week; // ָ�����ں�����
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay; // ����Ϊ���ڻ�������ѡ��
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_None; // ����ʱ����������
	RTC_AlarmStructure.RTC_AlarmTime = RTC_TimeStructure; // ����ʱ���趨

	RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmStructure); // ��������

	EXTI_ClearITPendingBit(EXTI_Line17); // �����ж�ǰ����ж��ߣ�alarm�ҽ���17��
	RTC_ClearITPendingBit(RTC_IT_ALRA); // ����RTCǰ���alarm�жϱ�־λ    

	RTC_ITConfig(RTC_IT_ALRA,ENABLE); // �����ж�
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE); // �������ӹ���

	EXTI_InitStructure.EXTI_Line=EXTI_Line17; // �ⲿ�ж���22
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;   //�����ش���
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn; // RTC_WKUP�ж�ͨ��
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = RTC_IRQn_Priority; // �����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	// IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	// ����ָ���Ĳ�����ʼ��VIC�Ĵ���
}
