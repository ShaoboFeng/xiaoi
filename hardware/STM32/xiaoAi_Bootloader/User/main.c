#include "driver.h"
#include "common.h"

extern void jump(void);

int main(void)
{
    //Flash ����
    FLASH_Unlock();
		InitAllPeriph();
    if(1!=2)
		{
        SerialPutString("\r\n======================================================================");
        SerialPutString("\r\n=                                                                    =");
        SerialPutString("\r\n=     In-Application Programming Application  (Version 1.0.0)        =");
        SerialPutString("\r\n=                                                                    =");
        SerialPutString("\r\n======================================================================");
        SerialPutString("\r\n\r\n");
        Main_Menu ();
    }
    //����ִ���û�����
    else
    {	
			//execute app program
      jump();
    }

    while (1)
    {
    }
}
