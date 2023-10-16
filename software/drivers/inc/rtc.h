#ifndef __RTC_H__
#define	__RTC_H__

#include "type.h"

// ��ʼ��RTC
void bsp_InitRTC(void);
// ����RTCʱ�䣬��ʽ��xx-xx-xx xx:xx:xx
void RTC_Time_Set(int y, int m, int d, int wd, int hour, int min, int sec);
// ���ó�ʼRTCʱ�䣬Ĭ��Ϊ��0:0:0
void RTC_TimeRegulate(void);
// ��ʾ��ǰʱ��
void RTC_TimeShow(void);
// ��ʾ����ʱ��
void RTC_AlarmShow(void);
// ��ȡʱ�������λ��s
uint32_t RTC_GetTraceTick(void);
// �����жϺ���������ʱ�䵥λ��s����Χ��0~65535
void RTC_Set_WakeUp(uint16_t cnt);
// �����ж�(�ⲿ�ж�17)����ǰֻ֧����������A
void RTC_Set_Alarm(uint8_t hour, uint8_t min, uint8_t sec, uint8_t ampm, uint8_t week);
// ��ȡ�ַ�����ʽ��ʱ��
void RTC_GetTimeString(uint8_t * buf);
#endif
