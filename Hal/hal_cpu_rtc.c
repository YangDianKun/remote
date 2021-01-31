/*************************************** Copyright (c)******************************************************
** File name            :   hal_cpu_rtc.c
** Latest modified Date :   2018-06-03
** Latest Version       :   0.1
** Descriptions         :   cpu�ڲ� rtc ����ģ�� 
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



#define LowSpeedCrystalSelectLSE  0 //�ⲿʱ��ѡ��

/*********************************************************************************************************
*	�� �� ��: hal_rtc_init
*	����˵��: 
*	��    ��:  _
*	�� �� ֵ: ��
*********************************************************************************************************/
uint8_t hal_rtc_init(void)
{
	//����ǲ��ǵ�һ������ʱ��
	uint8_t temp = 0;
	uint32_t temp1;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);        //ʹ��PWR��BKP����ʱ��   
	
	PWR_BackupAccessCmd(ENABLE);        //ʹ�ܺ󱸼Ĵ�������  
	temp1 = BKP_ReadBackupRegister(BKP_DR1);
	
	#if  LowSpeedCrystalSelectLSE   //�ⲿ�ֱ���ѡ��
	if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050)                //��ָ���ĺ󱸼Ĵ����ж�������:��������д���ָ�����ݲ����
	{                                 
		BKP_DeInit();        //��λ��������         
		RCC_LSEConfig(RCC_LSE_ON);        //�����ⲿ���پ���(LSE),ʹ��������پ���
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET && temp < 250)        //���ָ����RCC��־λ�������,�ȴ����پ������
		{
			temp++;
			delay_ms(10);
		}
		if(temp >= 250)return 1;//��ʼ��ʱ��ʧ��,����������            
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);                //����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��   
		RCC_RTCCLKCmd(ENABLE);        //ʹ��RTCʱ��  
		RTC_WaitForLastTask();        //�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_WaitForSynchro();                //�ȴ�RTC�Ĵ���ͬ��  
		RTC_ITConfig(RTC_IT_SEC, ENABLE);                //ʹ��RTC���ж�
		RTC_WaitForLastTask();        //�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_EnterConfigMode();/// ��������        
		RTC_SetPrescaler(32767); //����RTCԤ��Ƶ��ֵ
		RTC_WaitForLastTask();        //�ȴ����һ�ζ�RTC�Ĵ�����д�������
		//RTC_Set(2015,1,14,17,42,55);  //����ʱ��        
		RTC_ExitConfigMode(); //�˳�����ģʽ  
		BKP_WriteBackupRegister(BKP_DR1, 0X5050);        //��ָ���ĺ󱸼Ĵ�����д���û���������
	}
	else//ϵͳ������ʱ
	{

		RTC_WaitForSynchro();        //�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_ITConfig(RTC_IT_SEC, ENABLE);        //ʹ��RTC���ж�
		RTC_WaitForLastTask();        //�ȴ����һ�ζ�RTC�Ĵ�����д�������
	}               
	#else                             //�ڲ����پ���ѡ��   40KHZ
	if (BKP_ReadBackupRegister(BKP_DR1) != 0x5040)                //��ָ���ĺ󱸼Ĵ����ж�������:��������д���ָ�����ݲ����
	{                                 
		BKP_DeInit();        //��λ��������         
		RCC_LSICmd(ENABLE);        //�����ⲿ���پ���(LSE),ʹ��������پ���
		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET && temp < 250)        //���ָ����RCC��־λ�������,�ȴ����پ������
		{
			temp++;
			delay_ms(10);
		}
		if(temp >= 250)return 1;//��ʼ��ʱ��ʧ��,����������            
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);                //����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��   
		RCC_RTCCLKCmd(ENABLE);        //ʹ��RTCʱ��  
		RTC_WaitForLastTask();        //�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_WaitForSynchro();                //�ȴ�RTC�Ĵ���ͬ��  
		RTC_ITConfig(RTC_IT_SEC, ENABLE);                //ʹ��RTC���ж�
		RTC_WaitForLastTask();        //�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_EnterConfigMode();/// ��������        
		RTC_SetPrescaler(39999); //����RTCԤ��Ƶ��ֵ
		RTC_WaitForLastTask();        //�ȴ����һ�ζ�RTC�Ĵ�����д�������
		//RTC_Set(2015,1,14,17,42,55);  //����ʱ��        
		RTC_ExitConfigMode(); //�˳�����ģʽ  
		BKP_WriteBackupRegister(BKP_DR1, 0X5040);        //��ָ���ĺ󱸼Ĵ�����д���û���������
	}
	else//ϵͳ������ʱ
	{
		RCC_LSICmd(ENABLE);        //�����ⲿ���پ���(LSE),ʹ��������پ���
		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET && temp < 250)        //���ָ����RCC��־λ�������,�ȴ����پ������
		{
			temp++;
			delay_ms(10);
		}
		if(temp >= 250)return 1;//��ʼ��ʱ��ʧ��,����������            
		RTC_WaitForLastTask();        //�ȴ����һ�ζ�RTC�Ĵ�����д�������        
		RTC_WaitForSynchro();        //�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_ITConfig(RTC_IT_SEC, ENABLE);        //ʹ��RTC���ж�
		RTC_WaitForLastTask();        //�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_EnterConfigMode();/// ��������        
		RTC_SetPrescaler(39999); //����RTCԤ��Ƶ��ֵ
		RTC_WaitForLastTask();        //�ȴ����һ�ζ�RTC�Ĵ�����д�������                        
	}               
	#endif

	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;        
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;       
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          
	NVIC_Init(&NVIC_InitStructure);                                                  
    
	return 0; //ok
}




