/*************************************** Copyright (c)******************************************************
** File name            :   hal_key.h
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
#ifndef __HAL_KEY_H
#define __HAL_KEY_H

#include "stm32f10x.h"




#define KEY_COUNT        12	   					/* 按键个数, 12个独立建 */




/* 	定义键值代码, 必须按如下次序定时每个键的按下、弹起和长按事件 */
typedef enum
{
	KEY_NONE = 0,			/* 0 表示按键事件 */

	KEY_1_DOWN,				/* 1键按下 */
	KEY_2_DOWN,				/* 2键按下 */
	KEY_3_DOWN,				/* 3键按下 */
	KEY_4_DOWN,				/* 4键按下 */
	KEY_5_DOWN,				/* 5键按下 */
	KEY_6_DOWN,				/* 6键按下 */
	KEY_7_DOWN,				/* 7键按下 */
	KEY_8_DOWN,				/* 8键按下 */
	KEY_9_DOWN,				/* 9键按下 */
	KEY_10_DOWN,			/* 10键按下 */	
	KEY_11_DOWN,			/* 11键按下 */
	KEY_12_DOWN,			/* 12键按下 */

}KEY_ENUM;



/* 供外部调用的函数声明 */
int8_t hal_InitKey(void);
uint8_t hal_GetKeyValue(void);



uint8_t IsKey1Down(void);
uint8_t IsKey2Down(void);
uint8_t IsKey3Down(void);
uint8_t IsKey4Down(void);
uint8_t IsKey5Down(void);
uint8_t IsKey6Down(void);
uint8_t IsKey7Down(void);
uint8_t IsKey8Down(void);
uint8_t IsKey9Down(void);
uint8_t IsKey10Down(void);
uint8_t IsKey11Down(void);     
uint8_t IsKey12Down(void);


#endif
	
/*********************************** (C) COPYRIGHT 2018 YangDianKun ************************END OF FILE****/





