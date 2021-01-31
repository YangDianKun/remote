/*************************************** Copyright (c)******************************************************
** File name            :   board.c
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
#include "board.h"
#include "interact.h"




static void _board_gpio_init (void);
static void EXTI_PA0_config (void);


/**********************************************************************************************************
** Function name        :   board_init
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
int board_init(void)
{
	/* NVIC 中断分组 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* 板载GPIO初始化 */
	_board_gpio_init();
	
	LCD_LED_OFF();
	
	/* 精确延时函数初始化 */
	delay_init();
	
	/* 开启  3.3V、5V 供电 */
//	POW_3V3_ON();
//	POW_5V_ON();
//	delay_ms(10);
	
	/* 串口初始化 */
	usart2_init(9600);
	
	/* led初始化 */
	led_init();
	
	/* lcd12864初始化 */
	lcd_init();
	LCD_LED_ON();
	
	/* 按键初始化 */
	hal_InitKey();
	
	/* 射频模块初始化 */
	if (wl_mode_init() < 0)
	{
		//SYS_WL_ERR
	}
	
	/* adc 配置 */
	acd_int();
	
	/* RTC 时钟初始化 */
	hal_rtc_init();
	
	/* PA0 中断配置 */
	EXTI_PA0_config ();
	
	return 0;
}

/**********************************************************************************************************
** Function name        :   EXTI_PA0_config
** Descriptions         :   PA0 配置为外部中断，用于检测按键并处理按键
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void EXTI_PA0_config (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	
	EXTI_DeInit();	
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;             
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;               
	EXTI_Init(&EXTI_InitStructure); 
	

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;        
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;       
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          
    NVIC_Init(&NVIC_InitStructure);
}

/**********************************************************************************************************
** Function name        :   _board_gpio_init
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
static void _board_gpio_init (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// 开启外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	// 推挽输出, LCD背光
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
//    // +3V3 ON 
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
//	GPIO_Init(GPIOB, &GPIO_InitStruct);
//	
//	// +5V ON
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
//	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
}

/**********************************************************************************************************
** Function name        :   board_enter_low_Power
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void board_enter_low_Power(void)
{
	LCD_LED_OFF();                       // 关闭 lcd 背光       
	RTC_ITConfig(RTC_IT_SEC, DISABLE);   // 关闭 RTC 秒中断
	wl_mode_config(SLEEP_MODE);          // 射频模块进入省电模式
	                                     // lcd 低功耗处理
	PWR_EnterStopyMode();                // mcu 进入停机模式 
}

/**********************************************************************************************************
** Function name        :   board_exit_low_Power
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void board_exit_low_Power(void)
{
	/* mcu 退出 stop 模式后，系统时钟默认为内部低精度低速时钟。
	   由于系统要求重新配置为外部高精度时钟 */
	SystemInit();                     // 配置系统时钟
	LCD_LED_ON();                     // 开启 lcd 背光     
	wl_mode_config(NORMANL_MODE);     // 射频模块进入正常模式，准备发送数据
	                                  // lcd 低功耗处理 
	RTC_ITConfig(RTC_IT_SEC, ENABLE); // 开启 RTC 秒中断
}

/**********************************************************************************************************
** Function name        :   board_soft_reset
** Descriptions         :   软件复位
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void board_soft_reset(void)
{
	DISABLE_INT();
	NVIC_SystemReset();
}

/**********************************************************************************************************
** Function name        :   board_soft_reset
** Descriptions         :   软件复位
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void board_unlock(void)
{
	uint8_t cnt = 0;
	lcd_clear();
	lcd_str_gb2312(1, 12, 0, 0, "长按向下键5秒");
	lcd_str_gb2312(4, 48, 0, 0, "解锁");
	lcd_str_gb2312(6, 44, 0, 0, ".....");
	
	while(1)
	{
		delay_ms(1000);
		if (!IsKey3Down())
		{
			lcd_str_gb2312(6, (44 + cnt * 8), 0, 0, " ");
			if (++cnt >= 5)
			{
				delay_ms(10);
				break;
			}
		}
		else
		{
			lcd_str_gb2312(6, 44, 0, 0, ".....");
			cnt = 0;
		}
	}
}


