#include "bsp_uart.h"
#include "app_cfg.h"

uint16 ack_len = 0;
uint8_t ack_buffer[ACK_BUFFER_LENGTH] = {0}; // 应答数据缓存
extern uint8_t return_val;
static void uart1_nvic_init(void);
static void uart3_nvic_init(void);
/**
 * @name	  uart1_init
 * @brief     初始化串口1，使能收发
 * @param	  bound：波特率
 * @return    空
 * @DateTime  2019-7-20
 */
void uart1_init(uint32_t bound)
{
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 		// 使能USART1时钟

	// 串口1对应引脚复用映射
	GPIO_PinAFConfig(UART1_TX_PORT_GROUP, UART1_TX_PIN_SOURSE, GPIO_AF_USART1);  // GPIOA0复用为USART1
	GPIO_PinAFConfig(UART1_RX_PORT_GROUP, UART1_RX_PIN_SOURSE, GPIO_AF_USART1);  // GPIOA1复用为USART1

	// USART1端口配置
	// PA9 ---> TXD
	// PA10 ---> RXD
	gpio_cfg((uint32_t)UART1_TX_PORT_GROUP, UART1_TX_PIN, GPIO_Mode_AF_PP);
	gpio_cfg((uint32_t)UART1_RX_PORT_GROUP, UART1_RX_PIN, GPIO_Mode_AF_IF);

	// USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;										// 波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						// 字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							// 一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 无硬件数据流控制	
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// 收发模式
	USART_Init(USART1, &USART_InitStructure); 										// 初始化串口4
	
	USART_Cmd(USART1, ENABLE);  // 使能串口x
	
	uart1_nvic_init();		   // 使能接收中断
}

/**
 * @name	  uart1_init
 * @brief     初始化串口3，使能收发
 * @param	  bound：波特率
 * @return    空
 * @DateTime  2019-7-20
 */
void uart3_init(uint32_t bound)
{
	USART_InitTypeDef USART_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 		// 使能USART3时钟

	// 串口1对应引脚复用映射
	GPIO_PinAFConfig(UART3_TX_PORT_GROUP, UART3_TX_PIN_SOURSE, GPIO_AF_USART3);  // GPIOA0复用为USART3
	GPIO_PinAFConfig(UART3_RX_PORT_GROUP, UART3_RX_PIN_SOURSE, GPIO_AF_USART3);  // GPIOA1复用为USART3

	// USART1端口配置
	// PD8 ---> TXD
	// PD9 ---> RXD
	gpio_cfg((uint32_t)UART3_TX_PORT_GROUP, UART3_TX_PIN, GPIO_Mode_AF_PP);
	gpio_cfg((uint32_t)UART3_RX_PORT_GROUP, UART3_RX_PIN, GPIO_Mode_AF_IF);

	// USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;										// 波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						// 字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							// 一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 无硬件数据流控制	
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// 收发模式
	USART_Init(USART3, &USART_InitStructure); 										// 初始化串口4
	
	USART_Cmd(USART3, ENABLE);  // 使能串口x
	
	uart3_nvic_init();		   // 使能接收中断
}

/**
 * @name	  uart1_nvic_init
 * @brief     串口1中断优先级设置
 * @param	  空
 * @return    空
 * @DateTime  2019-7-20
 */
void uart1_nvic_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// 使能串口x接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);				// 开启相关中断

	// Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			// 串口x中断通道
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART1_IRQn_Priority;			// 子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				// IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);								// 根据指定的参数初始化NVIC寄存器、
}

/**
 * @name	  uart3_nvic_init
 * @brief     串口3中断优先级设置
 * @param	  空
 * @return    空
 * @DateTime  2019-7-20
 */
void uart3_nvic_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// 使能串口x接收中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);				// 开启相关中断
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	
	// Usart3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			// 串口x中断通道
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART3_IRQn_Priority;			// 子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				// IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);								// 根据指定的参数初始化NVIC寄存器、
}

void usart1_sendArray(uint8_t *str,uint32_t nSize)
{
	uint32_t i = 0;
	do
	{
		usart1_sendByte(str[i]);
		i++;
	}while(i < nSize);
}

void usart1_sendString(uint8_t *str)
{
	while(*str != '\0')
	{
		usart1_sendByte(*str);
		str++;
	}
}

void usart1_sendByte(uint8_t ch)
{
	while((USART1->SR&0X40)==0);
	USART1->DR = ch;
}


void usart3_sendArray(uint8_t *str,uint32_t nSize)
{
	uint32_t i = 0;
	do
	{
		usart3_sendByte(str[i]);
		i++;
	}while(i < nSize);
}

void usart3_sendString(uint8_t *str)
{
	while(*str != '\0')
	{
		usart3_sendByte(*str);
		str++;
	}
}

void usart3_sendByte(uint8_t ch)
{
	while((USART3->SR&0X40)==0);
	USART3->DR = ch;
}
