#ifndef __DELAY_H
#define __DELAY_H 
#include "stm32f10x.h"

void DelayMs_init(void);
void DelayMs(u32 i);
void DelayUs(u32 nus);
#endif
