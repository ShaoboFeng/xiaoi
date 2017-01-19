#include "usart2.h"
#include "driver.h"
#include "usart.h"
#include "stm32f10x_it.h"
#include <stdio.h>
#include <stdlib.h>

#define  tty_pin_tx	GPIO_Pin_2
#define  tty_pin_rx GPIO_Pin_3
u8 RXbytes[128] = {0};
u8 rxlen = 0;
u8 flag = 0;
u8 RecBuffer[1024] = {0};
u8 canRecFlag = 0; //if connnected tcp
u8 startRecData = 0;
int RecDataSize = 0;
void tty_init(void){
	
		USART_InitTypeDef USART_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		//config pin
	  GPIO_InitStructure.GPIO_Pin = tty_pin_tx;	         //tx
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = tty_pin_rx;	         //rx
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		//usart configure
	  USART_StructInit(&USART_InitStructure);
	  USART_InitStructure.USART_BaudRate =115200;
	  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	  USART_InitStructure.USART_Parity = USART_Parity_No;
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	 
	  USART_Init(USART2,& USART_InitStructure);
		USART_ITConfig(USART2, USART_IT_RXNE,ENABLE);
		
	  USART_Cmd(USART2, ENABLE);
		USART_ClearITPendingBit( USART2, USART_IT_RXNE );// Clear the USART  Receive interrup/
}


int tty_command(char *cmd){
		return 1;
}
void tty_putchar(uint8_t c)
{
    USART_SendData(USART2, c);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
    {}
}
void tty_putchars(uint8_t *p,unsigned int length)
{
	unsigned char i;
	for(i=0; i<length; i++)
	{		
		tty_putchar(*p);
		p++;
	}
}

int tty_printf(char *str){
	while(*str){
		USART_SendData(USART2,*str++);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
	}
	return 1;
}

int tty_receive(char *rec){
	if (USART_GetFlagStatus(USART2,USART_FLAG_RXNE) != RESET){
		*rec = (char)USART1->DR;
		return 1;
	}else{
		return 0;
	}	
}

/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
*******************************************************************************/
void USART2_IRQHandler(void)
{
  if( USART_GetITStatus( USART2, USART_IT_RXNE ) != RESET )
  {
		if(startRecData){ //deal date from tcp
			RecBuffer[rxlen++] = USART_ReceiveData(USART2);
			if(RecBuffer[rxlen-1]==':'){
				RecBuffer[rxlen-1]='\0';
				RecDataSize = atoi((const char *)RecBuffer);
				rxlen = 0;
			}
			else	if(RecDataSize!=0&&rxlen >= RecDataSize){
					RecBuffer[rxlen] = '\0';
					//SerialPutString("rec a pagecke\r\n");
					//SerialPutString(RecBuffer);
					//SerialPutString("\r\n");
					startRecData = 0;
					clearRXbytes();
			}
		}
		else if(rxlen<sizeof(RXbytes)){
			RXbytes[rxlen++] = USART_ReceiveData(USART2);
			if(canRecFlag&&rxlen>=5){ //if the bytes come fome tcp
				if(RXbytes[rxlen-1]==','&&RXbytes[rxlen-2]=='D'
					&&RXbytes[rxlen-3]=='P'&&RXbytes[rxlen-4]=='I'&&RXbytes[rxlen-5]=='+'){
					startRecData = 1;
					clearRXbytes();
					
				}
			}
			else if(canRecFlag&&rxlen>=6){
				if(RXbytes[rxlen-1]=='D'&&RXbytes[rxlen-2]=='E'&&RXbytes[rxlen-3]=='S'&&RXbytes[rxlen-4]=='O'
					&&RXbytes[rxlen-5]=='L'&&RXbytes[rxlen-6]=='C'){
						SerialPutString("close connect tcp\r\n");
						canRecFlag = 0;
						clearRXbytes();
					}
			}
			else if(RXbytes[rxlen-1] == 0x00||RXbytes[rxlen-1] == '\n'){
				flag = 1;
			}
		}else{
			//drop data
			USART_ReceiveData(USART2);
			flag = 1;
		}
		//tty_putchars(RXbytes,rxlen);
		USART_ClearITPendingBit( USART2, USART_IT_RXNE );// Clear the USART  Receive interrup/
	}
}

/*
* data:the buffer for receive data
* len: the buffer size
* return: the receive data size , -1 stand for error
*/
int tty_recString(char *data, int len){
	int datasize = 0;
	while(1){
		char b;
		tty_receive(&b);
		if(datasize>=len){
			return -1;
		}
		*(data+datasize) = b;
		datasize++;
		if(b == 0){
			return datasize;
		}
	}
}
//esp8266
void init_esp8266(void){
	tty_init();
}

u8 send_ATCmdWithRetType(char *cmd,u8 retType){
	return send_ATCmdWithRetTypeAndTime(cmd,retType,100);
}
u8 send_ATCmdWithRetTypeAndTime(char *cmd,u8 retType,int delay){
	u8 i;
	u8 retry = 3;
	tty_printf(cmd);
	if(retType == NO_NEED_RET){
		DelayMs(delay);
		return SUCCESS;
	}
	while(retry--){
		DelayMs(delay);
		if(flag){
			for(i=1;i<rxlen;i++){
				if(RXbytes[i-1]=='O'&&RXbytes[i]=='K'){
						clearRXbytes();
						return SUCCESS;
				}
				if(i<4) continue;
				if(RXbytes[i-4]=='E'&&RXbytes[i-3]=='R'
							&&RXbytes[i-2]=='R'&&RXbytes[i-1]=='O'
							&&RXbytes[i]=='R'){
								clearRXbytes();
						return ERROR;
				}
			}
			clearRXbytes();
		}else{
			tty_printf(cmd);
		}
	}
	return ERROR;
}

u8 send_ATCmd(char *cmd){
	return send_ATCmdWithRetType(cmd,OK_OR_ERROR);
}

void test_AT(void){
	u8 ret = send_ATCmd("AT\r\n");
	//tty_recString(buf,sizeof(buf));
	if(ret == SUCCESS)
		SerialPutString("ret OK\n");
	//SerialPutString(buf);
}

u8 changeToMode(u8 mode){
	int ret;
	switch(mode){
		case 1 :
			ret = send_ATCmd("AT+CWMODE=1\r\n");
			break;
		case 2 :
			ret = send_ATCmd("AT+CWMODE=2\r\n");
			break;
		case 3 :
			ret = send_ATCmd("AT+CWMODE=3\r\n");
			break;
	}
	return ret;
}

u8 connetRoute(char *ssid, char *password){
	char cmd[100]={0};
  sprintf(cmd,"AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,password);
	return send_ATCmdWithRetTypeAndTime(cmd,OK_OR_ERROR,2000);
}

u8 connectTCPServer(char *ip, int port){
	u8 ret = 0;
	char cmd[100]={0};
	sprintf(cmd,"AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",ip,port);
	ret=send_ATCmd(cmd);
	if(ret == SUCCESS){
		canRecFlag = 1;
	}
	return ret;
}

u8 ptmode = 0;
u8 startpassthrough(){
	if(ptmode==0){
		return send_ATCmd("AT+CIPMODE=1\r\n"); 
	}
	return ERROR;
}

u8 stoppassthrough() {
	if(ptmode==1){
		return send_ATCmd("+++");
	}
	return ERROR;
}

u8 buildTCPServer(int port){
	u8 ret = 0;
	char cmd[100]={0};
	ret = send_ATCmd("AT+CIPMUX=1\r\n");
	if(ret != SUCCESS){
		return ret;
	}
	sprintf(cmd,"AT+CIPSERVER=1\r\n");
	return send_ATCmd(cmd);
}

u8 sendData(char *data, int len){
	u8 ret = 0;
	char cmd[100]={0};
  if(!canRecFlag){
		return ERROR;
	}
	sprintf(cmd,"AT+CIPSEND=%d\r\n",len);
	ret = send_ATCmdWithRetType(cmd,NO_NEED_RET);
	if(ret !=SUCCESS){
		return ret;
	}
	return send_ATCmdWithRetType(data,NO_NEED_RET);
}

u8 whetherHasRecData(char **buffer){
	if(RecDataSize!=0){
		*buffer = (char*)RecBuffer;
		return RecDataSize;
	}
	return 0;
}

void clearRecDataBuffer(void){
	RecDataSize = 0;
	memset(RecBuffer,0,sizeof(RecBuffer));
}

void clearRXbytes(void){
	flag = 0;
	rxlen = 0;
	memset(RXbytes,0,sizeof(RXbytes));
}
