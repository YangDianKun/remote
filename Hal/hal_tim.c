/*************************************** Copyright (c)******************************************************
** File name            :   hal_tim.c
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
#include "hal_tim.h"
#include "stdio.h"
#include "board.h"
#include "handle.h"




/*
	ȫ������ʱ�䣬��λ10ms
*/
__IO int32_t g_iRunTime = 0;

/* ���������ʱ���ṹ����� */
static SoftTmr_t s_tTmr[TMR_COUNT];


static void soft_timer_init(void);
static void soft_timer_dec(SoftTmr_t *_tmr);

/**********************************************************************************************************
** Function name        :   hardTim4Init
** Descriptions         :   
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
int8_t timer4_init (void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	soft_timer_init();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);


	//��ʱ��TIM4��ʼ��
	TIM_TimeBaseStructure.TIM_Period = 1000;                     // Ԥװ��ֵ����������Ϊ100ms
	TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;              // ϵͳʱ��Ϊ72Mhz����ʱ��7200��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;               //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);                               //��ʼ��NVIC�Ĵ���

    
	TIM_Cmd(TIM4, ENABLE);                                        //ʹ��TIMx	
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE );                   //ʹ��ָ����TIM4�ж�,��������ж�	
	
	return 0;
}

/*********************************************************************************************************
*	�� �� ��: soft_timer_init
*	����˵��: Ӳ����ʱ�� TIM4 ģ�����ʱ��
*	��    ��:  _
*	�� �� ֵ: ��
*********************************************************************************************************/
static void soft_timer_init(void)
{
	uint8_t i;
	
	/* �������е������ʱ�� */
	for (i = 0; i < TMR_COUNT; i++)
	{
		s_tTmr[i].Count = 0;
		s_tTmr[i].PreLoad = 0;
		s_tTmr[i].Flag = 0;
		s_tTmr[i].Mode = TMR_ONCE_MODE;	/* ȱʡ��1���Թ���ģʽ */
	}
}

/*********************************************************************************************************
*	�� �� ��: soft_timer_dec
*	����˵��: ÿ��1ms�����ж�ʱ��������1�����뱻SysTick_ISR�����Ե��á�
*	��    ��:  _tmr : ��ʱ������ָ��
*	�� �� ֵ: ��
*********************************************************************************************************/
static void soft_timer_dec(SoftTmr_t *_tmr)
{
	if (_tmr->Count > 0)
	{
		/* �����ʱ����������1�����ö�ʱ�������־ */
		if (--_tmr->Count == 0)
		{
			_tmr->Flag = 1;

			/* ������Զ�ģʽ�����Զ���װ������ */
			if(_tmr->Mode == TMR_AUTO_MODE)
			{
				_tmr->Count = _tmr->PreLoad;
			}
		}
	}
}

/*********************************************************************************************************
*	�� �� ��: soft_timer_start
*	����˵��: ����һ����ʱ���������ö�ʱ���ڡ�
*	��    ��:  	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*				_period : ��ʱ���ڣ���λ1ms
*	�� �� ֵ: ��
*********************************************************************************************************/
void soft_timer_start(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		_id = TMR_COUNT - 1;
	}

	//DISABLE_INT();  			/* ���ж� */

	s_tTmr[_id].Count = _period;		/* ʵʱ��������ֵ */
	s_tTmr[_id].PreLoad = _period;		/* �������Զ���װֵ�����Զ�ģʽ������ */
	s_tTmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 1���Թ���ģʽ */

	//ENABLE_INT();  				/* ���ж� */
}

/*********************************************************************************************************
*	�� �� ��: soft_timer_auto
*	����˵��: ����һ���Զ���ʱ���������ö�ʱ���ڡ�
*	��    ��:  	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*				_period : ��ʱ���ڣ���λ10ms
*	�� �� ֵ: ��
*********************************************************************************************************/
void soft_timer_auto(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		_id = TMR_COUNT - 1;
	}

	//DISABLE_INT();  		/* ���ж� */

	s_tTmr[_id].Count = _period;			/* ʵʱ��������ֵ */
	s_tTmr[_id].PreLoad = _period;		/* �������Զ���װֵ�����Զ�ģʽ������ */
	s_tTmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	s_tTmr[_id].Mode = TMR_AUTO_MODE;	/* �Զ�����ģʽ */

	//ENABLE_INT();  			/* ���ж� */
}

/*********************************************************************************************************
*	�� �� ��: soft_timer_stop
*	����˵��: ֹͣһ����ʱ��
*	��    ��:  	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*	�� �� ֵ: ��
*********************************************************************************************************/
void soft_timer_stop(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		_id = TMR_COUNT - 1;
	}

	//DISABLE_INT();  	/* ���ж� */

	s_tTmr[_id].Count = 0;				/* ʵʱ��������ֵ */
	s_tTmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* �Զ�����ģʽ */

	//ENABLE_INT();  		/* ���ж� */
}

/*********************************************************************************************************
*	�� �� ��: soft_timer_check
*	����˵��: ��ⶨʱ���Ƿ�ʱ
*	��    ��:  	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*				_period : ��ʱ���ڣ���λ1ms
*	�� �� ֵ: ���� 0 ��ʾ��ʱδ���� 1��ʾ��ʱ��
*********************************************************************************************************/
uint8_t soft_timer_check(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		return 0;
	}

	if (s_tTmr[_id].Flag == 1)
	{
		s_tTmr[_id].Flag = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

/**********************************************************************************************************
** Function name        :   get_timer4_tick
** Descriptions         :   
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
uint32_t get_timer4_tick (void)
{
	return g_iRunTime;
}
/**********************************************************************************************************
** Function name        :   SysTick_Handler
** Descriptions         :   TIM4�жϺ���������
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
void TIM4_IRQHandler (void)
{
	uint8_t i;
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		
		/* ȫ������ʱ��ÿ100ms��1 */
		g_iRunTime++;
		
		/* ÿ��100ms���������ʱ���ļ��������м�һ���� */
		for (i = 0; i < TMR_COUNT; i++)
		{
			soft_timer_dec(&s_tTmr[i]);
		}
		
	}
}

