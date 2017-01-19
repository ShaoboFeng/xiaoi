#include "stm32f10x_tim.h"
#include "stm32f10x.h"
#include "delay.h"
/******************
function:Delay function initialize
	important task is initialize timer3
author:Zachary
time:2013/8/12
*****************/
u8 fac_us=0;
u16 fac_ms=0;
void DelayMs_init(void){
	u8 SYSCLK = 72;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period=500-1;
	TIM_TimeBaseStructure.TIM_Prescaler=(SYSCLK-1);
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	fac_us=SYSCLK/8;
	fac_ms=(u16)fac_us*1000;
}

//make sure i < 1500
void DelayMs(u32 i){
		u32 temp;
		u32 loop;
	  for(loop = 0;loop < i;loop++){
			SysTick->LOAD=1000*fac_us;          //时间加载                       
			SysTick->VAL=0x00;                //清空计数器  
			SysTick->CTRL=0x01 ;              //Enable Systick，开始倒数       
			do{  
				temp=SysTick->CTRL;  
			}while(temp&0x01&&!(temp&(1<<16)));//等待时间到达        
			SysTick->CTRL=0x00;              //Disable Systick 
			SysTick->VAL =0X00;              //清空计数器
		}
}

void DelayUs(u32 nus)  
{                
		u32 temp;                            
		SysTick->LOAD=nus*fac_us;              //时间加载                       
		SysTick->VAL=0x00;                //清空计数器  
		SysTick->CTRL=0x01 ;              //Enable Systick，开始倒数       
		do{  
			temp=SysTick->CTRL;  
		}while(temp&0x01&&!(temp&(1<<16)));//等待时间到达        
		SysTick->CTRL=0x00;              //Disable Systick 
		SysTick->VAL =0X00;              //清空计数器          
}
