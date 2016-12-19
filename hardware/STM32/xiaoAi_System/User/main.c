#include "driver.h"
#include "delay.h"
#include "common.h"

extern void jump(void);

int main(void)
{
		InitAllPeriph();
	  while (1)
    {
				if(1!=2){
					SerialPutString("app is running\r\n");
					DelayMs(500);
				} else {
					//return bootloader
					if (((*(__IO uint32_t*)AppAddress) & 0x2FFE0000 ) == 0x20000000)
					{
							SerialPutString("Execute user Program\r\n\n");
							//跳转至用户代码
							jump();
					}
					else
					{
							SerialPutString("no user Program\r\n\n");
					}
				}
    }
}
