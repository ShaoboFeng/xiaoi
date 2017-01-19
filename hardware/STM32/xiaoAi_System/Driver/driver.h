#ifndef  _DRIVER_H_
#define _DRIVER_H_

#include "stm32f10x.h"
#include "string.h"
#include "usart.h"
#include "usart2.h"
#include "delay.h"

/**************************************************************
**
**  GPIO管脚定义
**
**************************************************************/
//-------------------LED---------------------
#define  GPIO_LED1       GPIOA
#define  GPIO_LED2       GPIOD
#define  PIN_LED1       GPIO_Pin_8
#define  PIN_LED2       GPIO_Pin_2

//-------------------------------------
//USART1
#define  Pin_USART1_TX	   GPIO_Pin_9 
#define  Pin_USART1_RX	   GPIO_Pin_10 	  
#define  GPIO_USART1	   GPIOA 		


/**************************************************************
**
**  GPIO管脚操作
**
**************************************************************/


#define     LED1_On()         GPIO_ResetBits(GPIO_LED1,PIN_LED1)
#define     LED1_Off()        GPIO_SetBits(GPIO_LED1,PIN_LED1)
#define     LED1_Toggle()     GPIO_LED1->ODR ^= PIN_LED1

#define     LED2_On()         GPIO_ResetBits(GPIO_LED2,PIN_LED2)
#define     LED2_Off()        GPIO_SetBits(GPIO_LED2,PIN_LED2)
#define     LED2_Toggle()     GPIO_LED2->ODR ^= PIN_LED2



//restart system
#define RESET_SYSTEM()          NVIC_GenerateSystemReset()
typedef  void (*pFunction)(void);
/**************************************************************
**
**  函数声明部分
**
**************************************************************/



extern void _NOP_(uint32 cnt);
extern void TestInnerFlash(void);
extern void InitAllPeriph(void);		//	 初始化外设函数


#endif




