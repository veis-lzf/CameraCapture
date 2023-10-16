#include "jx_uart_send.h"

// 串口通信消息头
const unsigned char g_uart_send_head[] = {
  0xaa, 0x55
};

// 串口通信消息尾
const unsigned char g_uart_send_foot[] = {
  0x55, 0xaa
};


// 串口发送函数实现
void _uart_send_impl(unsigned char* buff, int len) {
  // TODO: 调用项目实际的串口发送函数
	usart3_sendArray(buff, len);
}

// action: PhotoOk
void _uart_PhotoOk(void) {
	uart_param_t param;
	int i = 0;
	unsigned char Status = 0;
	unsigned char buff[UART_SEND_MAX] = {0};
	for (i = 0; i < UART_MSG_HEAD_LEN; i++) 
	{
		buff[i + 0] = g_uart_send_head[i];
	}
	buff[2] = U_MSG_PhotoOk;
	param.d_char = Status;
	buff[3] = param.d_uchar;
	for (i = 0; i < UART_MSG_FOOT_LEN; i++) 
	{
		buff[i + 4] = g_uart_send_foot[i];
	}
	_uart_send_impl(buff, 6);
}

// action: PhotoError
void _uart_PhotoError(void) {
	uart_param_t param;
	int i = 0;
	unsigned char Status = 0;
	unsigned char buff[UART_SEND_MAX] = {0};
	for (i = 0; i < UART_MSG_HEAD_LEN; i++) 
	{
		buff[i + 0] = g_uart_send_head[i];
	}
	buff[2] = U_MSG_PhotoError;
	param.d_char = Status;
	buff[3] = param.d_uchar;
	for (i = 0; i < UART_MSG_FOOT_LEN; i++) 
	{
		buff[i + 4] = g_uart_send_foot[i];
	}
	_uart_send_impl(buff, 6);
}

