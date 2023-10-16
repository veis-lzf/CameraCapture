#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "bsp.h"
#include "app_cfg.h"

// 幅度范围
#define MAX_AMP		33
#define MIN_AMP		0
// 占空比范围
#define MIN_DUTY	1
#define MAX_DUTY	100
// 频率范围
#define MAX_FREQ	6500*10
// CPUID起始地址
#define CPU_ID              ((unsigned int*)0x1fff7a10)

// 协议头
#define HEARD_ID1		0xAA	// 协议头1
#define HEARD_ID2		0x55	// 协议头2

#define TAIL_ID1		0x55	// 协议尾1
#define TAIL_ID2		0xAA	// 协议尾2

// 协议标识
#define LED_CONTROL_CMD	0x01	// 开关灯控制
#define LED_LIGHT_CMD	0x02	// 档位控制
#define TAKE_PHOTO_CMD	0x03	// 拍照

// 数据域
#define LED_LEVEL1		0x01	// 一档
#define LED_LEVEL2		0x02	// 二档
#define	LED_LEVEL3		0x03	// 三挡
#define	LED_TURN_ON		0x01	// 开灯
#define	LED_TURN_OFF	0x00	// 关灯
#define	TAKE_PHOTO		0x01	// 拍照

enum eControlRet
{
	RET_ERROR,
	RET_OK,
};

// 解析串口协议
void Receive_Handle(uint8_t *buf, uint16_t len);

#endif	/* __PROTOCOL_H__ */
