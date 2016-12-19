#ifndef     _USART_H_
#define     _USART_H_ 
#include  "stm32f10x.h"  



//USART1Ïà¹Øº¯Êý

extern void COM1_SendData(unsigned char *p,unsigned int length);

extern void COM1_printf(unsigned char *s_buff);

#endif




