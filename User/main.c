/*************************************** Copyright (c)******************************************************
** File name            :   main.c
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
#include "main.h"
#include "board.h"
#include "handle.h"
#include "menu.h"
#include "interact.h"
#include "paramter.h"
#include "stdio.h"



/* // ���ر��� */
volatile uint8_t sysStatus;  // ϵͳ״̬����
volatile uint8_t sysMode;    // ϵͳģʽ����
static uint8_t sysIdleCnt;   // ϵͳ���м���

/* // �ⲿ���� */
extern volatile uint8_t rtcSecInterrupt;


/**********************************************************************************************************
** Function name        :   main
** Descriptions         :   
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
int main (void)
{	
	/* // �ر�ȫ���ж� */
	DISABLE_INT();
	
	/* // ϵͳ��������Դ��ʼ�� */
	board_init();
	
	/* // ϵͳ����У�飬����Ƿ��зǷ��۸ĵ����� */
	systemParamInit();
	
	/* // ����˵����� */
	EnterMenu();
	
	/* // �˻����潻����ʼ�� */
	interact_init();
	
	/* // ����������ʼ�� */
	opt_handle_init();
	
	/* // ������ʼ�� */	
	sysMode = USER_DATA->systemMode;
	sysStatus = SYS_ACTION_STATUS;
	sysIdleCnt = 0;
	
	/* // �豸���� */
	board_unlock();
	
	/* // ����ȫ���ж� */
	ENABLE_INT();
	
	/* while loop */
	while(1)
	{	
		if (sysStatus == SYS_ACTION_STATUS)
		{
			 /* ������ѯ���������ذ���״̬ */
			if (keys_option_handle())      
			{
				sysIdleCnt = 0;                         // ϵͳ���м������㣬���¼���
			}
			
			/* rtc ���ж� */
			if (rtcSecInterrupt != 0)
			{
				rtcSecInterrupt = 0;
				battery_power_display();                 // ��ص�����ʾ
				digit_time_display();                    // ʱ����ʾ
											
				if (++sysIdleCnt >= 30)                  // ����ʱ����ֵ�ж�
				{
					sysIdleCnt = 0;                      // ϵͳ����͹��ļ������㣬���¼���
					switch(sysMode)
					{
						case SYS_LOW_POWER_MODE: // �͹���ģʽ
							sysStatus = SYS_WAIT_WAKE_STATUS;      
							board_enter_low_Power();               // ϵͳ���� stop ģʽ
							break;
						
						case SYS_NORMAL_MODE: // ����ģʽ
							sysStatus = SYS_ACTION_STATUS;
							LCD_LED_OFF();
							break;
						
						case SYS_HIG_POWER_MODE: // ������ģʽ
							sysStatus = SYS_ACTION_STATUS;
							LCD_LED_ON();
							break;
					}
				}
			}
		}
	}//while end
}//main end






#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1)
  {
  }
}
#endif

