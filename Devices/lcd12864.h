/*************************************** Copyright (c)******************************************************
** File name            :   hal_lcd.h
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
#ifndef __LCD12864_H
#define __LCD12864_H


#include "stm32f10x.h"







/* LCD坐标大小 */
#define LCD_ROW_SIZE             64
#define LCD_COLUMN_SIZE          128

/* LCD 字符修饰*/
#define LCD_NO_FRAM              0
#define LCD_UP_FRAM              0x01
#define LCD_DOWN_FRAM            0x80


/* LCD端口引脚定义 */
#define LCD_CS_PIN         GPIO_Pin_11
#define LCD_RS_PIN         GPIO_Pin_9
#define LCD_SCK_PIN        GPIO_Pin_8
#define LCD_SDA_PIN        GPIO_Pin_7
#define LCD_RESET_PIN      GPIO_Pin_10
#define LCD_PORT           GPIOB


/* LCD字库芯片端口引脚定义 */
#define ROM_SCK_PIN        GPIO_Pin_13
#define ROM_IN_PIN         GPIO_Pin_15
#define ROM_OUT_GET_PIN    GPIO_Pin_14        // 上拉输入
#define ROM_CS_PIN         GPIO_Pin_12
#define ROM_PORT           GPIOB



#define LCD_ROM_PORT_RCC   RCC_APB2Periph_GPIOB


/* LCD端口引脚操作 */
#define LCD_CS_H()                GPIO_SetBits(LCD_PORT, LCD_CS_PIN)
#define LCD_CS_L()                GPIO_ResetBits(LCD_PORT, LCD_CS_PIN)

#define LCD_RS_H()                GPIO_SetBits(LCD_PORT, LCD_RS_PIN)
#define LCD_RS_L()                GPIO_ResetBits(LCD_PORT, LCD_RS_PIN)

#define LCD_SCK_H()               GPIO_SetBits(LCD_PORT, LCD_SCK_PIN)
#define LCD_SCK_L()               GPIO_ResetBits(LCD_PORT, LCD_SCK_PIN)

#define LCD_SDA_H()               GPIO_SetBits(LCD_PORT, LCD_SDA_PIN)
#define LCD_SDA_L()               GPIO_ResetBits(LCD_PORT, LCD_SDA_PIN)

#define LCD_RESET_H()             GPIO_SetBits(LCD_PORT, LCD_RESET_PIN)
#define LCD_RESET_L()             GPIO_ResetBits(LCD_PORT, LCD_RESET_PIN)


/* LCD字库芯片端口引脚操作 */
#define ROM_SCK_H()               GPIO_SetBits(ROM_PORT, ROM_SCK_PIN)
#define ROM_SCK_L()               GPIO_ResetBits(ROM_PORT, ROM_SCK_PIN)

// ROM输入引脚，单片机输出
#define ROM_IN_H()                GPIO_SetBits(ROM_PORT, ROM_IN_PIN)
#define ROM_IN_L()                GPIO_ResetBits(ROM_PORT, ROM_IN_PIN)

// ROM输出引脚，单片机读取
#define GET_ROM_OUT()             GPIO_ReadInputDataBit(ROM_PORT, ROM_OUT_GET_PIN)

#define ROM_CS_H()                GPIO_SetBits(ROM_PORT, ROM_CS_PIN)
#define ROM_CS_L()                GPIO_ResetBits(ROM_PORT, ROM_CS_PIN)




/* 函数声明 */
int8_t lcd_init(void);
void lcd_gpio_initial(void);
void lcd_clear(void);
void lcd_str_5x8(uint8_t page,uint8_t column,uint8_t reverse,uint8_t framdecor,uint8_t *text);
void lcd_str_gb2312(uint8_t page,uint8_t column,uint8_t reverse,uint8_t framdecor,uint8_t *text);

void lcd_str_5x8_line(uint8_t line,uint8_t reverse,uint8_t framdecor,uint8_t *str);
void lcd_str_gb2312_line(uint8_t line,uint8_t reverse,uint8_t framdecor,uint8_t *str);
//void lcd_str_gb24 (uint8_t page, uint8_t colum, uint8_t reverse, uint8_t *str);
void lcd_str_digit1624 (uint8_t page, uint8_t colum, uint8_t reverse, uint8_t *str);
void lcd_area_clear(uint8_t y, uint8_t x, uint8_t ySize, uint8_t xSize);
void lcd_graphic_128x64(uint8_t reverse, const uint8_t *bmp);
void lcd_graphic_any_size (uint8_t y, uint8_t x, uint8_t ySize, uint8_t xSize, uint8_t reverse, const uint8_t * ico);

void transfer_data_lcd(uint8_t  data);
void lcd_address(uint8_t page,uint8_t column);

	
#endif
	
/*********************************** (C) COPYRIGHT 2018 YangDianKun ************************END OF FILE****/

