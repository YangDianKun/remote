/*************************************** Copyright (c)******************************************************
** File name            :   hal_adc.h
** Latest modified Date :   2018-06-03
** Latest Version       :   0.1
** Descriptions         :   ���ļ�������Ӧ�ô���
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
#ifndef __HAL_ADC
#define __HAL_ADC

#include "stm32f10x.h"

/* �������� */
int acd_int (void);
uint16_t getAdcValue(uint8_t ch);
uint16_t getRandomFromAdc (void);


#endif
