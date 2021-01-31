/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
#include "board.h"
#include "handle.h"
#include "interact.h"
#include "main.h"


/* // 本地变量 */
volatile uint8_t rtcSecInterrupt = 0;


/* // 外部变量 */
extern volatile uint8_t sysStatus;
extern volatile uint8_t sysMode;

/**********************************************************************************************************
** Function name        :   EXTI0_IRQHandler
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void EXTI0_IRQHandler(void)        
{
    EXTI_ClearFlag(EXTI_Line0);      // 清除外部中断 0，等待下一次触发
	
	if ((sysMode == SYS_LOW_POWER_MODE) && (sysStatus == SYS_WAIT_WAKE_STATUS))
	{
		sysStatus = SYS_ACTION_STATUS;     // 系统状态，配置为作业模式
		/* 低功耗唤醒中断处理 */
		board_exit_low_Power();     // 退出低功耗模式
	}
	
	
}

/*********************************************************************************************************
*	函 数 名: RTC_IRQHandler
*	功能说明: 
*	形    参:  
*	返 回 值: 无
*********************************************************************************************************/
void RTC_IRQHandler(void)
{
   if(RTC_GetITStatus(RTC_IT_SEC) != RESET)//读取中断标志
   {
	   RTC_ClearITPendingBit(RTC_IT_SEC);  //清楚中断标志

		if (!rtcSecInterrupt)
		{
			rtcSecInterrupt = 1;
		}
   }
}

/**********************************************************************************************************
** Function name        :   USART2_IRQHandler
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void USART2_IRQHandler(void)                	
{} 

void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
	#ifdef MCU_ERR_DISP
	lcd_clear();
	lcd_str_5x8_line(3, 0, 0, "HardFault....");
	#endif
	while (1)
	{
	}
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
	#ifdef MCU_ERR_DISP
	lcd_clear();
	lcd_str_5x8_line(3, 0, 0, "MemManage....");
	#endif
	while (1)
	{
	}
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
	#ifdef MCU_ERR_DISP
	lcd_clear();
	lcd_str_5x8_line(3, 0, 0, "BusFault....");
	#endif
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
	#ifdef MCU_ERR_DISP
	lcd_clear();
	lcd_str_5x8_line(3, 0, 0, "UsageFault....");
	#endif
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
void SysTick_Handler(void)
{
}

void NMI_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
