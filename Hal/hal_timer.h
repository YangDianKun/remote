/*************************************** Copyright (c)******************************************************
** File name            :   bsp_timer.h
** Latest modified Date :   2018-06-03
** Latest Version       :   0.1
** Descriptions         :   
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
#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H

#include <stdint.h>


enum{
	TIMER5_CC_1_MODS = 0,         // 定时器通道1用于modbus协议
	TIMER5_CC_2_USART2,           // 定时器通道2用于串口USART2
};


void bsp_InitialTIMER4 (void * p_callback);

void bsp_InitHardTimer5(void);
void bsp_StartHardTimer5(uint8_t _CC, uint32_t _uiTimeOut, void * _pCallBack);

#endif

/*********************************** (C) COPYRIGHT 2018 YangDianKun ************************END OF FILE****/

