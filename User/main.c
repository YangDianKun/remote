/*************************************** Copyright (c)******************************************************
** File name            :   main.c
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
#include "main.h"
#include "board.h"
#include "handle.h"
#include "menu.h"
#include "interact.h"
#include "paramter.h"
#include "stdio.h"



/* // 本地变量 */
volatile uint8_t sysStatus;  // 系统状态变量
volatile uint8_t sysMode;    // 系统模式变量
static uint8_t sysIdleCnt;   // 系统空闲计数

/* // 外部变量 */
extern volatile uint8_t rtcSecInterrupt;


/**********************************************************************************************************
** Function name        :   main
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
int main (void)
{	
	/* // 关闭全局中断 */
	DISABLE_INT();
	
	/* // 系统及板载资源初始化 */
	board_init();
	
	/* // 系统参数校验，检测是否有非法篡改的数据 */
	systemParamInit();
	
	/* // 进入菜单设置 */
	EnterMenu();
	
	/* // 人机界面交互初始化 */
	interact_init();
	
	/* // 操作参数初始化 */
	opt_handle_init();
	
	/* // 变量初始化 */	
	sysMode = USER_DATA->systemMode;
	sysStatus = SYS_ACTION_STATUS;
	sysIdleCnt = 0;
	
	/* // 设备解锁 */
	board_unlock();
	
	/* // 开启全局中断 */
	ENABLE_INT();
	
	/* while loop */
	while(1)
	{	
		if (sysStatus == SYS_ACTION_STATUS)
		{
			 /* 按键查询处理，并返回按键状态 */
			if (keys_option_handle())      
			{
				sysIdleCnt = 0;                         // 系统空闲计数清零，重新计数
			}
			
			/* rtc 秒中断 */
			if (rtcSecInterrupt != 0)
			{
				rtcSecInterrupt = 0;
				battery_power_display();                 // 电池电量显示
				digit_time_display();                    // 时钟显示
											
				if (++sysIdleCnt >= 30)                  // 空闲时间阈值判断
				{
					sysIdleCnt = 0;                      // 系统进入低功耗计数清零，重新计数
					switch(sysMode)
					{
						case SYS_LOW_POWER_MODE: // 低功耗模式
							sysStatus = SYS_WAIT_WAKE_STATUS;      
							board_enter_low_Power();               // 系统进入 stop 模式
							break;
						
						case SYS_NORMAL_MODE: // 正常模式
							sysStatus = SYS_ACTION_STATUS;
							LCD_LED_OFF();
							break;
						
						case SYS_HIG_POWER_MODE: // 高性能模式
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

