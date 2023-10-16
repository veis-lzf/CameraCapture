#include "bsp_sccb.h"
#include "sys_misc.h"

void SCCB_Init(void)
{
	gpio_cfg((uint32_t)SIO_C_PORT_GROUP, SIO_C_PIN, GPIO_Mode_Out_PP);
	gpio_cfg((uint32_t)SIO_D_PORT_GROUP, SIO_D_PIN, GPIO_Mode_Out_PP);
}

//SCCB起始信号
//当时钟为高的时候,数据线的高到低,为SCCB起始信号
//在激活状态下,SDA和SCL均为低电平
void SCCB_Start(void)
{
    GPIO_SET(SIO_D_PORT_GROUP, SIO_D_PIN);     //数据线高电平	   
    GPIO_SET(SIO_C_PORT_GROUP, SIO_C_PIN);	    //在时钟线高的时候数据线由高至低
    delay_us(50);  
    GPIO_CLR(SIO_D_PORT_GROUP, SIO_D_PIN);
    delay_us(50);	 
    GPIO_CLR(SIO_C_PORT_GROUP, SIO_C_PIN);	    //数据线恢复低电平，单操作函数必要	  
}

//SCCB停止信号
//当时钟为高的时候,数据线的低到高,为SCCB停止信号
//空闲状况下,SDA,SCL均为高电平
void SCCB_Stop(void)
{
    GPIO_CLR(SIO_D_PORT_GROUP, SIO_D_PIN);
    delay_us(50);	 
    GPIO_SET(SIO_C_PORT_GROUP, SIO_C_PIN);	
    delay_us(50); 
    GPIO_SET(SIO_D_PORT_GROUP, SIO_D_PIN);	
    delay_us(50);
}  
//产生NA信号
void SCCB_No_Ack(void)
{
	delay_us(50);
	GPIO_SET(SIO_D_PORT_GROUP, SIO_D_PIN);	
	GPIO_SET(SIO_C_PORT_GROUP, SIO_C_PIN);	
	delay_us(50);
	GPIO_CLR(SIO_C_PORT_GROUP, SIO_C_PIN);	
	delay_us(50);
	GPIO_CLR(SIO_D_PORT_GROUP, SIO_D_PIN);	
	delay_us(50);
}
//SCCB,写入一个字节
//返回值:0,成功;1,失败. 
uint8_t SCCB_WR_Byte(uint8_t dat)
{
	uint8_t j,res;	 
	for(j=0;j<8;j++) //循环8次发送数据
	{
		if(dat&0x80)GPIO_SET(SIO_D_PORT_GROUP, SIO_D_PIN);	
		else GPIO_CLR(SIO_D_PORT_GROUP, SIO_D_PIN);
		dat<<=1;
		delay_us(50);
		GPIO_SET(SIO_C_PORT_GROUP, SIO_C_PIN);	
		delay_us(50);
		GPIO_CLR(SIO_C_PORT_GROUP, SIO_C_PIN);		   
	}			 
	SIO_D_IN;		//设置SDA为输入 
	delay_us(50);
	GPIO_SET(SIO_C_PORT_GROUP, SIO_C_PIN);			//接收第九位,以判断是否发送成功
	delay_us(50);
	if(GPIO_STAT(SIO_D_PORT_GROUP, SIO_D_PIN))res=1;  //SDA=1发送失败，返回1
	else res=0;         //SDA=0发送成功，返回0
	GPIO_CLR(SIO_C_PORT_GROUP, SIO_C_PIN);		 
	SIO_D_OUT;		//设置SDA为输出    
	return res;  
}	 
//SCCB 读取一个字节
//在SCL的上升沿,数据锁存
//返回值:读到的数据
uint8_t SCCB_RD_Byte(void)
{
	uint8_t temp=0,j;    
	SIO_D_IN;		//设置SDA为输入  
	for(j=8;j>0;j--) 	//循环8次接收数据
	{		     	  
		delay_us(50);
		GPIO_SET(SIO_C_PORT_GROUP, SIO_C_PIN);
		temp=temp<<1;
		if(GPIO_STAT(SIO_D_PORT_GROUP, SIO_D_PIN))temp++;   
		delay_us(50);
		GPIO_CLR(SIO_C_PORT_GROUP, SIO_C_PIN);
	}	
	SIO_D_OUT;		//设置SDA为输出    
	return temp;
} 							    
//写寄存器
//返回值:0,成功;1,失败.
uint8_t SCCB_WR_Reg(uint8_t reg,uint8_t data)
{
	uint8_t res=0;
	SCCB_Start(); 					//启动SCCB传输
	if(SCCB_WR_Byte(SCCB_ID))res=1;	//写器件ID	  
	delay_us(100);
	if(SCCB_WR_Byte(reg))res=1;		//写寄存器地址	  
	delay_us(100);
	if(SCCB_WR_Byte(data))res=1; 	//写数据	 
	SCCB_Stop();	  
	return	res;
}		  					    
//读寄存器
//返回值:读到的寄存器值
uint8_t SCCB_RD_Reg(uint8_t reg)
{
	uint8_t val=0;
	SCCB_Start(); 				//启动SCCB传输
	SCCB_WR_Byte(SCCB_ID);		//写器件ID	  
	delay_us(100);	 
	SCCB_WR_Byte(reg);			//写寄存器地址	  
	delay_us(100);	  
	SCCB_Stop();   
	delay_us(100);	   
	//设置寄存器地址后，才是读
	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID|0X01);	//发送读命令	  
	delay_us(100);
	val=SCCB_RD_Byte();		 	//读取数据
	SCCB_No_Ack();
	SCCB_Stop();
	return val;
}



