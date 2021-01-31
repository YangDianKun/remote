/*************************************** Copyright (c)******************************************************
** File name            :   hal_tim.c
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
#include "hal_tim.h"
#include "stdio.h"
#include "board.h"
#include "handle.h"




/*
	全局运行时间，单位10ms
*/
__IO int32_t g_iRunTime = 0;

/* 定于软件定时器结构体变量 */
static SoftTmr_t s_tTmr[TMR_COUNT];


static void soft_timer_init(void);
static void soft_timer_dec(SoftTmr_t *_tmr);

/**********************************************************************************************************
** Function name        :   hardTim4Init
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
int8_t timer4_init (void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	soft_timer_init();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);


	//定时器TIM4初始化
	TIM_TimeBaseStructure.TIM_Period = 1000;                     // 预装载值，更新周期为100ms
	TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;              // 系统时钟为72Mhz，定时器7200分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              //根据指定的参数初始化TIMx的时间基数单位

	

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;               //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);                               //初始化NVIC寄存器

    
	TIM_Cmd(TIM4, ENABLE);                                        //使能TIMx	
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE );                   //使能指定的TIM4中断,允许更新中断	
	
	return 0;
}

/*********************************************************************************************************
*	函 数 名: soft_timer_init
*	功能说明: 硬件定时器 TIM4 模拟出软定时器
*	形    参:  _
*	返 回 值: 无
*********************************************************************************************************/
static void soft_timer_init(void)
{
	uint8_t i;
	
	/* 清零所有的软件定时器 */
	for (i = 0; i < TMR_COUNT; i++)
	{
		s_tTmr[i].Count = 0;
		s_tTmr[i].PreLoad = 0;
		s_tTmr[i].Flag = 0;
		s_tTmr[i].Mode = TMR_ONCE_MODE;	/* 缺省是1次性工作模式 */
	}
}

/*********************************************************************************************************
*	函 数 名: soft_timer_dec
*	功能说明: 每隔1ms对所有定时器变量减1。必须被SysTick_ISR周期性调用。
*	形    参:  _tmr : 定时器变量指针
*	返 回 值: 无
*********************************************************************************************************/
static void soft_timer_dec(SoftTmr_t *_tmr)
{
	if (_tmr->Count > 0)
	{
		/* 如果定时器变量减到1则设置定时器到达标志 */
		if (--_tmr->Count == 0)
		{
			_tmr->Flag = 1;

			/* 如果是自动模式，则自动重装计数器 */
			if(_tmr->Mode == TMR_AUTO_MODE)
			{
				_tmr->Count = _tmr->PreLoad;
			}
		}
	}
}

/*********************************************************************************************************
*	函 数 名: soft_timer_start
*	功能说明: 启动一个定时器，并设置定时周期。
*	形    参:  	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*				_period : 定时周期，单位1ms
*	返 回 值: 无
*********************************************************************************************************/
void soft_timer_start(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		_id = TMR_COUNT - 1;
	}

	//DISABLE_INT();  			/* 关中断 */

	s_tTmr[_id].Count = _period;		/* 实时计数器初值 */
	s_tTmr[_id].PreLoad = _period;		/* 计数器自动重装值，仅自动模式起作用 */
	s_tTmr[_id].Flag = 0;				/* 定时时间到标志 */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 1次性工作模式 */

	//ENABLE_INT();  				/* 开中断 */
}

/*********************************************************************************************************
*	函 数 名: soft_timer_auto
*	功能说明: 启动一个自动定时器，并设置定时周期。
*	形    参:  	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*				_period : 定时周期，单位10ms
*	返 回 值: 无
*********************************************************************************************************/
void soft_timer_auto(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		_id = TMR_COUNT - 1;
	}

	//DISABLE_INT();  		/* 关中断 */

	s_tTmr[_id].Count = _period;			/* 实时计数器初值 */
	s_tTmr[_id].PreLoad = _period;		/* 计数器自动重装值，仅自动模式起作用 */
	s_tTmr[_id].Flag = 0;				/* 定时时间到标志 */
	s_tTmr[_id].Mode = TMR_AUTO_MODE;	/* 自动工作模式 */

	//ENABLE_INT();  			/* 开中断 */
}

/*********************************************************************************************************
*	函 数 名: soft_timer_stop
*	功能说明: 停止一个定时器
*	形    参:  	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*	返 回 值: 无
*********************************************************************************************************/
void soft_timer_stop(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		_id = TMR_COUNT - 1;
	}

	//DISABLE_INT();  	/* 关中断 */

	s_tTmr[_id].Count = 0;				/* 实时计数器初值 */
	s_tTmr[_id].Flag = 0;				/* 定时时间到标志 */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 自动工作模式 */

	//ENABLE_INT();  		/* 开中断 */
}

/*********************************************************************************************************
*	函 数 名: soft_timer_check
*	功能说明: 检测定时器是否超时
*	形    参:  	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*				_period : 定时周期，单位1ms
*	返 回 值: 返回 0 表示定时未到， 1表示定时到
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
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
uint32_t get_timer4_tick (void)
{
	return g_iRunTime;
}
/**********************************************************************************************************
** Function name        :   SysTick_Handler
** Descriptions         :   TIM4中断函数处理函数
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void TIM4_IRQHandler (void)
{
	uint8_t i;
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		
		/* 全局运行时间每100ms增1 */
		g_iRunTime++;
		
		/* 每隔100ms，对软件定时器的计数器进行减一操作 */
		for (i = 0; i < TMR_COUNT; i++)
		{
			soft_timer_dec(&s_tTmr[i]);
		}
		
	}
}

