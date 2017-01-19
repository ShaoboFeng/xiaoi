#ifndef __I2C_H
#define __I2C_H
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "delay.h"
   	   		   
//IO��������
#define SDA_IN()  {}//{GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
#define SDA_OUT() {}//{GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}
//IO��������	 
#define IIC_SCL_1    GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_SET)			//		PBout(10) //SCL
#define IIC_SCL_0    GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_RESET)			//		PBout(10) //SCL
#define IIC_SDA_1    GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_SET)			//PBout(11) //SDA	 
#define IIC_SDA_0    GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_RESET)		//PBout(11) //SDA	 
#define READ_SDA   	 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)			//PBin(11)  //����SDA 
//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�
void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif

