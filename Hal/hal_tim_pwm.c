/*************************************** Copyright (c)******************************************************
** File name            :   hal_tim_pwm.c
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
#include "hal_tim_pwm.h"
#include "stm32f10x.h"


/**********************************************************************************************************
** Function name        :   timer3_pwm_init
** Descriptions         :   PWM ���
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
int8_t tim3_ch4_pwm_init (void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

//	// �������
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
//	GPIO_Init(GPIOB, &GPIO_InitStruct);
//	
//	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);

	//��ʱ�� TIM3 ��ʼ��
	TIM_TimeBaseStructure.TIM_Period = 500;                      // Ԥװ��ֵ����������Ϊ
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;                // ϵͳʱ��Ϊ72Mhz����ʱ��72��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);              //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	
	TIM_Cmd(TIM3, ENABLE);
	
	return 0;
}


void d (void)
{
	TIM_SetCompare4(TIM3, 250);
}



