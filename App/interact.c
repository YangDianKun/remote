/*************************************** Copyright (c)******************************************************
** File name            :   interact.c
** Latest modified Date :   2019-12-03
** Latest Version       :   0.1
** Descriptions         :   主文件，包含应用代码
**
**--------------------------------------------------------------------------------------------------------
** Created by           :   YangDianKun
** Created date         :   
** Version              :   1.0
** Descriptions         :   The original version
**
**--------------------------------------------------------------------------------------------------------
** Copyright            :  
** Author Email         :   1163101403@qq.com
**********************************************************************************************************/
#include "interact.h"
#include "board.h"
#include "icon.h"
#include "paramter.h"
#include "main.h"



void batVolIcoDisplay(uint8_t vIndex);

/**********************************************************************************************************
** Function name        :   logo_display
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
static void logo_display(uint8_t refreshMS, uint8_t reverse, const uint8_t *bmp)
{
	uint8_t i, j;
	uint8_t temp;
	         
	for(j = 0; j < 128; j++)
	{
		for(i = 0; i < 8; i++)
		{
			lcd_address(i, j);
			if (reverse)
			{
				temp = ~bmp[i * 128 + j];
			}
			else
			{
				temp = bmp[i * 128 + j];
			}
			transfer_data_lcd(temp);   
		}
		
		if (refreshMS != 0)
			delay_ms(refreshMS);
	}  
}


/**********************************************************************************************************
** Function name        :   interface_display
** Descriptions         :   界面显示
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void interface_display(void)
{
	uint8_t i;
	uint8_t idStr[6];
	uint16_t id;

	/* // 工作模式显示 */
	switch (USER_DATA->systemMode)
	{
		case SYS_LOW_POWER_MODE: 
			lcd_str_5x8(0, 60, 0, 0, " L");
			break;
		case SYS_NORMAL_MODE: 
			lcd_str_5x8(0, 60, 0, 0, " N");
			break;
		case SYS_HIG_POWER_MODE: 
			lcd_str_5x8(0, 60, 0, 0, " H");
			break;
		default:break;
	}
	
	/* // RF模式发射功率显示 */
	switch (gwlCurrentParam.option & 0x03)
	{
		case 0: 
			lcd_str_5x8(0, 0, 0, 0, "20"); // 20dBm
			break; 
		case 1:
			lcd_str_5x8(0, 0, 0, 0, "17"); // 17dBm			
			break; 
		case 2: 
			lcd_str_5x8(0, 0, 0, 0, "14"); // 14dBm
			break; 
		case 3: 
			lcd_str_5x8(0, 0, 0, 0, "10"); // 10dBm
			break; 
		default:break;
	}
	
	/* // 左站、右站显示 */
	switch (USER_DATA->devID)
	{
		case LEFT_DEV_ID:
			lcd_str_gb2312(6, 64, 1, 0, "左发射机");
			break;
		case RIGHT_DEV_ID:
		lcd_str_gb2312(6, 64, 1, 0, "右发射机");
			break;
		default:
			lcd_str_gb2312(6, 64, 1, 0, "--发射机");
			break;
	}
	
	/* // 电池电量图标显示 */
	batVolIcoDisplay(4);

	
	/* // 射频模块地址显示 */
	lcd_str_5x8(0, 84, 0, 0, "id:----");
	id = (gwlCurrentParam.addr_h << 8 ) | gwlCurrentParam.addr_l;
	snprintf((char *)idStr, 5, "%x", id);
	lcd_str_5x8(0,102,0,0,idStr);

	/* // 运行时间显示 */
	lcd_str_digit1624(2, 0, 0, "00:00:00");
	digit_time_display();
	
	
	/* // 画底部分割线 */
	lcd_address(6, 0);
	for(i = 0; i < 60; i++)
	{
		transfer_data_lcd(0x01);
	}
}

/**********************************************************************************************************
** Function name        :   digit_time_display
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void digit_time_display(void)
{
	uint32_t rtc_time;
	uint8_t hour, minute, second;
	uint8_t h_str[3];
	uint8_t m_str[3];
	uint8_t s_str[3];
	
	
	rtc_time = RTC_GetCounter();
	second = rtc_time % 60;         // 秒
	minute = rtc_time / 60 % 60;    // 分
	hour = rtc_time / 3600 % 24;    // 时
	
	snprintf((char*)s_str, 3, "%2u", second);
	snprintf((char*)m_str, 3, "%2u", minute);
	snprintf((char*)h_str, 3, "%2u", hour);
	
	if(s_str[0] == ' ') s_str[0] = '0';
	if(m_str[0] == ' ') m_str[0] = '0';
	if(h_str[0] == ' ') h_str[0] = '0';
	
	lcd_str_digit1624(2, 0, 0, h_str);
	lcd_str_digit1624(2, 48, 0, m_str);
	lcd_str_digit1624(2, 96, 0, s_str);
	
}

/**********************************************************************************************************
** Function name        :   boot_animation
** Descriptions         :   开机动画
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void boot_animation(void)
{
	/* 显示logo */
	logo_display(1, 1, &bmp_12864_logo[0]);
	delay_ms(300);
	logo_display(1, 0, &bmp_12864_logo[0]);
	delay_ms(1000);

}
/**********************************************************************************************************
** Function name        :   interact_init
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
int8_t interact_init (void)
{
	/* 开机动画 */
	boot_animation();
	
	/* 清屏 */
	lcd_clear();
	
	/* 界面显示 */
	interface_display();
	
	return 0;
}

/**********************************************************************************************************
** Function name        :   battery_power_display
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void battery_power_display(void)
{
	uint32_t adcValue = 0;
	//static float batMinVoltage = 4.2, batPrevVoltage = 0.0;
	float batVoltage = 0.0;
	//uint8_t batVolStr[5];
	
	adcValue = getAdcValue(1); // ADC 通道 1 为电池检测 IO， PA1
	batVoltage = 6.6 * adcValue / 4096;
	
//	/* 幅值滤波 */
//	if ()
//	{
//	
//	}
//	
//	/* 测量电池电压最小值 */
//	if (batMinVoltage > batVoltage)
//	{
//		batMinVoltage = batVoltage;
//	}
//	
#if 0
	/* 显示电压值，用于调试 */
	snprintf((char *)batVolStr, 5, "%1.2f", batVoltage);
	lcd_str_5x8(0, 57, 0, 0, batVolStr);
#endif

	/* 电压值转换为可视电量图标 */
	if (batVoltage > BAT_LARGE_VALUE)        // 100
	{
		batVolIcoDisplay(4);
	}
	else if (batVoltage <= BAT_LARGE_VALUE && batVoltage > BAT_BIG_VALUE)   // 75
	{
		batVolIcoDisplay(3);
	}
	else if (batVoltage <= BAT_BIG_VALUE && batVoltage > BAT_MIDDLE_VALUE)   // 25
	{
		batVolIcoDisplay(2);
	}
	else if (batVoltage <= BAT_MIDDLE_VALUE && batVoltage > BAT_SMALL_VALUE)
	{
		batVolIcoDisplay(1);
	}
	else
	{
		batVolIcoDisplay(0);
		// 关机
		//		POW_3V3_OFF();  // 关闭 3.3V 供电
		//		POW_5V_OFF();   // 关闭 5V 供电
		//		PWR_EnterStandbyMode();
	}
	
}

/**********************************************************************************************************
** Function name        :   battery_power_display
** Descriptions         :   
** parameters           :   vIndex: 0,1,2,3,4
** Returned value       :   无
***********************************************************************************************************/
void batVolIcoDisplay(uint8_t vIndex)
{
	uint8_t i,j;
	
	if (vIndex > 4) return ;
	
	lcd_address(0, 24);
	transfer_data_lcd(0xE7);
	transfer_data_lcd(0x81);
	for (i = 0; i < 3; i++)
	{
		lcd_address(0, 30 + i * 7);
		for (j = 0; j < 3; j++)
		{
			transfer_data_lcd(0x81);
		}
	}
	
	for (i = 0; i < vIndex; i++)
	{
		lcd_address(0, 26 + i * 7);
		for (j = 0; j < 4; j++)
		{
			transfer_data_lcd(0xBD);  
		}
	}
	for (i = i; i < 4; i++)
	{
		lcd_address(0, 26 + i * 7);
		for (j = 0; j < 4; j++)
		{
			transfer_data_lcd(0x81);  
		}
	}
	
	transfer_data_lcd(0x81);
	transfer_data_lcd(0xE7);
	transfer_data_lcd(0x3C);
}


// 电池调试
#if 1
void battery_debug(void)
{
	static uint32_t adcMaxV = 0, adcMinV = 4096, adcValue = 0;
	float adcV;
	uint8_t adcStr[6];
	
	adcValue = getAdcValue(1);
	
	if (adcMaxV < adcValue)
	{
		adcMaxV = adcValue;
	}
	
	if (adcMinV > adcValue)
	{
		adcMinV = adcValue;
	}
	
	adcV = 6.6 * adcMaxV / 4096.0;
	snprintf((char*)adcStr, 6, "%1.3f", adcV);
	lcd_str_5x8(5, 4, 0, LCD_NO_FRAM, adcStr);
	snprintf((char*)adcStr, 6, "%4u", adcMaxV);
	lcd_str_5x8(3, 4, 0, LCD_NO_FRAM, adcStr);
	lcd_str_5x8(1, 4, 0, LCD_NO_FRAM, "MAX:");
	
	
	adcV = 6.6 * adcValue / 4096.0;
	snprintf((char*)adcStr, 6, "%1.3f", adcV);
	lcd_str_5x8(5, 44, 0, LCD_NO_FRAM, adcStr);
	snprintf((char*)adcStr, 6, "%4u", adcValue);
	lcd_str_5x8(3, 44, 0, LCD_NO_FRAM, adcStr);
	lcd_str_5x8(1, 44, 0, LCD_NO_FRAM, "ACT:");

	
	adcV = 6.6 * adcMinV / 4096.0;
	snprintf((char*)adcStr, 6, "%1.3f", adcV);
	lcd_str_5x8(5, 84, 0, LCD_NO_FRAM, adcStr);
	snprintf((char*)adcStr, 6, "%4u", adcMinV);
	lcd_str_5x8(3, 84, 0, LCD_NO_FRAM, adcStr);
	lcd_str_5x8(1, 84, 0, LCD_NO_FRAM, "MIN:");
}
#endif

