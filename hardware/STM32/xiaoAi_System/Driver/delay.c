#include "stm32f10x_tim.h"
#include "stm32f10x.h"
#include "delay.h"
/******************
function:Delay function initialize
	important task is initialize timer3
author:Zachary
time:2013/8/12
*****************/
void DelayMs_init(void){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period=500-1;
	TIM_TimeBaseStructure.TIM_Prescaler=(72-1);
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
}

void DelayMs(u32 i){
	u32 m;
	for(m=0;m<i;m++){
		TIM_Cmd(TIM3,ENABLE);
		while(TIM_GetITStatus(TIM3,TIM_IT_Update)==RESET);
					TIM_ClearFlag(TIM3,TIM_FLAG_Update);
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
					TIM_Cmd(TIM3,DISABLE);
	}
}

void DelayUs(u32 nus)  
{                
        u32 temp;                            
        SysTick->LOAD=nus*9; //时间加载                       
        SysTick->VAL=0x00;                //清空计数器  
        SysTick->CTRL=0x01 ;            //Enable Systick，开始倒数       
        do  
        {  
            temp=SysTick->CTRL;  
        }while(temp&0x01&&!(temp&(1<<16)));//等待时间到达        
        SysTick->CTRL=0x00;              //Disable Systick 
        SysTick->VAL =0X00;              //清空计数器          
}
