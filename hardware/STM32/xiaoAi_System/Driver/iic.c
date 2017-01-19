#include "iic.h"

//初始化IIC
void IIC_Init(void)
{		
  GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;  //传说OD开漏可是stm32IO可成双向
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;  //传说OD开漏可是stm32IO可成双向
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA_1;	  	  
	IIC_SCL_1;
	DelayUs(5);
 	IIC_SDA_0;//START:when CLK is high,DATA change form high to low 
	DelayUs(5);
	IIC_SCL_0;//钳住I2C总线，准备发送或接收数据 
}
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL_0;
	IIC_SDA_0;//STOP:when CLK is high DATA change form low to high
 	DelayUs(5);
	IIC_SCL_1; 
	IIC_SDA_1;//发送I2C总线结束信号
	DelayUs(5);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA_1;DelayUs(5);	   
	IIC_SCL_1;DelayUs(5);	 
	while(READ_SDA)
	{
		ucErrTime++;
		
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL_0;
	SDA_OUT();
	IIC_SDA_0;
	DelayUs(5);
	IIC_SCL_1;
	DelayUs(5);
	IIC_SCL_0;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL_0;
	SDA_OUT();
	IIC_SDA_1;
	DelayUs(5);
	IIC_SCL_1;
	DelayUs(5);
	IIC_SCL_0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		SDA_OUT(); 	    
    IIC_SCL_0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
				if(txd&0x80){
					IIC_SDA_1;
				}else{
					IIC_SDA_0;
				}
				txd<<=1; 
				DelayUs(5);   //对TEA5767这三个延时都是必须的
				IIC_SCL_1;
				DelayUs(5); 
				IIC_SCL_0;	
				DelayUs(5);
    }
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
  for(i=0;i<8;i++ )
	{
        IIC_SCL_0; 
        DelayUs(5);
				IIC_SCL_1;
        receive<<=1;
        if(READ_SDA)receive++;   
				DelayUs(5); 
   }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}

