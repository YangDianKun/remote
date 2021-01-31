/*************************************** Copyright (c)******************************************************
** File name            :   hal_lcd.c
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
#include "lcd12864.h"
#include "hal_delay.h"
#include "fonts.h"




//static void transfer_command_lcd(int data1);
//void send_command_to_ROM( uint8_t datu );
//void get_and_write_16x16(uint32_t fontaddr,uint8_t reverse,uint8_t page,uint8_t column);
//void get_and_write_8x16(uint32_t fontaddr,uint8_t reverse,uint8_t page,uint8_t column);
//void get_and_write_5x8(uint32_t fontaddr,uint8_t reverse,uint8_t page,uint8_t column) ;


/**********************************************************************************************************
** Function name        :   
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
static void lcdDelayUS(uint32_t x)
{
	delay_us(x);
}

/**********************************************************************************************************
** Function name        :   transfer_command_lcd
** Descriptions         :   写指令到LCD模块
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
static void transfer_command_lcd(uint8_t data)   
{
	uint8_t i;

	LCD_CS_L();
	LCD_RS_L();

	for(i = 0; i < 8; i++)
	{
		LCD_SCK_L();
		if(data & 0x80)
		{
			LCD_SDA_H();
		}
		else 
		{
			LCD_SDA_L();
		}

		LCD_SCK_H();
		data <<= 1;
	}
	LCD_CS_H();
}

/**********************************************************************************************************
** Function name        :   lcd_address
** Descriptions         :   设置液晶屏的显示地址（page，column）
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void lcd_address(uint8_t page,uint8_t column)
{
	transfer_command_lcd(0xb0 + page);   				 //设置页地址。每页是8行。一个画面的64行被分成8个页。我们平常所说的第1页，在LCD驱动IC里是第0页，所以在这里减去1
	transfer_command_lcd(((column >> 4) & 0x0f) + 0x10); //设置列地址的高4位
	transfer_command_lcd(column & 0x0f);				 //设置列地址的低4位
}

/**********************************************************************************************************
** Function name        :   transfer_data_lcd
** Descriptions         :   写数据到LCD模块
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void transfer_data_lcd(uint8_t data)
{
	uint8_t i;

	LCD_CS_L();
	LCD_RS_H();

	for(i = 0; i < 8; i++)
	{
		LCD_SCK_L();
		if(data & 0x80)
		{
			LCD_SDA_H();
		}
		else
		{
			LCD_SDA_L();
		}
		LCD_SCK_H();
		data <<= 1;
	}
	LCD_CS_H();
}

/**********************************************************************************************************
** Function name        :   lcd_gpio_initial
** Descriptions         :   初始化PA口为输出口.并使能这个IO口的时钟	
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void lcd_gpio_initial(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(LCD_ROM_PORT_RCC, ENABLE);

	//推完输出模式
	GPIO_InitStruct.GPIO_Pin = (LCD_CS_PIN | LCD_RS_PIN | LCD_SCK_PIN | LCD_SDA_PIN | LCD_RESET_PIN);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_PORT, &GPIO_InitStruct);

	//推完输出模式
	GPIO_InitStruct.GPIO_Pin = (ROM_SCK_PIN | ROM_IN_PIN | ROM_CS_PIN);
	GPIO_Init(ROM_PORT, &GPIO_InitStruct);
	
	// 上拉
	GPIO_InitStruct.GPIO_Pin = ROM_OUT_GET_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(ROM_PORT, &GPIO_InitStruct);
	
}

/**********************************************************************************************************
** Function name        :   lcd_init
** Descriptions         :   LCD模块初始化
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
int8_t lcd_init(void)
{
	lcd_gpio_initial();
	
	
	//lcd_reset=0;        					//低电平复位
	LCD_RESET_L();
	delay_ms(500);
	//lcd_reset=1;		    					//复位完毕
	LCD_RESET_H();
	delay_ms(200);        
	transfer_command_lcd(0xe2);	 //软复位
	delay_ms(50);
	transfer_command_lcd(0x2c);  //升压步聚1
	delay_ms(50);	
	transfer_command_lcd(0x2e);  //升压步聚2
	delay_ms(50);
	transfer_command_lcd(0x2f);  //升压步聚3
	delay_ms(50);
	transfer_command_lcd(0x23);  //粗调对比度，可设置范围0x20～0x27
	transfer_command_lcd(0x81);  //微调对比度
	transfer_command_lcd(0x28);  //JLX12864G-086的微调的对比度值为0x28,微调对比度的值可设置范围为0x00～0x3f
	transfer_command_lcd(0xa2);  //1/9偏压比（bias）
	transfer_command_lcd(0xc8);  //行扫描顺序：从上到下
	transfer_command_lcd(0xa0);  //列扫描顺序：从左到右
	transfer_command_lcd(0x40);  //起始行：第一行开始
	transfer_command_lcd(0xaf);  //开显示
	
	delay_ms(50);
	lcd_clear();
	
	return 0;
}

/**********************************************************************************************************
** Function name        :   lcd_clear
** Descriptions         :   全屏清屏
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void lcd_clear(void)
{
//	uint8_t i,j;
//	
// 	for(i = 0; i < 8; i++)
//	{
//		lcd_address(i, 0);
//		for(j = 0; j < 128; j++)
//		{
//			transfer_data_lcd(0x00);
//		}
//	}
	lcd_area_clear(0,0,8,128);
}

/**********************************************************************************************************
** Function name        :   lcd_area_clear
** Descriptions         :   清除指定区域
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void lcd_area_clear(uint8_t page, uint8_t colum, uint8_t pageSize, uint8_t columSize)
{
	uint8_t i, j;
	
	for (i = 0; i < pageSize; i++)
	{
		lcd_address(page + i, colum);
		for (j = 0; j < columSize; j++)
		{
			transfer_data_lcd(0x00);
		}
	}
}

/**********************************************************************************************************
** Function name        :   lcd_graphic_128x64
** Descriptions         :   显示128x64点阵图像，(0,0)坐标原点位置在左上角，(7,127)结束位置在右下角
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void lcd_graphic_128x64(uint8_t reverse, const uint8_t *bmp)
{
	uint8_t i, j;
	
	for(j = 0; j < 8; j++)
	{
		lcd_address(j, 0);
		for(i = 0; i < 128; i++)
		{
			transfer_data_lcd(bmp[i]);   
		}
	}          
}

//显示任意尺寸点阵图像，(0,0)坐标原点位置在左上角，(7,127)结束位置在右下角
void lcd_graphic_any_size (uint8_t y, uint8_t x, uint8_t ySize, uint8_t xSize, uint8_t reverse, const uint8_t * ico)
{
	uint8_t pageSize;
	uint8_t i, j, temp;
	
	/* 根据 y 轴的像素点位置计算 page 地址范围 */
	ySize % 8 == 0 ? (pageSize = ySize / 8) : (pageSize = ySize / 8 + 1);
	
	for(j = 0; j < pageSize; j++)
	{
		lcd_address(j + y, x);
		for(i = 0; i < xSize; i++)
		{
			if (reverse)
			{
				temp = ~ico[i + j * xSize];
			}
			else
			{
				temp = ico[i + j * xSize];
			}
			transfer_data_lcd(temp);
		}
	}
}

////24x24
//void lcd_str_gb24 (uint8_t page, uint8_t colum, uint8_t reverse, uint8_t *str)
//{
//	uint8_t i, j, k, temp, size;
//	uint8_t *t_str;
//	
//	t_str = str;
//	size = sizeof(GB_24) / sizeof(GB_24[0]);
//	while(*str > 0)
//	{
//		for (i = 0; i < size; i++)
//		{
//			if (t_str[0] == GB_24[i].Index[0] && t_str[1] == GB_24[i].Index[1])
//			{
//				k = i;
//				break;
//			}
//		}
//		
//		for (j = 0; j < 3; j++)
//		{
//			lcd_address(page + j, colum);
//			for (i = 0; i < 24; i++)
//			{
//				if (reverse)
//				{
//					temp = ~(GB_24[k].Msk[i + j * 24]);
//				}
//				else
//				{
//					temp = (GB_24[k].Msk[i + j * 24]);
//				}
//				transfer_data_lcd(temp);
//			}
//		}
//		colum += 24;
//		str += 2;
//		t_str = str;
//	}
//}

//16 x 24
void lcd_str_digit1624 (uint8_t page, uint8_t colum, uint8_t reverse, uint8_t *str)
{
	uint8_t i, j, k, temp, size;
	uint8_t *t_str;
	
	t_str = str;
	size = sizeof(DIGIT1624) / sizeof(DIGIT1624[0]);
	while(*str > 0)
	{
		for (i = 0; i < size; i++)
		{
			if (t_str[0] == DIGIT1624[i].Index[0])
			{
				k = i;
				break;
			}
		}
		
		for (j = 0; j < 3; j++)
		{
			lcd_address(page + j, colum);
			for (i = 0; i < 16; i++)
			{
				if (reverse)
				{
					temp = ~(DIGIT1624[k].Msk[i + j * 16]);
				}
				else
				{
					temp = (DIGIT1624[k].Msk[i + j * 16]);
				}
				transfer_data_lcd(temp);
			}
		}
		colum += 16;
		str++;
		t_str = str;
	}
}

/**********************************************************************************************************
** Function name        :   send_command_to_ROM
** Descriptions         :   送指令到晶联讯字库IC
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
static void send_command_to_ROM(uint8_t datu)
{
	uint8_t i;
	
	for(i = 0; i < 8; i++ )
	{
		ROM_SCK_L();
		lcdDelayUS(1);
		if(datu&0x80)
		{
			ROM_IN_H();
		}
		else
		{
			ROM_IN_L();
		}
		datu = datu << 1;
		ROM_SCK_H();
		lcdDelayUS(1);
	}
}

/**********************************************************************************************************
** Function name        :   get_data_from_ROM
** Descriptions         :   从晶联讯字库IC中取汉字或字符数据（1个字节）
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
static uint8_t get_data_from_ROM(void)
{
	
	uint8_t i;
	uint8_t ret_data = 0;
	for(i = 0; i < 8; i++)
	{
		ROM_SCK_L();
		lcdDelayUS(1);
		ret_data = ret_data << 1;
		if(GET_ROM_OUT())
		{
			ret_data = ret_data + 1;
		}
		else
		{
			ret_data = ret_data + 0;
		}
		ROM_SCK_H();
		lcdDelayUS(1);
	}
	return(ret_data);
}

/**********************************************************************************************************
** Function name        :   get_and_write_16x16
** Descriptions         :   从指定地址读出数据写到液晶屏指定（page,column)座标中
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
static void get_and_write_16x16(uint32_t fontaddr,uint8_t reverse,uint8_t framdecor,uint8_t page,uint8_t column)
{
	uint8_t i,j,disp_data;
	
	ROM_CS_L();
	send_command_to_ROM(0x03);
	send_command_to_ROM((fontaddr&0xff0000)>>16);	 //地址的高8位,共24位
	send_command_to_ROM((fontaddr&0xff00)>>8);		 //地址的中8位,共24位
	send_command_to_ROM(fontaddr&0xff);				 		//地址的低8位,共24位
	for(j = 0; j < 2; j++)
	{
		lcd_address(page + j,column);
		for(i = 0; i < 16; i++ )
	    {
			if(reverse)
			{
				disp_data = ~get_data_from_ROM();
			}
			else
			{
				if(0 == j)
				{
					disp_data = get_data_from_ROM() | (0x01 & framdecor);
				}
				else
				{
					disp_data = get_data_from_ROM() | (0x80 & framdecor);
				}
			}
			transfer_data_lcd(disp_data);	//写数据到LCD,每写完1字节的数据后列地址自动加1
		}
	}
	ROM_CS_H();
}

/**********************************************************************************************************
** Function name        :   get_and_write_8x16
** Descriptions         :   从指定地址读出数据写到液晶屏指定（page,column)座标中
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
static void get_and_write_8x16(uint32_t fontaddr,uint8_t reverse,uint8_t framdecor,uint8_t page,uint8_t column)
{
	uint8_t i,j,disp_data;
	
	ROM_CS_L();
	send_command_to_ROM(0x03);
	send_command_to_ROM((fontaddr&0xff0000)>>16);	 //地址的高8位,共24位
	send_command_to_ROM((fontaddr&0xff00)>>8);		 //地址的中8位,共24位
	send_command_to_ROM(fontaddr&0xff);				 //地址的低8位,共24位
	for(j = 0; j < 2; j++)
	{
		lcd_address(page + j,column);
		for(i = 0; i < 8; i++ )
	    {
			if(reverse)
			{
				disp_data = ~get_data_from_ROM();
			}
			else
			{
				if(0 == j)
				{
					disp_data = get_data_from_ROM() | (0x01 & framdecor);
				}
				else
				{
					disp_data = get_data_from_ROM() | (0x80 & framdecor);
				}
			}
			transfer_data_lcd(disp_data);	//写数据到LCD,每写完1字节的数据后列地址自动加1
		}
	}
	ROM_CS_H();
}

/**********************************************************************************************************
** Function name        :   get_and_write_5x8
** Descriptions         :   从指定地址读出数据写到液晶屏指定（page,column)座标中
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
static void get_and_write_5x8(uint32_t fontaddr,uint8_t reverse,uint8_t framdecor,uint8_t page,uint8_t column) 
{
	uint8_t i,disp_data;
	
	ROM_CS_L();
	send_command_to_ROM(0x03);
	send_command_to_ROM((fontaddr&0xff0000)>>16);	 //地址的高8位,共24位
	send_command_to_ROM((fontaddr&0xff00)>>8);		 //地址的中8位,共24位
	send_command_to_ROM(fontaddr&0xff);				 //地址的低8位,共24位
	lcd_address(page,column);
	for(i = 0; i < 5; i++ )
	{
		if(reverse)
		{
			disp_data=~get_data_from_ROM();
		}
		else
		{
			disp_data=get_data_from_ROM() | framdecor;
		}

		transfer_data_lcd(disp_data);	//写数据到LCD,每写完1字节的数据后列地址自动加1
	}
	ROM_CS_H();
}
/**********************************************************************************************************
** Function name        :   get_and_write_5x8
** Descriptions         :   显示16x16点阵的汉字/全角符号/全角标点，或8x16点阵的数字/英文/半角标点/ASCII码符号
                            当“reverse=1”时选择反显，当“reverse=0”时选择正显
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void lcd_str_gb2312(uint8_t page,uint8_t column,uint8_t reverse,uint8_t framdecor,uint8_t *text)
{
	uint8_t i = 0;
	uint32_t  fontaddr = 0;
	
	while((text[i] > 0x00) && i < 16)
	{
		if(((text[i]>=0xb0) &&(text[i]<=0xf7))&&(text[i+1]>=0xa1))
		{						
			//国标简体（GB2312）汉字在晶联讯字库IC中的地址由以下公式来计算：
			//Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0
			//由于担心8位单片机有乘法溢出问题，所以分三部取地址
			fontaddr = (text[i] - 0xb0) * 94; 
			fontaddr += (text[i + 1] - 0xa1) + 846;
			fontaddr = (uint32_t)(fontaddr * 32);

			get_and_write_16x16(fontaddr,reverse,framdecor,page,column);	 //从指定地址读出数据写到液晶屏指定（page,column)座标中
			i += 2;
			column += 16;
		}
		else if(((text[i] >= 0xa1) && (text[i] <= 0xa3)) && (text[i+1] >= 0xa1))
		{						
			//国标简体（GB2312）15x16点的字符在晶联讯字库IC中的地址由以下公式来计算：
			//Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0
			//由于担心8位单片机有乘法溢出问题，所以分三部取地址
			fontaddr = (text[i]- 0xa1) * 94; 
			fontaddr += (text[i + 1] - 0xa1);
			fontaddr = (uint32_t)(fontaddr * 32);
	
			get_and_write_16x16(fontaddr,reverse,framdecor,page,column);	 //从指定地址读出数据写到液晶屏指定（page,column)座标中
			i += 2;
			column += 16;
		}
		else if((text[i] >= 0x20) &&(text[i] <= 0x7e))	
		{									
			fontaddr = (text[i]- 0x20);
			fontaddr = (uint32_t)(fontaddr * 16);
			fontaddr = (uint32_t)(fontaddr + 0x3cf80);			

			get_and_write_8x16(fontaddr,reverse,framdecor,page,column);	 //从指定地址读出数据写到液晶屏指定（page,column)座标中
			i += 1;
			column += 8;
		}
		else
		{
			i++;	
		}
	}
}

/**********************************************************************************************************
** Function name        :   get_and_write_5x8
** Descriptions         :   显示5x8点阵数字/半角符号/半角标点
                            当“reverse=1”时选择反显，当“reverse=0”时选择正显
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void lcd_str_5x8(uint8_t page,uint8_t column,uint8_t reverse,uint8_t framdecor,uint8_t *text)
{
	uint8_t i= 0;
	uint32_t  fontaddr = 0;
	
	while((text[i]>0x00))
	{
		
		if((text[i] >= 0x20) && (text[i] <= 0x7e))	
		{						
			fontaddr = (text[i]- 0x20);
			fontaddr = (uint32_t)(fontaddr * 8);
			fontaddr = (uint32_t)(fontaddr + 0x3bfc0);			


			get_and_write_5x8(fontaddr,reverse,framdecor,page,column);	 //从指定地址读出数据写到液晶屏指定（page,column)座标中
			if(reverse)	
			{
				transfer_data_lcd(0xff);
			}
			else	
			{
				transfer_data_lcd(0x00);
			}
			i += 1;
			column += 6;
		}
		else
		i++;	
	}
}


void lcd_str_gb2312_line(uint8_t line,uint8_t reverse,uint8_t framdecor,uint8_t *str)
{
	lcd_str_gb2312(line, 0, reverse, framdecor, str);
}

void lcd_str_5x8_line(uint8_t line,uint8_t reverse,uint8_t framdecor,uint8_t *str)
{
	 lcd_str_5x8(line, 0, reverse, framdecor, str);
}

