#include "relay.h"

int Relay_init(RelayNum r){
	GPIO_InitTypeDef GPIO_InitStructure;
	static u8 Relay_Busy=0;
	if(Relay_Busy){
		return 0;
	}else{
		Relay_Busy=1;
		GPIO_InitStructure.GPIO_Pin = R1_PIN|R2_PIN|R3_PIN|R4_PIN;								
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(RPORT, &GPIO_InitStructure);
	}
	return 1;
}
int Relay_Connect(RelayNum r){
	switch(r){
		case Relay1:
			Relay1_PIN_connected;break;
		case Relay2:
			Relay2_PIN_connected;break;
		case Relay3:
			Relay3_PIN_connected;break;
		case Relay4:
			Relay4_PIN_connected;break;
		default:break;
	}
	return 1;
}

int Relay_Disconnect(RelayNum r){
	switch(r){
		case Relay1:
			Relay1_PIN_disconnected;break;
		case Relay2:
			Relay2_PIN_disconnected;break;
		case Relay3:
			Relay3_PIN_disconnected;break;
		case Relay4:
			Relay4_PIN_disconnected;break;
		default:break;
	}
	return 1;
}

