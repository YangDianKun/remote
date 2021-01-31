/*************************************** Copyright (c)******************************************************
** File name            :   hal_config.c
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
#include "hal_config.h"



void halKeyInit (void);

/**********************************************************************************************************
** Function name        :   sysInit
** Descriptions         :   
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
void sysInit (void)
{
	
}


/**********************************************************************************************************
** Function name        :   halInit
** Descriptions         :   
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
void halInit (void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	sysInit();
	
	
	// �ر����ж�	
	__set_PRIMASK(1);

	
	delayInit();
	ledsInit();
	initial_lcd();
	hardTim4Init();
	adcInit();
	
	halKeyInit();
	
	// �������ж�
	__set_PRIMASK(0);
}


void halKeyInit (void)
{
	keyInit_t pKeyInit;
	uint8_t i;
	
	for(i = 0; i < KEY_NUMBER; i++)
	{
		pKeyInit.filter = 20;
		pKeyInit.id = i;
		keyInit(&pKeyInit);
	}
}
