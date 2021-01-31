/*************************************** Copyright (c)******************************************************
** File name            :   hal_key.h
** Latest modified Date :   2018-06-03
** Latest Version       :   0.1
** Descriptions         :   ���ļ�������Ӧ�ô���
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




#define KEY_COUNT        12	   					/* ��������, 12�������� */




/* 	�����ֵ����, ���밴���´���ʱÿ�����İ��¡�����ͳ����¼� */
typedef enum
{
	KEY_NONE = 0,			/* 0 ��ʾ�����¼� */

	KEY_1_DOWN,				/* 1������ */
	KEY_2_DOWN,				/* 2������ */
	KEY_3_DOWN,				/* 3������ */
	KEY_4_DOWN,				/* 4������ */
	KEY_5_DOWN,				/* 5������ */
	KEY_6_DOWN,				/* 6������ */
	KEY_7_DOWN,				/* 7������ */
	KEY_8_DOWN,				/* 8������ */
	KEY_9_DOWN,				/* 9������ */
	KEY_10_DOWN,			/* 10������ */	
	KEY_11_DOWN,			/* 11������ */
	KEY_12_DOWN,			/* 12������ */

}KEY_ENUM;



/* ���ⲿ���õĺ������� */
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





