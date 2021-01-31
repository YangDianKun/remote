/*************************************** Copyright (c)******************************************************
** File name            :   hal_delay.h
** Latest modified Date :   2018-06-03
** Latest Version       :   0.1
** Descriptions         :   主文件，包含应用代码
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
#ifndef __HAL_DELAY_H
#define __HAL_DELAY_H

#include "stm32f10x.h"



/* 函数声明 */
void delay_init (void);
void delay_ms(uint32_t nms);
void delay_us(uint32_t nus);

#endif
