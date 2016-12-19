#ifndef __RTC_H
#define __RTC_H

/* Includes ------------------------------------------------------------------*/


#include "stm32f10x.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_bkp.h"




/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  RTCClockOutput_Enable
/* Private macro -------------------------------------------------------------*/
#define CONFIGURATION_DONE  0xA5A5 //配置完成0xAAAA //配置完成
#define CONFIGURATION_RESET 0x0000 //配置复位
/* Private function prototypes -----------------------------------------------*/


extern uint32 TimeDisplay;



extern void RTC_Init(void);
extern void Get_Time(void);

#endif 

/**
  ****************** (C) COPYRIGHT 2011 Tiamaes *****END OF FILE**************
**/
