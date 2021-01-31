/*************************************** Copyright (c)******************************************************
** File name            :   bsp_delay.c
** Latest modified Date :   2018-06-03
** Latest Version       :   0.1
** Descriptions         :   本延时函数库是用systick定时器实现的，采用非中断的方式
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






// 本地变量
static uint8_t fac_us = 0;   // 微秒单位
static uint16_t fac_ms = 0;  // 毫秒单位


/**********************************************************************************************************
** Function name        :   delayInit
** Descriptions         :   systick定时器初始化
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void delay_init (void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); // 选择外部时钟 HCLK/8
	 
	fac_us = SystemCoreClock / 8000000;                   // 系统时钟的1/8
	fac_ms = (uint16_t)fac_us * 1000;                     // 每个ms需要的systick时钟数
}

/**********************************************************************************************************
** Function name        :   delayMS
** Descriptions         :   精确毫秒延时
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void delay_ms (uint32_t nms)
{
	uint32_t temp;
	
	SysTick->LOAD = fac_ms * nms;                     // 时间加载(systick->load为24bit)
	SysTick->VAL = 0;                                 // 清空计数器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;         // 使能，减到0是无动作，采用外部时钟源
	
	do
	{
		temp = SysTick->CTRL;                           // 读取当前到计数值
	}while((temp & 0x01) && (!(temp & (1 << 16))));   // 等待时间到达
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;        // 关闭计数器
	SysTick->VAL = 0;                                 // 清空计数器
}

/**********************************************************************************************************
** Function name        :   delayUS
** Descriptions         :   精确微妙延时
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void delay_us (uint32_t nus)
{
	uint32_t temp;
	
	SysTick->LOAD = fac_us * nus;                      // 时间加载
	SysTick->VAL = 0;                                  // 清空计数器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;          // 使能，减到0是无动作，采用外部时钟源
	
	do
	{
		temp = SysTick->CTRL;                            // 读取当前到计数值
	}while((temp & 0x01) && (!(temp & (1 << 16))));    // 等待时间到达
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;         // 关闭计数器
	SysTick->VAL = 0;                                  // 清空计数器
}
	

