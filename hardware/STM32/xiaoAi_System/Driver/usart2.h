#ifndef tty
#define tty
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
void tty_init(void);
int tty_command(char *cmd);
int tty_printf(char *str);

//function esp8266
enum{
	NO_NEED_RET,
	NEED_RET,
	OK_OR_ERROR,
};
void init_esp8266(void);
void test_AT(void);
u8 connetRoute(char *ssid,char *pwd);
u8 connectTCPServer(char *ip,int port);
u8 sendData(char *data, int len);
u8 whetherHasRecData(char **buffer);
void clearRecDataBuffer(void);
void clearRXbytes(void);
u8 send_ATCmdWithRetTypeAndTime(char *cmd,u8 retType,int delay);
#endif
