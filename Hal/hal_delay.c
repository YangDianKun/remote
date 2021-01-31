/*************************************** Copyright (c)******************************************************
** File name            :   bsp_delay.c
** Latest modified Date :   2018-06-03
** Latest Version       :   0.1
** Descriptions         :   ����ʱ����������systick��ʱ��ʵ�ֵģ����÷��жϵķ�ʽ
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
#include "hal_delay.h"	






// ���ر���
static uint8_t fac_us = 0;   // ΢�뵥λ
static uint16_t fac_ms = 0;  // ���뵥λ


/**********************************************************************************************************
** Function name        :   delayInit
** Descriptions         :   systick��ʱ����ʼ��
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
void delay_init (void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); // ѡ���ⲿʱ�� HCLK/8
	 
	fac_us = SystemCoreClock / 8000000;                   // ϵͳʱ�ӵ�1/8
	fac_ms = (uint16_t)fac_us * 1000;                     // ÿ��ms��Ҫ��systickʱ����
}

/**********************************************************************************************************
** Function name        :   delayMS
** Descriptions         :   ��ȷ������ʱ
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
void delay_ms (uint32_t nms)
{
	uint32_t temp;
	
	SysTick->LOAD = fac_ms * nms;                     // ʱ�����(systick->loadΪ24bit)
	SysTick->VAL = 0;                                 // ��ռ�����
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;         // ʹ�ܣ�����0���޶����������ⲿʱ��Դ
	
	do
	{
		temp = SysTick->CTRL;                           // ��ȡ��ǰ������ֵ
	}while((temp & 0x01) && (!(temp & (1 << 16))));   // �ȴ�ʱ�䵽��
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;        // �رռ�����
	SysTick->VAL = 0;                                 // ��ռ�����
}

/**********************************************************************************************************
** Function name        :   delayUS
** Descriptions         :   ��ȷ΢����ʱ
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
void delay_us (uint32_t nus)
{
	uint32_t temp;
	
	SysTick->LOAD = fac_us * nus;                      // ʱ�����
	SysTick->VAL = 0;                                  // ��ռ�����
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;          // ʹ�ܣ�����0���޶����������ⲿʱ��Դ
	
	do
	{
		temp = SysTick->CTRL;                            // ��ȡ��ǰ������ֵ
	}while((temp & 0x01) && (!(temp & (1 << 16))));    // �ȴ�ʱ�䵽��
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;         // �رռ�����
	SysTick->VAL = 0;                                  // ��ռ�����
}
	

