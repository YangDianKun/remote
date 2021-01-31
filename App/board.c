/*************************************** Copyright (c)******************************************************
** File name            :   board.c
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
#include "board.h"
#include "interact.h"




static void _board_gpio_init (void);
static void EXTI_PA0_config (void);


/**********************************************************************************************************
** Function name        :   board_init
** Descriptions         :   
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
int board_init(void)
{
	/* NVIC �жϷ��� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* ����GPIO��ʼ�� */
	_board_gpio_init();
	
	LCD_LED_OFF();
	
	/* ��ȷ��ʱ������ʼ�� */
	delay_init();
	
	/* ����  3.3V��5V ���� */
//	POW_3V3_ON();
//	POW_5V_ON();
//	delay_ms(10);
	
	/* ���ڳ�ʼ�� */
	usart2_init(9600);
	
	/* led��ʼ�� */
	led_init();
	
	/* lcd12864��ʼ�� */
	lcd_init();
	LCD_LED_ON();
	
	/* ������ʼ�� */
	hal_InitKey();
	
	/* ��Ƶģ���ʼ�� */
	if (wl_mode_init() < 0)
	{
		//SYS_WL_ERR
	}
	
	/* adc ���� */
	acd_int();
	
	/* RTC ʱ�ӳ�ʼ�� */
	hal_rtc_init();
	
	/* PA0 �ж����� */
	EXTI_PA0_config ();
	
	return 0;
}

/**********************************************************************************************************
** Function name        :   EXTI_PA0_config
** Descriptions         :   PA0 ����Ϊ�ⲿ�жϣ����ڼ�ⰴ����������
** parameters           :   ��
** Returned value       :   ��
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
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
static void _board_gpio_init (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// ��������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	// �������, LCD����
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
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
void board_enter_low_Power(void)
{
	LCD_LED_OFF();                       // �ر� lcd ����       
	RTC_ITConfig(RTC_IT_SEC, DISABLE);   // �ر� RTC ���ж�
	wl_mode_config(SLEEP_MODE);          // ��Ƶģ�����ʡ��ģʽ
	                                     // lcd �͹��Ĵ���
	PWR_EnterStopyMode();                // mcu ����ͣ��ģʽ 
}

/**********************************************************************************************************
** Function name        :   board_exit_low_Power
** Descriptions         :   
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
void board_exit_low_Power(void)
{
	/* mcu �˳� stop ģʽ��ϵͳʱ��Ĭ��Ϊ�ڲ��;��ȵ���ʱ�ӡ�
	   ����ϵͳҪ����������Ϊ�ⲿ�߾���ʱ�� */
	SystemInit();                     // ����ϵͳʱ��
	LCD_LED_ON();                     // ���� lcd ����     
	wl_mode_config(NORMANL_MODE);     // ��Ƶģ���������ģʽ��׼����������
	                                  // lcd �͹��Ĵ��� 
	RTC_ITConfig(RTC_IT_SEC, ENABLE); // ���� RTC ���ж�
}

/**********************************************************************************************************
** Function name        :   board_soft_reset
** Descriptions         :   �����λ
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
void board_soft_reset(void)
{
	DISABLE_INT();
	NVIC_SystemReset();
}

/**********************************************************************************************************
** Function name        :   board_soft_reset
** Descriptions         :   �����λ
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
void board_unlock(void)
{
	uint8_t cnt = 0;
	lcd_clear();
	lcd_str_gb2312(1, 12, 0, 0, "�������¼�5��");
	lcd_str_gb2312(4, 48, 0, 0, "����");
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


