#include "bsp_sccb.h"
#include "sys_misc.h"

void SCCB_Init(void)
{
	gpio_cfg((uint32_t)SIO_C_PORT_GROUP, SIO_C_PIN, GPIO_Mode_Out_PP);
	gpio_cfg((uint32_t)SIO_D_PORT_GROUP, SIO_D_PIN, GPIO_Mode_Out_PP);
}

//SCCB��ʼ�ź�
//��ʱ��Ϊ�ߵ�ʱ��,�����ߵĸߵ���,ΪSCCB��ʼ�ź�
//�ڼ���״̬��,SDA��SCL��Ϊ�͵�ƽ
void SCCB_Start(void)
{
    GPIO_SET(SIO_D_PORT_GROUP, SIO_D_PIN);     //�����߸ߵ�ƽ	   
    GPIO_SET(SIO_C_PORT_GROUP, SIO_C_PIN);	    //��ʱ���߸ߵ�ʱ���������ɸ�����
    delay_us(50);  
    GPIO_CLR(SIO_D_PORT_GROUP, SIO_D_PIN);
    delay_us(50);	 
    GPIO_CLR(SIO_C_PORT_GROUP, SIO_C_PIN);	    //�����߻ָ��͵�ƽ��������������Ҫ	  
}

//SCCBֹͣ�ź�
//��ʱ��Ϊ�ߵ�ʱ��,�����ߵĵ͵���,ΪSCCBֹͣ�ź�
//����״����,SDA,SCL��Ϊ�ߵ�ƽ
void SCCB_Stop(void)
{
    GPIO_CLR(SIO_D_PORT_GROUP, SIO_D_PIN);
    delay_us(50);	 
    GPIO_SET(SIO_C_PORT_GROUP, SIO_C_PIN);	
    delay_us(50); 
    GPIO_SET(SIO_D_PORT_GROUP, SIO_D_PIN);	
    delay_us(50);
}  
//����NA�ź�
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
//SCCB,д��һ���ֽ�
//����ֵ:0,�ɹ�;1,ʧ��. 
uint8_t SCCB_WR_Byte(uint8_t dat)
{
	uint8_t j,res;	 
	for(j=0;j<8;j++) //ѭ��8�η�������
	{
		if(dat&0x80)GPIO_SET(SIO_D_PORT_GROUP, SIO_D_PIN);	
		else GPIO_CLR(SIO_D_PORT_GROUP, SIO_D_PIN);
		dat<<=1;
		delay_us(50);
		GPIO_SET(SIO_C_PORT_GROUP, SIO_C_PIN);	
		delay_us(50);
		GPIO_CLR(SIO_C_PORT_GROUP, SIO_C_PIN);		   
	}			 
	SIO_D_IN;		//����SDAΪ���� 
	delay_us(50);
	GPIO_SET(SIO_C_PORT_GROUP, SIO_C_PIN);			//���յھ�λ,���ж��Ƿ��ͳɹ�
	delay_us(50);
	if(GPIO_STAT(SIO_D_PORT_GROUP, SIO_D_PIN))res=1;  //SDA=1����ʧ�ܣ�����1
	else res=0;         //SDA=0���ͳɹ�������0
	GPIO_CLR(SIO_C_PORT_GROUP, SIO_C_PIN);		 
	SIO_D_OUT;		//����SDAΪ���    
	return res;  
}	 
//SCCB ��ȡһ���ֽ�
//��SCL��������,��������
//����ֵ:����������
uint8_t SCCB_RD_Byte(void)
{
	uint8_t temp=0,j;    
	SIO_D_IN;		//����SDAΪ����  
	for(j=8;j>0;j--) 	//ѭ��8�ν�������
	{		     	  
		delay_us(50);
		GPIO_SET(SIO_C_PORT_GROUP, SIO_C_PIN);
		temp=temp<<1;
		if(GPIO_STAT(SIO_D_PORT_GROUP, SIO_D_PIN))temp++;   
		delay_us(50);
		GPIO_CLR(SIO_C_PORT_GROUP, SIO_C_PIN);
	}	
	SIO_D_OUT;		//����SDAΪ���    
	return temp;
} 							    
//д�Ĵ���
//����ֵ:0,�ɹ�;1,ʧ��.
uint8_t SCCB_WR_Reg(uint8_t reg,uint8_t data)
{
	uint8_t res=0;
	SCCB_Start(); 					//����SCCB����
	if(SCCB_WR_Byte(SCCB_ID))res=1;	//д����ID	  
	delay_us(100);
	if(SCCB_WR_Byte(reg))res=1;		//д�Ĵ�����ַ	  
	delay_us(100);
	if(SCCB_WR_Byte(data))res=1; 	//д����	 
	SCCB_Stop();	  
	return	res;
}		  					    
//���Ĵ���
//����ֵ:�����ļĴ���ֵ
uint8_t SCCB_RD_Reg(uint8_t reg)
{
	uint8_t val=0;
	SCCB_Start(); 				//����SCCB����
	SCCB_WR_Byte(SCCB_ID);		//д����ID	  
	delay_us(100);	 
	SCCB_WR_Byte(reg);			//д�Ĵ�����ַ	  
	delay_us(100);	  
	SCCB_Stop();   
	delay_us(100);	   
	//���üĴ�����ַ�󣬲��Ƕ�
	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID|0X01);	//���Ͷ�����	  
	delay_us(100);
	val=SCCB_RD_Byte();		 	//��ȡ����
	SCCB_No_Ack();
	SCCB_Stop();
	return val;
}



