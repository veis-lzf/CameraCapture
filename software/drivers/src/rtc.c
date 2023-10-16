#define DEBUG

#include "debug.h"
#include "drivers.h"
#include "app.h"
#include "api.h"

#define RTCPWD_VALUE	(0x32F2)

/* 选择RTC的时钟源 */
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
*	函 数 名: RTC_Config
*	功能说明: 1. 选择不同的RTC时钟源LSI或者LSE。
*             2. 配置RTC时钟。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void RTC_Config(void)
{
	/* 使能PWR时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* 允许访问RTC */
	PWR_BackupAccessCmd(ENABLE);

	/* 选择LSI作为时钟源 */
#if defined (RTC_CLOCK_SOURCE_LSI)  
	
	/* Enable the LSI OSC */ 
	RCC_LSICmd(ENABLE);

	/* Wait till LSI is ready */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{
	}

	/* 选择RTC时钟源 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	
	/* ck_spre(1Hz) = RTCCLK(LSE) /(uwAsynchPrediv + 1)*(uwSynchPrediv + 1)*/
	uwSynchPrediv = 0xFF;
	uwAsynchPrediv = 0x7F;
	
	/* 选择LSE作为RTC时钟 */
#elif defined (RTC_CLOCK_SOURCE_LSE)
	/* 使能LSE振荡器  */
	RCC_LSEConfig(RCC_LSE_ON);

	/* 等待就绪 */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{
	}

	/* 选择RTC时钟源 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	uwSynchPrediv = 0xFF;
	uwAsynchPrediv = 0x7F;

#else
#error Please select the RTC Clock source inside the main.c file
#endif 

	/* 使能RTC时钟 */
	RCC_RTCCLKCmd(ENABLE);

	/* 等待RTC APB寄存器同步 */
	RTC_WaitForSynchro();

	/* 配置RTC数据寄存器和分频器  */
	RTC_InitStructure.RTC_AsynchPrediv = uwAsynchPrediv;
	RTC_InitStructure.RTC_SynchPrediv = uwSynchPrediv;
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	RTC_Init(&RTC_InitStructure);

	/* 设置年月日和星期 */
	RTC_DateStructure.RTC_Year = 0x13;
	RTC_DateStructure.RTC_Month = RTC_Month_January;
	RTC_DateStructure.RTC_Date = 0x01;
	RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Saturday;
	RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

	/* 设置时分秒，以及显示格式 */
	RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
	RTC_TimeStructure.RTC_Hours   = 0x00;
	RTC_TimeStructure.RTC_Minutes = 0x00;
	RTC_TimeStructure.RTC_Seconds = 0x00; 
	RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);   

	/* 配置备份寄存器，表示已经设置过RTC */
	RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);

	/* 设置闹钟 BCD格式，05小时，20分钟，30秒 */
	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = 0x05;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0x20;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0x30;
	RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;

	/* 配置 RTC Alarm A 寄存器 */
	RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);

	/* 使能 RTC Alarm A 中断 */
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);

	/* 使能闹钟 */
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

	/* 清除RTC闹钟标志 */
	RTC_ClearFlag(RTC_FLAG_ALRAF);
}

/* 
*********************************************************************************************************
*  函 数 名: bsp_InitRTC 
*  功能说明: 初始化RTC 
*  形 参：无  *  返 回 值: 无   
*********************************************************************************************************
*/ 
void bsp_InitRTC(void) 
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
	
	/* 用于检测是否已经配置过RTC，如果配置过的话，会在配置结束时 
	设置RTC备份寄存器为0x32F2。如果检测RTC备份寄存器不是0x32F2   那么表示没有配置过，需要配置RTC.   */

	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != RTCPWD_VALUE) 
	{ 
		/* RTC 配置 */ 
		RTC_Config(); 
		/* 打印调试信息 */ 
		#ifdef RTC_Debug 
			p_dbg("The First time to use RTC!"); 
		#endif
		
		/* 检测上电复位标志是否设置 */ 
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET) 
		{
		} 
	} 
	else 
	{
		/* 检测上电复位标志是否设置 */ 
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET) 
		{ 
			/* 发生上电复位 */ 
			#if defined (RTC_CLOCK_SOURCE_LSI) // 如果是使用LSI，需要每次复位/上电都对内部LSI进行使能，不然RTC不会走时
				RCC_LSICmd(ENABLE);
			#endif
		} 
		/* 检测引脚复位标志是否设置 */ 
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET) 
		{
		} 
		
		/* 使能PWR时钟 */ 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); 
		/* 允许访问RTC */ 
		PWR_BackupAccessCmd(ENABLE); 
		/* 等待 RTC APB 寄存器同步 */ 
		RTC_WaitForSynchro(); 
		/* 清除RTC闹钟标志 */ 
		RTC_ClearFlag(RTC_FLAG_ALRAF); 
		/* 清除RTC闹钟中断挂起标志 */ 
		EXTI_ClearITPendingBit(EXTI_Line17); 
	}
		
	/* RTC闹钟中断配置 */ 
	/* EXTI 配置 */ 
	EXTI_ClearITPendingBit(EXTI_Line17); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line17; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; 
	EXTI_Init(&EXTI_InitStructure); 
	
	/* 使能RTC闹钟中断 */ 
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

// 唤醒中断函数（中断22）
void RTC_Set_WakeUp(uint16_t cnt)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RTC_WakeUpCmd(DISABLE);
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits); // 设置唤醒时钟源，以1Hz计数
	RTC_SetWakeUpCounter(cnt);   // 设置周期性唤醒自动重装载值

	EXTI_ClearITPendingBit(EXTI_Line22);  // 开启中断前清除中断线
	RTC_ClearITPendingBit(RTC_IT_WUT);   // 开启RTC前清除RTC唤醒中断标志位

	RTC_ITConfig(RTC_IT_WUT,ENABLE);  // RTC中断配置
	RTC_WakeUpCmd( ENABLE);           // 开启wakeup


	EXTI_InitStructure.EXTI_Line=EXTI_Line22;   // 外部中断线22
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;   //上升沿触发
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn; // RTC_WKUP中断通道
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = RTC_WakeupIRQn_Priority; // 子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			// IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	// 根据指定的参数初始化VIC寄存器
}

// 闹钟中断(外部中断17)
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

	RTC_AlarmStructure.RTC_AlarmDateWeekDay = week; // 指定日期和星期
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay; // 闹铃为日期还是星期选择
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_None; // 闹钟时间掩码设置
	RTC_AlarmStructure.RTC_AlarmTime = RTC_TimeStructure; // 闹钟时间设定

	RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmStructure); // 设置闹钟

	EXTI_ClearITPendingBit(EXTI_Line17); // 开启中断前清除中断线，alarm挂接在17上
	RTC_ClearITPendingBit(RTC_IT_ALRA); // 开启RTC前清除alarm中断标志位    

	RTC_ITConfig(RTC_IT_ALRA,ENABLE); // 开启中断
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE); // 开启闹钟功能

	EXTI_InitStructure.EXTI_Line=EXTI_Line17; // 外部中断线22
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;   //上升沿触发
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn; // RTC_WKUP中断通道
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = RTC_IRQn_Priority; // 子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	// IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	// 根据指定的参数初始化VIC寄存器
}
