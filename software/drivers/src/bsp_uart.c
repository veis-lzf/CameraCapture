#include "bsp_uart.h"
#include "app_cfg.h"

uint16 ack_len = 0;
uint8_t ack_buffer[ACK_BUFFER_LENGTH] = {0}; // Ӧ�����ݻ���
extern uint8_t return_val;
static void uart1_nvic_init(void);
static void uart3_nvic_init(void);
/**
 * @name	  uart1_init
 * @brief     ��ʼ������1��ʹ���շ�
 * @param	  bound��������
 * @return    ��
 * @DateTime  2019-7-20
 */
void uart1_init(uint32_t bound)
{
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 		// ʹ��USART1ʱ��

	// ����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(UART1_TX_PORT_GROUP, UART1_TX_PIN_SOURSE, GPIO_AF_USART1);  // GPIOA0����ΪUSART1
	GPIO_PinAFConfig(UART1_RX_PORT_GROUP, UART1_RX_PIN_SOURSE, GPIO_AF_USART1);  // GPIOA1����ΪUSART1

	// USART1�˿�����
	// PA9 ---> TXD
	// PA10 ---> RXD
	gpio_cfg((uint32_t)UART1_TX_PORT_GROUP, UART1_TX_PIN, GPIO_Mode_AF_PP);
	gpio_cfg((uint32_t)UART1_RX_PORT_GROUP, UART1_RX_PIN, GPIO_Mode_AF_IF);

	// USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;										// ����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						// �ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							// һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								// ����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// ��Ӳ������������	
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// �շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); 										// ��ʼ������4
	
	USART_Cmd(USART1, ENABLE);  // ʹ�ܴ���x
	
	uart1_nvic_init();		   // ʹ�ܽ����ж�
}

/**
 * @name	  uart1_init
 * @brief     ��ʼ������3��ʹ���շ�
 * @param	  bound��������
 * @return    ��
 * @DateTime  2019-7-20
 */
void uart3_init(uint32_t bound)
{
	USART_InitTypeDef USART_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 		// ʹ��USART3ʱ��

	// ����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(UART3_TX_PORT_GROUP, UART3_TX_PIN_SOURSE, GPIO_AF_USART3);  // GPIOA0����ΪUSART3
	GPIO_PinAFConfig(UART3_RX_PORT_GROUP, UART3_RX_PIN_SOURSE, GPIO_AF_USART3);  // GPIOA1����ΪUSART3

	// USART1�˿�����
	// PD8 ---> TXD
	// PD9 ---> RXD
	gpio_cfg((uint32_t)UART3_TX_PORT_GROUP, UART3_TX_PIN, GPIO_Mode_AF_PP);
	gpio_cfg((uint32_t)UART3_RX_PORT_GROUP, UART3_RX_PIN, GPIO_Mode_AF_IF);

	// USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;										// ����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						// �ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							// һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								// ����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// ��Ӳ������������	
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// �շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); 										// ��ʼ������4
	
	USART_Cmd(USART3, ENABLE);  // ʹ�ܴ���x
	
	uart3_nvic_init();		   // ʹ�ܽ����ж�
}

/**
 * @name	  uart1_nvic_init
 * @brief     ����1�ж����ȼ�����
 * @param	  ��
 * @return    ��
 * @DateTime  2019-7-20
 */
void uart1_nvic_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// ʹ�ܴ���x�����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);				// ��������ж�

	// Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			// ����x�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART1_IRQn_Priority;			// �����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				// IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);								// ����ָ���Ĳ�����ʼ��NVIC�Ĵ�����
}

/**
 * @name	  uart3_nvic_init
 * @brief     ����3�ж����ȼ�����
 * @param	  ��
 * @return    ��
 * @DateTime  2019-7-20
 */
void uart3_nvic_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// ʹ�ܴ���x�����ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);				// ��������ж�
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	
	// Usart3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			// ����x�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART3_IRQn_Priority;			// �����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				// IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);								// ����ָ���Ĳ�����ʼ��NVIC�Ĵ�����
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
