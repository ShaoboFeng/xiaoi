#include  <string.h>
#include  "driver.h"



/************************************************************************
** Function Name : Usart1_SendData
** Description   : comm 1 send a len byte            
************************************************************************/
void SerialPutChars(uint8_t *p,unsigned int length)
{
	unsigned char i;
	for(i=0; i<length; i++)
	{		
		SerialPutChar(*p);
		p++;
	}
}
/*******************************************************************************
* sand a char to comm
*******************************************************************************/
void SerialPutChar(uint8_t c)
{
    USART_SendData(USART1, c);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    {}
}

/*******************************************************************************
* send a string to comm
*******************************************************************************/
void SerialPutString(uint8_t *s)
{
    while (*s != '\0')
    {
        SerialPutChar(*s);
        s++;
    }
}
