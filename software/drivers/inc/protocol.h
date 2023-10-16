#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "bsp.h"
#include "app_cfg.h"

// ���ȷ�Χ
#define MAX_AMP		33
#define MIN_AMP		0
// ռ�ձȷ�Χ
#define MIN_DUTY	1
#define MAX_DUTY	100
// Ƶ�ʷ�Χ
#define MAX_FREQ	6500*10
// CPUID��ʼ��ַ
#define CPU_ID              ((unsigned int*)0x1fff7a10)

// Э��ͷ
#define HEARD_ID1		0xAA	// Э��ͷ1
#define HEARD_ID2		0x55	// Э��ͷ2

#define TAIL_ID1		0x55	// Э��β1
#define TAIL_ID2		0xAA	// Э��β2

// Э���ʶ
#define LED_CONTROL_CMD	0x01	// ���صƿ���
#define LED_LIGHT_CMD	0x02	// ��λ����
#define TAKE_PHOTO_CMD	0x03	// ����

// ������
#define LED_LEVEL1		0x01	// һ��
#define LED_LEVEL2		0x02	// ����
#define	LED_LEVEL3		0x03	// ����
#define	LED_TURN_ON		0x01	// ����
#define	LED_TURN_OFF	0x00	// �ص�
#define	TAKE_PHOTO		0x01	// ����

enum eControlRet
{
	RET_ERROR,
	RET_OK,
};

// ��������Э��
void Receive_Handle(uint8_t *buf, uint16_t len);

#endif	/* __PROTOCOL_H__ */
