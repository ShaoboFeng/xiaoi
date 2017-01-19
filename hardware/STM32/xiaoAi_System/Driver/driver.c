#include "driver.h"
#include "delay.h"
#include "iic.h"

uint32 SysMsCnt = 0;
uint32 TimingDelay = 0;



/************************************************************************
** Function Name : RCC_Configuration
** Description      : 配置时钟
** Input Parameters  :
                
** Output Parameters :
                = 1  : 保存成功
                = 0  : 保存失败
    note :                         
************************************************************************/

void RCC_Configuration(void)
{
    ErrorStatus HSEStartUpStatus;
	// RCC system reset(for debug purpose) 
    RCC_DeInit();
    // Enable HSE   
    RCC_HSEConfig(RCC_HSE_ON);
    // Wait till HSE is ready 
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    if(HSEStartUpStatus == SUCCESS)
    {
        // HCLK = SYSCLK 				AHB时钟为系统时钟  72MHz 
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        // PCLK2 = HCLK 				APB2时钟为系统时钟 72MHz
        RCC_PCLK2Config(RCC_HCLK_Div1);
        // PCLK1 = HCLK/2 				APB1时钟为系统时钟 72MHz/2=36MHz
        RCC_PCLK1Config(RCC_HCLK_Div2);
        // Flash 2 wait state 
        FLASH_SetLatency(FLASH_Latency_2);
        // Enable Prefetch Buffer 
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        // PLLCLK = 8MHz * 9 = 72 MHz 
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
        // Enable PLL 
        RCC_PLLCmd(ENABLE);
        // Wait till PLL is ready 
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }
        // Select PLL as system clock source 
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        // Wait till PLL is used as system clock source 
        while(RCC_GetSYSCLKSource() != 0x08)
        {
        }
    }  
 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO |
                                            RCC_APB2Periph_GPIOA |
                                            RCC_APB2Periph_GPIOB |
                                            RCC_APB2Periph_GPIOC |
											                      RCC_APB2Periph_GPIOD |
                                            RCC_APB2Periph_USART1 |
                                            RCC_APB2Periph_SPI1
                                            , ENABLE);
                                            
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 	  
#if EnableWdt                                            
    RCC_LSICmd(ENABLE);//打开LSI
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);//等待直到LSI稳定
#endif
}


/************************************************************************
** Function Name : NVIC_Configuration
** Description      : 配置中断                    
************************************************************************/
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    #ifdef  VECT_TAB_RAM  
    // Set the Vector Table base location at 0x20000000 
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
    #else  /* VECT_TAB_FLASH  */
    // Set the Vector Table base location at 0x08000000 
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
    #endif
    // Configure one bit for preemption priority 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	  /*NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		*/

		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
}
/************************************************************************
** Function Name : HW_LED_PinInit
** Description      : USART GPIO初始化                       
************************************************************************/
void HW_LED_PinInit( void )
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = PIN_LED1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init( GPIO_LED1, &GPIO_InitStructure );
		GPIO_InitStructure.GPIO_Pin = PIN_LED2 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init( GPIO_LED2, &GPIO_InitStructure );
}
/************************************************************************
** Function Name : USART1_PinInit                       
************************************************************************/
void USART1_PinInit( void )
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = Pin_USART1_TX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIO_USART1, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = Pin_USART1_RX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIO_USART1, &GPIO_InitStructure);
}


/************************************************************************
** Function Name : GPIO_Configuration
************************************************************************/
void GPIO_Configuration(void)
{

	HW_LED_PinInit();
	USART1_PinInit();
}
/************************************************************************
** Function Name : USART1_Configuration                      
************************************************************************/
void USART1_Configuration(uint32 val )
{
    USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef  USART_ClockInitStructure;
    
    USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
    /* Configure the USART1 synchronous paramters */
    USART_ClockInit(USART1, &USART_ClockInitStructure);

    USART_InitStructure.USART_BaudRate = val;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    /* Configure USART1 basic and asynchronous paramters */
    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    /* Enable USART1 */
    USART_Cmd(USART1, ENABLE);
}

/************************************************************************
** Function Name : InitAllPeriph
** Description      : 配置外设，用于和硬盘录像机通讯
** Input Parameters  :
                
** Output Parameters :

    note :                         
************************************************************************/
void InitAllPeriph(void)
{
		DelayMs_init();
    RCC_Configuration();		
    NVIC_Configuration();			
    GPIO_Configuration();	
	  USART1_Configuration(115200);	
		IIC_Init();
}

/************************************************************************
** Function Name :   Refresh_SFR
** Description      : 刷新单片机特殊功能寄存器
** Input Parameters  :
                
** Output Parameters :

    note :                         
************************************************************************/
void Refresh_SFR(void)
{

    #if (EnableWdt && (WDT_OVF_TEST == 0))
    CLEAR_WDT();
    #endif
}
