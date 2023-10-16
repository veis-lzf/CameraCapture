#include "protocol.h"
#include "bsp_uart.h"
#include "debug.h"
#include "bsp_led.h"
#include "photo.h"
#include "jx_uart_send.h"


#if 0
static uint8_t checksum(uint8_t *arry, uint32_t len)
{
	uint8_t cks = arry[0];
	uint32_t i;
	for(i = 1; i < len; i++)
	{
		cks ^= arry[i];
	}
	return cks;
}

/*
��ȡһ��96λ��CpuID�������鱣��;
��ȫ��(32λ)��ȡ;
����ַ��0x1FFF7A10;
*/
static void GetSTM32_CpuID(uint32_t* CpuID)
{
	//��ȡCPUΨһID
	CpuID[0] = *(uint32_t*)(0x1FFF7A10);
	CpuID[1] = *(uint32_t*)(0x1FFF7A14);
	CpuID[2] = *(uint32_t*)(0x1FFF7A18);
}
#endif

// ��������Э��
void Receive_Handle(uint8_t *buf, uint16_t len)
{
	if(len <= 4) return;
	
	if(buf[0] == HEARD_ID1 && buf[1] == HEARD_ID2)
	{
		if(buf[len-2] == TAIL_ID1 && buf[len-1] == TAIL_ID2)
		{
			switch(buf[2])
			{
				case LED_CONTROL_CMD:
				{
					switch(buf[3])
					{
						case LED_TURN_OFF: // �ص�
						{
							Set_LED_Status(LED1,LED_OFF);
							Set_LED_Status(LED2,LED_OFF);
						}
						break;
						
						case LED_TURN_ON: // ����
						{
							Set_LED_Status(LED1,LED_ON); 
						}
						break;
					}
				}
				break;
				
				case LED_LIGHT_CMD: // ����ָ��
				{
					switch(buf[3])
					{
						case LED_LEVEL1: // 25%
						{
							Set_LED_Status(LED1,LED_MODE1_ON); 
						}
						break;
						
						case LED_LEVEL2: // 50%
						{
							Set_LED_Status(LED1,LED_MODE2_ON); 
						}
						break;
						
						case LED_LEVEL3: // 100%
						{
							Set_LED_Status(LED1,LED_MODE3_ON);
							Set_LED_Status(LED2,LED_ON); 
						}
						break;
					}
				}
				break;
				
				case TAKE_PHOTO_CMD: // ����
				{
					switch(buf[3])
					{
						case TAKE_PHOTO: // 0x01
						{
							if(SDCARD_Status == SD_CARD_STATUS_OK)	// SD������
							{  
								camera_new_pathname(pname, 1);//�õ��ļ���
								p_info("name:%s", pname);
								if(take_jpg_photo(pname))
								{
									p_info("take photo failed!");
									_uart_PhotoError();
								}
								else
								{
									p_info("take photo successful!");
									_uart_PhotoOk();
								}        
							}
						}
						break;
					}
				}
				break;
			}
		}
		else
		{
			p_err("invalid tail:0x%x,0x%x", buf[len-2], buf[len-1]);
		}
		
	}
	else
	{
		//AT_DeviceHandle(RX_BUFFER); // ����AT��������ӿں���
	}
}
