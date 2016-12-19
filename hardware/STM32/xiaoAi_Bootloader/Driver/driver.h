#ifndef  _DRIVER_H_
#define _DRIVER_H_

#include     "stm32f10x.h"
#include     "string.h"
#include     "usart.h"


extern uint32 SysMsCnt;
extern uint32 TimingDelay;

/**************************************************************
**
**  GPIO�ܽŶ���
**
**************************************************************/
//-------------------LEDָʾ�����----------------------
#define  GPIO_LED1       GPIOA
#define  GPIO_LED2       GPIOD
#define  PIN_LED1       GPIO_Pin_8
#define  PIN_LED2       GPIO_Pin_2

//-------------------USART���----------------------
//USART1
#define  Pin_USART1_TX	   GPIO_Pin_9 
#define  Pin_USART1_RX	   GPIO_Pin_10 	  
#define  GPIO_USART1	   GPIOA 		






/**************************************************************
**
**  GPIO�ܽŲ���
**
**************************************************************/


#define     LED1_On()         GPIO_ResetBits(GPIO_LED1,PIN_LED1)
#define     LED1_Off()        GPIO_SetBits(GPIO_LED1,PIN_LED1)
#define     LED1_Toggle()     GPIO_LED1->ODR ^= PIN_LED1

#define     LED2_On()         GPIO_ResetBits(GPIO_LED2,PIN_LED2)
#define     LED2_Off()        GPIO_SetBits(GPIO_LED2,PIN_LED2)
#define     LED2_Toggle()     GPIO_LED2->ODR ^= PIN_LED2



/************************************************************
*****   ��λϵͳ
*****/
#define RESET_SYSTEM()          NVIC_GenerateSystemReset()

//���Ź�Ԥ��Ƶѡ��
#define     D1800msRST    1800
#define     D2700msRST    2700

#if WatchDogTimerEnable == 0x01
//ι��
#define     CLEAR_WDT()     (IWDG_ReloadCounter() )

//�������Ź��궨��
#define     START_WDT()      (WDT_Configuration(D2700msRST))
#else
//ι��
#define     CLEAR_WDT()     

//�������Ź��궨��
#define     START_WDT()      
#endif



typedef  void (*pFunction)(void);


/**************************************************************
**
**  ������������
**
**************************************************************/



extern void _NOP_(uint32 cnt);
extern void Delay(uint32 nTime);



extern void TestInnerFlash(void);
extern void InitAllPeriph(void);		//	 ��ʼ�����躯��


#endif




