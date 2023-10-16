#include "WinMain.h"
#include "debug.h"

#define VERSION	"V1.0.1"

uint8_t frame_err = 0;

void dcmi_rx_callback(void)
{
	uint32_t i;
	uint32_t *pbuf;
	pbuf = jpeg_data_buf + jpeg_data_len;//偏移到有效数据末尾
	if(DMA2_Stream1->CR&(1<<19))//buf0已满,正常处理buf1
	{ 
		for(i=0;i<jpeg_dma_bufsize;i++)pbuf[i] = jpeg_buf0[i];//读取buf0里面的数据
		jpeg_data_len += jpeg_dma_bufsize;//偏移
	}else //buf1已满,正常处理buf0
	{
		for(i=0;i<jpeg_dma_bufsize;i++)pbuf[i] = jpeg_buf1[i];//读取buf1里面的数据
		jpeg_data_len += jpeg_dma_bufsize;//偏移 
	} 	
}

void Capture_CpltCallback(void)
{
	uint32_t i;
	uint32_t rlen; // 剩余数据长度
	uint32_t *pbuf;
	if(jpeg_data_ok == CAPTURE_BUSY) // jpeg数据还未采集完?
    {	
		DMA_Cmd(DMA2_Stream1,DISABLE);		// 停止当前传输
		while(DMA_GetCmdStatus(DMA2_Stream1) != DISABLE);	// 等待DMA2_Stream1可配置 
		rlen=jpeg_dma_bufsize-DMA_GetCurrDataCounter(DMA2_Stream1); // 得到剩余数据长度	
		pbuf=jpeg_data_buf+jpeg_data_len; // 偏移到有效数据末尾,继续添加
		if(DMA2_Stream1->CR&(1<<19))
			for(i=0;i<rlen;i++)pbuf[i]=jpeg_buf1[i];  // 读取buf1里面的剩余数据
		else for(i=0;i<rlen;i++)pbuf[i]=jpeg_buf0[i]; // 读取buf0里面的剩余数据 
		jpeg_data_len+=rlen;			// 加上剩余长度
		jpeg_data_ok = CAPTURE_COMPLETE; // 标记JPEG数据采集完按成,等待其他函数处理
    }
    if(jpeg_data_ok == PROCESS_COMPLETE) // 上一次的jpeg数据已经被处理了
    {
		DMA_SetCurrDataCounter(DMA2_Stream1,jpeg_dma_bufsize); // 传输长度为jpeg_buf_size*4字节
		DMA_Cmd(DMA2_Stream1,ENABLE); // 重新传输
		jpeg_data_ok = CAPTURE_BUSY;  // 标记数据未采集
		jpeg_data_len = 0;			  // 数据重新开始
	}
}

static void init(void *param)
{
	RCC_ClocksTypeDef RCC_ClocksStatus;

	driver_misc_init();	  // 初始化系统时基/GPIO时钟/NVIC分组
	usr_gpio_init(); 	  // 初始化外设端口	
	my_mem_init(SRAMIN);  // 初始化内部内存池 
	my_mem_init(SRAMCCM); // 初始化CCM内存池
	misc_init(); // 初始化外设驱动
	
	jpeg_buf0 = mymalloc(SRAMIN,jpeg_dma_bufsize*4);	//为jpeg dma接收申请内存	
	jpeg_buf1 = mymalloc(SRAMIN,jpeg_dma_bufsize*4);	//为jpeg dma接收申请内存	
	jpeg_data_buf = mymalloc(SRAMIN, 30*1024);		//为jpeg文件申请内存(最大300KB)
	pname = mymalloc(SRAMIN,30); // 为带路径的文件名分配30个字节的内存
	
	// 打印复位原因
//	check_rst_stat();
	// 打印系统频率
	RCC_GetClocksFreq(&RCC_ClocksStatus);
	p_dbg(
		"\r\nVERSION:%s\r\n"
		"SYSCLK_Frequency:%d\r\n"
		"HCLK_Frequency:%d\r\n"
		"PCLK1_Frequency:%d\r\n"
		"PCLK2_Frequency:%d\r\n",
		VERSION,
		RCC_ClocksStatus.SYSCLK_Frequency, 
		RCC_ClocksStatus.HCLK_Frequency, 
		RCC_ClocksStatus.PCLK1_Frequency, 
		RCC_ClocksStatus.PCLK2_Frequency);
		
	Init_SDCARD();
	
	uint8_t timeout = 0;
	while(OV2640_Init() && timeout < 10)
	{
		timeout++;
		gpio_set_level((uint32_t)CAMERA_PWR_PORT_GROUP, CAMERA_PWR_PIN, LOW_LEVEL);
		delay_ms(200);
		gpio_set_level((uint32_t)CAMERA_PWR_PORT_GROUP, CAMERA_PWR_PIN, HIGH_LEVEL);
		delay_ms(200);
	}
	if(timeout == 10)
	{
		p_err("ov2640 init failed!");
		soft_reset();
	}
}

//#define JPEG_UART_TX_TEST
#ifdef JPEG_UART_TX_TEST
// JPEG测试
// JPEG数据,通过串口1发送给电脑.
void jpeg_test(void)
{	
	uint32_t i, jpeg_start, jpeg_len, jpeg_ok;; 
	uint8_t *p;
	
	DCMI_DMA_Init((u32)jpeg_buf0,(u32)jpeg_buf1,jpeg_dma_bufsize,DMA_MemoryDataSize_Word,DMA_MemoryInc_Enable);//DCMI DMA配置(双缓冲模式)
	Init_JPEG(eType_QVGA);
	OV2640_JPEG_Mode();		//切换为JPEG模式 
	DCMI_Start(); 			//启动传输 
	while(1)
	{
		IWDG_Feed();
		if(jpeg_data_ok==1)	// 已经采集完一帧图像了
		{  
			p=(uint8_t*)jpeg_data_buf;
			for(i=0;i<jpeg_data_len*4;i++)//查找0XFF,0XD8
			{
				//printf("%x ", pbuf[i]);
				if((p[i]==0XFF)&&(p[i+1]==0XD8))
				{
					jpeg_start = i;
				}
				if((p[i]==0XFF)&&(p[i+1]==0XD9))
				{
					jpeg_len = i - jpeg_start + 2;
					jpeg_ok = 1;
					break;
				}
			}
			
			if(jpeg_ok)
			{
				p += jpeg_start;
				for(i=0;i<jpeg_len;i++)		                            // dma传输1次等于4字节,所以乘以4.
				{
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);	// 循环发送,直到发送完毕  		
					USART_SendData(USART1,p[i]); 
					IWDG_Feed();
				}
			}
			else
			{
				frame_err++;
			}
			jpeg_data_ok=2;
		} 
    }
} 
#endif

uint8_t run_status = 0;
static void run(void *param)
{
	DCMI_DMA_Init((u32)jpeg_buf0,(u32)jpeg_buf1,jpeg_dma_bufsize,DMA_MemoryDataSize_Word,DMA_MemoryInc_Enable);//DCMI DMA配置(双缓冲模式)
	Init_JPEG(eType_QVGA);
	OV2640_JPEG_Mode();		//切换为JPEG模式 
	DCMI_Start(); 			//启动传输
	
	WDT_init(IWDG_Prescaler_256, 500); // 预分频数为64,重载值为500,溢出时间为1s
	
	while(1)
	{
		IWDG_Feed();
		delay_ms(1*1000);
	#ifdef JPEG_UART_TX_TEST
		jpeg_test();
	#endif
	}
}

WinMainStruct mainTask =
{
	init,
	run,
};
