/*************************************** Copyright (c)******************************************************
** File name            :   board.h
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
#ifndef __BOARD_H
#define __BOARD_H


/* 包含硬件驱动 */
#include "hal_config.h"

/* app 头文件 */
//#include "main.h"
#include "menu.h"
#include "lcd12864.h"
#include "lora433.h"



/* 布尔常量值 */
#ifndef TURE
#define TURE 1
#endif

#ifndef FLASE
#define FLASE 0
#endif

#ifndef NULL
#define NULL (void *)0
#endif


/* 设备信息 */
#define DEVICE_MODLE               " DM : TX EQU"
#define FIRMWARE_VERSION           " FW : 1.0.0"
//#define DEVICE_NUMBER              " DN : 01112020"
//#define RF_NUMBER                " RF : D1D11A0844"
#define LEFT_DEV_ID                ((uint8_t)0xff)
#define RIGHT_DEV_ID               ((uint8_t)0)


/* 功能选择 */
//#define BOARD_DEBUG               // 测试
#define MCU_ERR_DISP                // 故障显示


/* 超时设置 */
#define BD_TICKS_PER_MISECOND      450000 					 //@MCU-M1  72MHz  100ms
#define BD_100MS_TIMEOUT           1 * BD_TICKS_PER_MISECOND
#define BD_200MS_TIMEOUT           2 * BD_TICKS_PER_MISECOND
#define BD_300MS_TIMEOUT           3 * BD_TICKS_PER_MISECOND




/* LCD 背光使能控制 */
#define LCD_LED_ON()         GPIO_ResetBits(GPIOB, GPIO_Pin_1);
#define LCD_LED_OFF()        GPIO_SetBits(GPIOB, GPIO_Pin_1);

/* +3V3 使能控制 */
#define POW_3V3_ON()         GPIO_ResetBits(GPIOB, GPIO_Pin_5);
#define POW_3V3_OFF()        GPIO_SetBits(GPIOB, GPIO_Pin_5);

/* +5V 使能控制 */
#define POW_5V_ON()          GPIO_ResetBits(GPIOB, GPIO_Pin_6);
#define POW_5V_OFF()         GPIO_SetBits(GPIOB, GPIO_Pin_6);



/* 函数声明 */
int board_init(void);
void board_gpio_exit_low_power(void);
void board_gpio_enter_low_power(void);
void board_enter_low_Power(void);
void board_exit_low_Power(void);
void board_soft_reset(void);
void board_unlock(void);


#endif
