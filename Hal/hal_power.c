/*************************************** Copyright (c)******************************************************
** File name            :   hal_power.c
** Latest modified Date :   2018-06-03
** Latest Version       :   0.1
** Descriptions         :   mcu ��Դ����
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
** Descriptions         :   ����˯��ģʽ
** parameters           :   ��
** Returned value       :   ��
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
** Descriptions         :   �������ģʽ��wakeup����λ��RTC�жϡ����Ź���λ�жϻ���
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
void PWR_EnterStandbyMode(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);

	PWR_WakeUpPinCmd(DISABLE); // PA0 ���ѹر�

	PWR_EnterSTANDBYMode();
}

/**********************************************************************************************************
** Function name        :   PWR_EnterStopyMode
** Descriptions         :   ����ͣ��ģʽ��
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
void PWR_EnterStopyMode(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);

	//PWR_Regulator_ON        ��Դ������͹��ģ����ѻ���û����ʱ
	//PWR_Regulator_LowPower  ��Դ����͹��ģ�����������һ����ʱ
	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}
