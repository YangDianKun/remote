/*************************************** Copyright (c)******************************************************
** File name            :   main.h
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
#ifndef __MAIN_H
#define __MAIN_H
#include "stm32f10x.h"





/* // 系统状态定义 */
#define SYS_ACTION_STATUS              0      //<! 作业状态
#define SYS_WAIT_WAKE_STATUS           1      //<! 等待唤醒状态



/* // 系统模式定义 */
#define SYS_LOW_POWER_MODE       0      //<! 功耗模式
#define SYS_NORMAL_MODE          1      //<! 正常模式
#define SYS_HIG_POWER_MODE       2      //<! 高性能模式


/* // 系统故障码定义 */
#define SYS_NO_ERR          (uint8_t)(0x00)    //<! 系统正常
#define SYS_KEY_ERR         (uint8_t)(0x01)    //<! 按键故障
#define SYS_WL_ERR          (uint8_t)(0x02)    //<! 无线模块故障



/* // 系统状态参数 */
typedef struct
{
	uint8_t mode;
	uint8_t state;
	uint8_t err;
}SysData_t;



#endif


