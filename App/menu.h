/*************************************** Copyright (c)******************************************************
** File name            :   menu.h
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
#ifndef __MENU_H
#define __MENU_H
#include "stm32f10x.h"
//#include "stdio.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/ 
#define  MAX_MENU_LEVELS                        4           // 菜单最大级数
//#define  MAX_DISPLAY_MEUN_INTEMS_LINE           3           // 菜单项最大显示行数



/* Module private variables --------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))



int8_t MenuInit (void);
void OptionMenu (void);
void EnterMenu(void);



#endif

