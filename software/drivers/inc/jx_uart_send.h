#ifndef JX_UART_SEND_H_
#define JX_UART_SEND_H_

#include "bsp_uart.h"

#ifdef __cplusplus
extern "C" {
#endif

// 串口发送消息最大长度
#define UART_SEND_MAX      32
#define UART_MSG_HEAD_LEN  2
#define UART_MSG_FOOT_LEN  2

// 串口发送消息号
#define U_MSG_PhotoOk      12
#define U_MSG_PhotoError      13

// 串口消息参数类型
typedef union {
  double d_double;
  int d_int;
  unsigned char d_ucs[8];
  char d_char;
  unsigned char d_uchar;
  unsigned long d_long;
  short d_short;
  float d_float;
}uart_param_t;

// 浮点数转双精度 
void _float_to_double(uart_param_t* param);
// 十六位整数转32位整数
void _int16_to_int32(uart_param_t* param);
// action: PhotoOk
void _uart_PhotoOk(void);

// action: PhotoError
void _uart_PhotoError(void);

#ifdef __cplusplus
}
#endif

#endif /*JX_UART_SEND_H_*/

