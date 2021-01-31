/*************************************** Copyright (c)******************************************************
** File name            :   hal_power.c
** Latest modified Date :   2018-06-03
** Latest Version       :   0.1
** Descriptions         :   mcu 电源管理
**
**--------------------------------------------------------------------------------------------------------
** Created by           :   YangDianKun
** Created date         :   2018-06-03
** Version              :   1.0
** Descriptions         :   The original version
**
**--------------------------------------------------------------------------------------------------------
** Copyright            :  
** Author Email         :   1163101403@qq.com
**********************************************************************************************************/
#include "hal_power.h"




/**********************************************************************************************************
** Function name        :   PWR_EnterSleepMode
** Descriptions         :   进入睡眠模式
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void PWR_EnterSleepMode(uint32_t SysCtrl_Set, uint8_t PWR_SLEEPEntry)
{
    if (SysCtrl_Set)
         SCB->SCR |= SysCtrl_SLEEPONEXIT_Set;   
    else
        SCB->SCR &= ~SysCtrl_SLEEPONEXIT_Set;
    SCB->SCR &= ~SysCtrl_SLEEPDEEP_Set;   
    if(PWR_SLEEPEntry == PWR_SLEEPEntry_WFI)          
        __WFI();                                       
    else
        __WFE();                                
}

/**********************************************************************************************************
** Function name        :   PWR_EnterStandbyMode
** Descriptions         :   进入待机模式，wakeup、复位、RTC中断、看门狗复位中断唤醒
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void PWR_EnterStandbyMode(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);

	PWR_WakeUpPinCmd(DISABLE); // PA0 唤醒关闭

	PWR_EnterSTANDBYMode();
}

/**********************************************************************************************************
** Function name        :   PWR_EnterStopyMode
** Descriptions         :   进入停机模式，
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void PWR_EnterStopyMode(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);

	//PWR_Regulator_ON        电源不进入低功耗，唤醒基本没有延时
	//PWR_Regulator_LowPower  电源进入低功耗，不过唤醒有一点延时
	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}
