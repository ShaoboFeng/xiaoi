#include "common.h"
#include "driver.h"

pFunction Jump_To_Application;  //the pointer of app function
uint32_t JumpAddress;
uint32_t BlockNbr = 0, UserMemoryMask = 0;
__IO uint32_t FlashProtection = 0;
extern uint32_t FlashDestination;

void jump(void)
{
	SerialPutString("Execute user Program\r\n");
	if (((*(volatile uint32_t*)AppAddress) & 0x2FFE0000 ) == 0x20000000)
	{
		JumpAddress = *(volatile uint32_t*)(AppAddress + 4);
		Jump_To_Application = (pFunction)JumpAddress;
		__set_MSP(*(volatile uint32_t*)AppAddress);    
		Jump_To_Application();
	}
}
