/*************************************** Copyright (c)******************************************************
** File name            :   interact.h
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
#ifndef __INTERACT_H  //interact
#define __INTERACT_H

#include "stm32f10x.h"


//#define BAT_FULL_VALUE              ((float)4.2)    // 电池满电电压 
#define BAT_LARGE_VALUE             ((float)3.9)    // 100%
#define BAT_BIG_VALUE               ((float)3.7)    // 75%
#define BAT_MIDDLE_VALUE            ((float)3.5)    // 50%
#define BAT_SMALL_VALUE             ((float)3.3)    // 25%
//#define BAT_EMPTY_VALUE             ((float)3.0)    // 电池空电电压 
	



/* 函数声明 */
int8_t interact_init (void);
void boot_animation(void);
void digit_time_display(void);
void interface_display(void);
void battery_power_display(void);


void battery_debug(void); // 调试用

#endif

