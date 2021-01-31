/*************************************** Copyright (c)******************************************************
** File name            :   hal_lcd.h
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
#ifndef __LCD_H
#define __LCD_H


#include "stm32f10x.h"





/* LCD �����С */
#define LCD_ROW_SIZE             64
#define LCD_COLUMN_SIZE          128

/* LCD �ַ�����*/
#define LCD_UP_FRAM              0x01
#define LCD_DOWN_FRAM            0x80




//


void lcd_str_gb2312(uint8_t page,uint8_t column,uint8_t reverse,uint8_t framdecor,uint8_t *text);


#endif

