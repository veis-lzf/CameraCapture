#ifndef __RTC_H__
#define	__RTC_H__

#include "type.h"

// 初始化RTC
void bsp_InitRTC(void);
// 设置RTC时间，格式：xx-xx-xx xx:xx:xx
void RTC_Time_Set(int y, int m, int d, int wd, int hour, int min, int sec);
// 设置初始RTC时间，默认为：0:0:0
void RTC_TimeRegulate(void);
// 显示当前时间
void RTC_TimeShow(void);
// 显示闹钟时间
void RTC_AlarmShow(void);
// 获取时间戳，单位：s
uint32_t RTC_GetTraceTick(void);
// 唤醒中断函数，唤醒时间单位：s，范围：0~65535
void RTC_Set_WakeUp(uint16_t cnt);
// 闹钟中断(外部中断17)，当前只支持设置闹钟A
void RTC_Set_Alarm(uint8_t hour, uint8_t min, uint8_t sec, uint8_t ampm, uint8_t week);
// 获取字符串形式的时间
void RTC_GetTimeString(uint8_t * buf);
#endif
