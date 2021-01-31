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


/* // ���ر��� */
volatile uint8_t rtcSecInterrupt = 0;


/* // �ⲿ���� */
extern volatile uint8_t sysStatus;
extern volatile uint8_t sysMode;

/**********************************************************************************************************
** Function name        :   EXTI0_IRQHandler
** Descriptions         :   
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
void EXTI0_IRQHandler(void)        
{
    EXTI_ClearFlag(EXTI_Line0);      // ����ⲿ�ж� 0���ȴ���һ�δ���
	
	if ((sysMode == SYS_LOW_POWER_MODE) && (sysStatus == SYS_WAIT_WAKE_STATUS))
	{
		sysStatus = SYS_ACTION_STATUS;     // ϵͳ״̬������Ϊ��ҵģʽ
		/* �͹��Ļ����жϴ��� */
		board_exit_low_Power();     // �˳��͹���ģʽ
	}
	
	
}

/*********************************************************************************************************
*	�� �� ��: RTC_IRQHandler
*	����˵��: 
*	��    ��:  
*	�� �� ֵ: ��
*********************************************************************************************************/
void RTC_IRQHandler(void)
{
   if(RTC_GetITStatus(RTC_IT_SEC) != RESET)//��ȡ�жϱ�־
   {
	   RTC_ClearITPendingBit(RTC_IT_SEC);  //����жϱ�־

		if (!rtcSecInterrupt)
		{
			rtcSecInterrupt = 1;
		}
   }
}

/**********************************************************************************************************
** Function name        :   USART2_IRQHandler
** Descriptions         :   
** parameters           :   ��
** Returned value       :   ��
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
