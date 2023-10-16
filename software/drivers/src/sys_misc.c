#define DEBUG

#include "debug.h"
#include "drivers.h"
#include "app.h"
#include "api.h"

#include <absacc.h>

void delay_1us()
{
	int i = 50;
	while (i--);
}

void delay_us(uint32_t us)
{
	while (us--)
		delay_1us();
}
void delay_ms(uint32_t us)
{
	while (us--)
		delay_us(1000);
}

void assert_failed(uint8_t *file, uint32_t line)
{
	p_err("assert_failed in:%s,line:%d \n", file ? file : "n", line);
	while (1);
}

void misc_init()
{
	uart1_init(115200);
	uart3_init(9600);
	bsp_InitRTC();
	TIM2_PWM_Init(1000-1, 840-1); // 100Hz PWM
	LED_Init();
	bsp_DCMI_Init();
}

void soft_reset()
{
	p_err("system well reset\n");
	NVIC_SystemReset();
  	__DSB();  
	while (1);
}

void usr_gpio_init()
{
	// Camera power control pin
	gpio_cfg((uint32_t)CAMERA_PWR_PORT_GROUP, CAMERA_PWR_PIN, GPIO_Mode_Out_PP);
	gpio_set_level((uint32_t)CAMERA_PWR_PORT_GROUP, CAMERA_PWR_PIN, LOW_LEVEL);
	
	// Camera reset pin
	gpio_cfg((uint32_t)DCMI_RST_PORT_GROUP, DCMI_RST_PIN, GPIO_Mode_Out_PP);
	gpio_set_level((uint32_t)DCMI_RST_PORT_GROUP, DCMI_RST_PIN, LOW_LEVEL);
	
	// Camera power down mode control pin, active high
	gpio_cfg((uint32_t)DCMI_PWD_PORT_GROUP, DCMI_PWD_PIN, GPIO_Mode_Out_PP);
	gpio_set_level((uint32_t)DCMI_PWD_PORT_GROUP, DCMI_PWD_PIN, LOW_LEVEL);
	
	// TF Card power control pin
	gpio_cfg((uint32_t)VSD_POW_PORT_GROUP, VSD_POW_PIN, GPIO_Mode_Out_PP);
	gpio_set_level((uint32_t)VSD_POW_PORT_GROUP, VSD_POW_PIN, HIGH_LEVEL);
	// SD Detect pin
	gpio_cfg((uint32_t)SD_DET_PORT, SD_DET_PIN, GPIO_Mode_IPU);
	
	
	// voice module power control pin
	gpio_cfg((uint32_t)VOICE_POW_PORT_GROUP, VOICE_POW_PIN, GPIO_Mode_Out_PP);
	gpio_set_level((uint32_t)VOICE_POW_PORT_GROUP, VOICE_POW_PIN, HIGH_LEVEL);
	
	// camera flash led control pin
	gpio_cfg((uint32_t)STROBE_PORT_GROUP, STROBE_PIN, GPIO_Mode_Out_PP);
	gpio_set_level((uint32_t)STROBE_PORT_GROUP, STROBE_PIN, HIGH_LEVEL);
	
	
	gpio_cfg((uint32_t)LED_PWM_PORT_GROUP, LED_PWM_PIN, GPIO_Mode_Out_PP);
	gpio_set_level((uint32_t)LED_PWM_PORT_GROUP, LED_PWM_PIN, LOW_LEVEL);
	
	gpio_cfg((uint32_t)MAIN_LED_PWM_PORT_GROUP, MAIN_LED_PWM_PIN, GPIO_Mode_Out_PP);
	gpio_set_level((uint32_t)MAIN_LED_PWM_PORT_GROUP, MAIN_LED_PWM_PIN, LOW_LEVEL);
}

//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//关闭所有中断(但是不包括fault和NMI中断)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//开启所有中断
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}


