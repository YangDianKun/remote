/*************************************** Copyright (c)******************************************************
** File name            :   hal_config.h
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
#ifndef __HAL_CONFIG_H
#define __HAL_CONFIG_H


#include <stdio.h>
#include <string.h>
#include <math.h>


#include "hal_delay.h"
#include "hal_led.h"
#include "hal_usart.h"
#include "hal_key.h"
//#include "hal_tim.h"
#include "hal_adc.h"
#include "hal_power.h"
#include "hal_cpu_rtc.h"
#include "hal_cpu_flash.h"


/* ����ȫ���жϵĺ� */
#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */


/*  */
#define HAL_DEBUG       1



/**********************************************************************************************************
** Function name        :   halInit
** Descriptions         :   
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
void halInit (void);
	


#endif
