#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "bsp.h"

#define ACK_BUFFER_LENGTH	15
extern uint8_t ack_buffer[ACK_BUFFER_LENGTH];
extern uint16 ack_len;

// 外部调用函数
void uart1_init(uint32_t bound);
void uart3_init(uint32_t bound);
void usart1_sendArray(uint8_t *str,uint32_t nSize);
void usart1_sendString(uint8_t *str);
void usart1_sendByte(uint8_t ch);
void usart3_sendArray(uint8_t *str,uint32_t nSize);
void usart3_sendString(uint8_t *str);
void usart3_sendByte(uint8_t ch);
#endif /* __BSP_UART4_H */
