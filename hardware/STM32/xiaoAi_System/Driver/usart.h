#ifndef     _USART_H_
#define     _USART_H_ 
#include  "stm32f10x.h"  

extern void SerialPutChars(uint8_t *p,unsigned int length);
extern void SerialPutChar(uint8_t c);
extern void SerialPutString(uint8_t *s);
#endif




