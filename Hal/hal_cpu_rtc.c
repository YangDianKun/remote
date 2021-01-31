/*************************************** Copyright (c)******************************************************
** File name            :   hal_cpu_rtc.c
** Latest modified Date :   2018-06-03
** Latest Version       :   0.1
** Descriptions         :   cpu内部 rtc 操作模块 
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
#include "hal_cpu_rtc.h"
#include "hal_config.h"



#define LowSpeedCrystalSelectLSE  0 //外部时钟选择

/*********************************************************************************************************
*	函 数 名: hal_rtc_init
*	功能说明: 
*	形    参:  _
*	返 回 值: 无
*********************************************************************************************************/
uint8_t hal_rtc_init(void)
{
	//检查是不是第一次配置时钟
	uint8_t temp = 0;
	uint32_t temp1;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);        //使能PWR和BKP外设时钟   
	
	PWR_BackupAccessCmd(ENABLE);        //使能后备寄存器访问  
	temp1 = BKP_ReadBackupRegister(BKP_DR1);
	
	#if  LowSpeedCrystalSelectLSE   //外部手表晶振选择
	if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050)                //从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
	{                                 
		BKP_DeInit();        //复位备份区域         
		RCC_LSEConfig(RCC_LSE_ON);        //设置外部低速晶振(LSE),使用外设低速晶振
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET && temp < 250)        //检查指定的RCC标志位设置与否,等待低速晶振就绪
		{
			temp++;
			delay_ms(10);
		}
		if(temp >= 250)return 1;//初始化时钟失败,晶振有问题            
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);                //设置RTC时钟(RTCCLK),选择LSE作为RTC时钟   
		RCC_RTCCLKCmd(ENABLE);        //使能RTC时钟  
		RTC_WaitForLastTask();        //等待最近一次对RTC寄存器的写操作完成
		RTC_WaitForSynchro();                //等待RTC寄存器同步  
		RTC_ITConfig(RTC_IT_SEC, ENABLE);                //使能RTC秒中断
		RTC_WaitForLastTask();        //等待最近一次对RTC寄存器的写操作完成
		RTC_EnterConfigMode();/// 允许配置        
		RTC_SetPrescaler(32767); //设置RTC预分频的值
		RTC_WaitForLastTask();        //等待最近一次对RTC寄存器的写操作完成
		//RTC_Set(2015,1,14,17,42,55);  //设置时间        
		RTC_ExitConfigMode(); //退出配置模式  
		BKP_WriteBackupRegister(BKP_DR1, 0X5050);        //向指定的后备寄存器中写入用户程序数据
	}
	else//系统继续计时
	{

		RTC_WaitForSynchro();        //等待最近一次对RTC寄存器的写操作完成
		RTC_ITConfig(RTC_IT_SEC, ENABLE);        //使能RTC秒中断
		RTC_WaitForLastTask();        //等待最近一次对RTC寄存器的写操作完成
	}               
	#else                             //内部低速晶振选择   40KHZ
	if (BKP_ReadBackupRegister(BKP_DR1) != 0x5040)                //从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
	{                                 
		BKP_DeInit();        //复位备份区域         
		RCC_LSICmd(ENABLE);        //设置外部低速晶振(LSE),使用外设低速晶振
		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET && temp < 250)        //检查指定的RCC标志位设置与否,等待低速晶振就绪
		{
			temp++;
			delay_ms(10);
		}
		if(temp >= 250)return 1;//初始化时钟失败,晶振有问题            
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);                //设置RTC时钟(RTCCLK),选择LSE作为RTC时钟   
		RCC_RTCCLKCmd(ENABLE);        //使能RTC时钟  
		RTC_WaitForLastTask();        //等待最近一次对RTC寄存器的写操作完成
		RTC_WaitForSynchro();                //等待RTC寄存器同步  
		RTC_ITConfig(RTC_IT_SEC, ENABLE);                //使能RTC秒中断
		RTC_WaitForLastTask();        //等待最近一次对RTC寄存器的写操作完成
		RTC_EnterConfigMode();/// 允许配置        
		RTC_SetPrescaler(39999); //设置RTC预分频的值
		RTC_WaitForLastTask();        //等待最近一次对RTC寄存器的写操作完成
		//RTC_Set(2015,1,14,17,42,55);  //设置时间        
		RTC_ExitConfigMode(); //退出配置模式  
		BKP_WriteBackupRegister(BKP_DR1, 0X5040);        //向指定的后备寄存器中写入用户程序数据
	}
	else//系统继续计时
	{
		RCC_LSICmd(ENABLE);        //设置外部低速晶振(LSE),使用外设低速晶振
		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET && temp < 250)        //检查指定的RCC标志位设置与否,等待低速晶振就绪
		{
			temp++;
			delay_ms(10);
		}
		if(temp >= 250)return 1;//初始化时钟失败,晶振有问题            
		RTC_WaitForLastTask();        //等待最近一次对RTC寄存器的写操作完成        
		RTC_WaitForSynchro();        //等待最近一次对RTC寄存器的写操作完成
		RTC_ITConfig(RTC_IT_SEC, ENABLE);        //使能RTC秒中断
		RTC_WaitForLastTask();        //等待最近一次对RTC寄存器的写操作完成
		RTC_EnterConfigMode();/// 允许配置        
		RTC_SetPrescaler(39999); //设置RTC预分频的值
		RTC_WaitForLastTask();        //等待最近一次对RTC寄存器的写操作完成                        
	}               
	#endif

	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;        
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;       
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          
	NVIC_Init(&NVIC_InitStructure);                                                  
    
	return 0; //ok
}




