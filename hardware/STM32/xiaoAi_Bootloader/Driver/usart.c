#include  <string.h>
#include  "driver.h"



/************************************************************************
** Function Name : Usart1_SendData
** Description      : ����1����һ֡���ݺ���	 ���ж�ģʽ
** Input Parameters  :
                
** Output Parameters :

    note :                         
************************************************************************/
void COM1_SendData(unsigned char *p,unsigned int length)
{
	unsigned char i;
	for(i=0; i<length; i++)
	{
		USART_SendData( USART1,*p++);
        while( USART_GetFlagStatus( USART1, USART_FLAG_TXE ) == RESET )
        { ; }
	}
}
/************************************************************************
** Function Name : COM1_printf
** Description      : ����1PC�����Ժ���
** Input Parameters  :
                
** Output Parameters :

    note :                         
************************************************************************/
void COM1_printf(unsigned char *s_buff)
{
	unsigned char s_len=0;
  	s_len=strlen((const char *)s_buff);
	COM1_SendData(s_buff,s_len);	
}


/*---------------------------------------------------END--------------------------------------------*/



