#ifndef __DELAY_H
#define __DELAY_H 

#define u8  unsigned char
#define u16 unsigned short int
#define u32 unsigned int

void DelayMs_init(void);
void DelayMs(u32 i);
void DelayUs(u32 nus);
#endif
