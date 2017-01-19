#include "driver.h"
#include "delay.h"
#include "common.h"
#include "stdio.h"

extern void jump(void);

void sendTempture(int temp){
	u8 retry = 2;
	char *data = "####{\"devices\":\"yes\",\"temp\":23}****";
	char *rec;
	sendData(data,strlen(data));
	while(retry--){
		if(whetherHasRecData(&rec)){
			SerialPutString("come from server\r\n");
			SerialPutString((u8*)rec);
			SerialPutString("\r\n");
			if(0==strcmp(rec,"OK")){
				clearRecDataBuffer();
				return;
			}
		}
		DelayMs(20);
		SerialPutString("do not receive data,try again\r\n");
	}
}

int main(void)
{
		InitAllPeriph();
		init_esp8266();
		DelayMs(30);
	  while (1)
    {
				if(1!=2){
					SerialPutString("app is running\r\n");
					test_AT();
					SerialPutString("test AT OK\r\n");
					connetRoute("happy","jiubugaosuni!");
					SerialPutString("connected wifi\r\n");
					while(1){
						connectTCPServer("192.168.0.121",8081);
						SerialPutString("TCP connect OK\r\n");
						while(1) {
							SerialPutString("send Tempture\r\n");
							sendTempture(10);
							DelayMs(5000);
						}
					}
				} else {
					//return bootloader
					if (((*(__IO uint32_t*)AppAddress) & 0x2FFE0000 ) == 0x20000000)
					{
							SerialPutString("Execute user Program\r\n\n");
							//jump to bootloader
							jump();
					}
					else
					{
							SerialPutString("no user Program\r\n\n");
					}
				}
    }
}
