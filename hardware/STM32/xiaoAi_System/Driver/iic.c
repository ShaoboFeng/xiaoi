#include "iic.h"

//��ʼ��IIC
void IIC_Init(void)
{		
  GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;  //��˵OD��©����stm32IO�ɳ�˫��
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;  //��˵OD��©����stm32IO�ɳ�˫��
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA_1;	  	  
	IIC_SCL_1;
	DelayUs(5);
 	IIC_SDA_0;//START:when CLK is high,DATA change form high to low 
	DelayUs(5);
	IIC_SCL_0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL_0;
	IIC_SDA_0;//STOP:when CLK is high DATA change form low to high
 	DelayUs(5);
	IIC_SCL_1; 
	IIC_SDA_1;//����I2C���߽����ź�
	DelayUs(5);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	IIC_SCL_0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		SDA_OUT(); 	    
    IIC_SCL_0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
				if(txd&0x80){
					IIC_SDA_1;
				}else{
					IIC_SDA_0;
				}
				txd<<=1; 
				DelayUs(5);   //��TEA5767��������ʱ���Ǳ����
				IIC_SCL_1;
				DelayUs(5); 
				IIC_SCL_0;	
				DelayUs(5);
    }
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}

