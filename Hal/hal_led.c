/*************************************** Copyright (c)******************************************************
** File name            :   hal_led.c
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
#include "hal_led.h"	
#include "hal_delay.h"







/**********************************************************************************************************
** Function name        :   ledGpioInit
** Descriptions         :   
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
int8_t led_init (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// ��������ʱ��
	RCC_APB2PeriphClockCmd(LED_PORT_RCC, ENABLE);

	// �������
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;

	// LED1 -> 
	GPIO_InitStruct.GPIO_Pin = LED1_PIN;
	GPIO_Init(LED1_PORT, &GPIO_InitStruct);
	
	LED1_OFF();
	
	return 0;
}






