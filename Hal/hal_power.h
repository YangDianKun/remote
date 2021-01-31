/*************************************** Copyright (c)******************************************************
** File name            :   hal_power.h
** Latest modified Date :   2018-06-03
** Latest Version       :   0.1
** Descriptions         :   mcu 电源管理
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
#ifndef __HAL_POWER_H
#define __HAL_POWER_H
#include "stm32f10x.h"


/* 睡眠模式参数宏定义 */
#define PWR_SLEEPEntry_WFI         ((uint8_t)0x01)   // 中断唤醒
#define PWR_SLEEPEntry_WFE         ((uint8_t)0x02)   // 事件唤醒
#define SysCtrl_SLEEPONEXIT_Set    ((u16)0x0002)     // 此模式不会执行后台任务，即 main while(1){...}
#define SysCtrl_SLEEPDEEP_Set      ((u16)0x0004)     // 立即进入睡眠


/* 函数声明 */
void PWR_EnterSleepMode(uint32_t SysCtrl_Set, uint8_t PWR_SLEEPEntry);
void PWR_EnterStandbyMode(void);
void PWR_EnterStopyMode(void);

#endif

