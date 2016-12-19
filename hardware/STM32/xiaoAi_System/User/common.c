#include "common.h"
#include "driver.h"


pFunction Jump_To_Application;  //the pointer of app function
uint32_t JumpAddress;
uint32_t BlockNbr = 0, UserMemoryMask = 0;
__IO uint32_t FlashProtection = 0;
extern uint32_t FlashDestination;

/*******************************************************************************
  * @函数名称	SerialPutChar
  * @函数说明   串口发送一个字符
  * @输入参数   C:需发送的字符
  * @输出参数   i无
  * @返回参数   无
*******************************************************************************/
void SerialPutChar(uint8_t c)
{
    USART_SendData(USART1, c);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    {
    }
}

/*******************************************************************************
  * @函数名称	SerialPutChar
  * @函数说明   串口发送一个字符串
  * @输入参数   *s:需发送的字符串
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void Serial_PutString(uint8_t *s)
{
    while (*s != '\0')
    {
        SerialPutChar(*s);
        s++;
    }
}

void jump(void)
{
	COM1_printf("Execute user Program\r\n");
	if (((*(volatile uint32_t*)AppAddress) & 0x2FFE0000 ) == 0x20000000)
	{
		JumpAddress = *(volatile uint32_t*)(AppAddress + 4);
		Jump_To_Application = (pFunction)JumpAddress;
		
		__set_MSP(*(volatile uint32_t*)AppAddress);    //初始化用户程序的堆栈指针 
		
		Jump_To_Application();
	}
}
