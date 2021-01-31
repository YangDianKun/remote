/*************************************** Copyright (c)******************************************************
** File name            :   hal_lcd.c
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
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
static void lcdDelayUS(uint32_t x)
{
	delay_us(x);
}

/**********************************************************************************************************
** Function name        :   transfer_command_lcd
** Descriptions         :   дָ�LCDģ��
** parameters           :   ��
** Returned value       :   ��
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
** Descriptions         :   ����Һ��������ʾ��ַ��page��column��
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
void lcd_address(uint8_t page,uint8_t column)
{
	transfer_command_lcd(0xb0 + page);   				 //����ҳ��ַ��ÿҳ��8�С�һ�������64�б��ֳ�8��ҳ������ƽ����˵�ĵ�1ҳ����LCD����IC���ǵ�0ҳ�������������ȥ1
	transfer_command_lcd(((column >> 4) & 0x0f) + 0x10); //�����е�ַ�ĸ�4λ
	transfer_command_lcd(column & 0x0f);				 //�����е�ַ�ĵ�4λ
}

/**********************************************************************************************************
** Function name        :   transfer_data_lcd
** Descriptions         :   д���ݵ�LCDģ��
** parameters           :   ��
** Returned value       :   ��
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
** Descriptions         :   ��ʼ��PA��Ϊ�����.��ʹ�����IO�ڵ�ʱ��	
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
void lcd_gpio_initial(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(LCD_ROM_PORT_RCC, ENABLE);

	//�������ģʽ
	GPIO_InitStruct.GPIO_Pin = (LCD_CS_PIN | LCD_RS_PIN | LCD_SCK_PIN | LCD_SDA_PIN | LCD_RESET_PIN);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_PORT, &GPIO_InitStruct);

	//�������ģʽ
	GPIO_InitStruct.GPIO_Pin = (ROM_SCK_PIN | ROM_IN_PIN | ROM_CS_PIN);
	GPIO_Init(ROM_PORT, &GPIO_InitStruct);
	
	// ����
	GPIO_InitStruct.GPIO_Pin = ROM_OUT_GET_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(ROM_PORT, &GPIO_InitStruct);
	
}

/**********************************************************************************************************
** Function name        :   lcd_init
** Descriptions         :   LCDģ���ʼ��
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
int8_t lcd_init(void)
{
	lcd_gpio_initial();
	
	
	//lcd_reset=0;        					//�͵�ƽ��λ
	LCD_RESET_L();
	delay_ms(500);
	//lcd_reset=1;		    					//��λ���
	LCD_RESET_H();
	delay_ms(200);        
	transfer_command_lcd(0xe2);	 //��λ
	delay_ms(50);
	transfer_command_lcd(0x2c);  //��ѹ����1
	delay_ms(50);	
	transfer_command_lcd(0x2e);  //��ѹ����2
	delay_ms(50);
	transfer_command_lcd(0x2f);  //��ѹ����3
	delay_ms(50);
	transfer_command_lcd(0x23);  //�ֵ��Աȶȣ������÷�Χ0x20��0x27
	transfer_command_lcd(0x81);  //΢���Աȶ�
	transfer_command_lcd(0x28);  //JLX12864G-086��΢���ĶԱȶ�ֵΪ0x28,΢���Աȶȵ�ֵ�����÷�ΧΪ0x00��0x3f
	transfer_command_lcd(0xa2);  //1/9ƫѹ�ȣ�bias��
	transfer_command_lcd(0xc8);  //��ɨ��˳�򣺴��ϵ���
	transfer_command_lcd(0xa0);  //��ɨ��˳�򣺴�����
	transfer_command_lcd(0x40);  //��ʼ�У���һ�п�ʼ
	transfer_command_lcd(0xaf);  //����ʾ
	
	delay_ms(50);
	lcd_clear();
	
	return 0;
}

/**********************************************************************************************************
** Function name        :   lcd_clear
** Descriptions         :   ȫ������
** parameters           :   ��
** Returned value       :   ��
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
** Descriptions         :   ���ָ������
** parameters           :   ��
** Returned value       :   ��
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
** Descriptions         :   ��ʾ128x64����ͼ��(0,0)����ԭ��λ�������Ͻǣ�(7,127)����λ�������½�
** parameters           :   ��
** Returned value       :   ��
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

//��ʾ����ߴ����ͼ��(0,0)����ԭ��λ�������Ͻǣ�(7,127)����λ�������½�
void lcd_graphic_any_size (uint8_t y, uint8_t x, uint8_t ySize, uint8_t xSize, uint8_t reverse, const uint8_t * ico)
{
	uint8_t pageSize;
	uint8_t i, j, temp;
	
	/* ���� y ������ص�λ�ü��� page ��ַ��Χ */
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
** Descriptions         :   ��ָ�����Ѷ�ֿ�IC
** parameters           :   ��
** Returned value       :   ��
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
** Descriptions         :   �Ӿ���Ѷ�ֿ�IC��ȡ���ֻ��ַ����ݣ�1���ֽڣ�
** parameters           :   ��
** Returned value       :   ��
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
** Descriptions         :   ��ָ����ַ��������д��Һ����ָ����page,column)������
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
static void get_and_write_16x16(uint32_t fontaddr,uint8_t reverse,uint8_t framdecor,uint8_t page,uint8_t column)
{
	uint8_t i,j,disp_data;
	
	ROM_CS_L();
	send_command_to_ROM(0x03);
	send_command_to_ROM((fontaddr&0xff0000)>>16);	 //��ַ�ĸ�8λ,��24λ
	send_command_to_ROM((fontaddr&0xff00)>>8);		 //��ַ����8λ,��24λ
	send_command_to_ROM(fontaddr&0xff);				 		//��ַ�ĵ�8λ,��24λ
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
			transfer_data_lcd(disp_data);	//д���ݵ�LCD,ÿд��1�ֽڵ����ݺ��е�ַ�Զ���1
		}
	}
	ROM_CS_H();
}

/**********************************************************************************************************
** Function name        :   get_and_write_8x16
** Descriptions         :   ��ָ����ַ��������д��Һ����ָ����page,column)������
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
static void get_and_write_8x16(uint32_t fontaddr,uint8_t reverse,uint8_t framdecor,uint8_t page,uint8_t column)
{
	uint8_t i,j,disp_data;
	
	ROM_CS_L();
	send_command_to_ROM(0x03);
	send_command_to_ROM((fontaddr&0xff0000)>>16);	 //��ַ�ĸ�8λ,��24λ
	send_command_to_ROM((fontaddr&0xff00)>>8);		 //��ַ����8λ,��24λ
	send_command_to_ROM(fontaddr&0xff);				 //��ַ�ĵ�8λ,��24λ
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
			transfer_data_lcd(disp_data);	//д���ݵ�LCD,ÿд��1�ֽڵ����ݺ��е�ַ�Զ���1
		}
	}
	ROM_CS_H();
}

/**********************************************************************************************************
** Function name        :   get_and_write_5x8
** Descriptions         :   ��ָ����ַ��������д��Һ����ָ����page,column)������
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
static void get_and_write_5x8(uint32_t fontaddr,uint8_t reverse,uint8_t framdecor,uint8_t page,uint8_t column) 
{
	uint8_t i,disp_data;
	
	ROM_CS_L();
	send_command_to_ROM(0x03);
	send_command_to_ROM((fontaddr&0xff0000)>>16);	 //��ַ�ĸ�8λ,��24λ
	send_command_to_ROM((fontaddr&0xff00)>>8);		 //��ַ����8λ,��24λ
	send_command_to_ROM(fontaddr&0xff);				 //��ַ�ĵ�8λ,��24λ
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

		transfer_data_lcd(disp_data);	//д���ݵ�LCD,ÿд��1�ֽڵ����ݺ��е�ַ�Զ���1
	}
	ROM_CS_H();
}
/**********************************************************************************************************
** Function name        :   get_and_write_5x8
** Descriptions         :   ��ʾ16x16����ĺ���/ȫ�Ƿ���/ȫ�Ǳ�㣬��8x16���������/Ӣ��/��Ǳ��/ASCII�����
                            ����reverse=1��ʱѡ���ԣ�����reverse=0��ʱѡ������
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
void lcd_str_gb2312(uint8_t page,uint8_t column,uint8_t reverse,uint8_t framdecor,uint8_t *text)
{
	uint8_t i = 0;
	uint32_t  fontaddr = 0;
	
	while((text[i] > 0x00) && i < 16)
	{
		if(((text[i]>=0xb0) &&(text[i]<=0xf7))&&(text[i+1]>=0xa1))
		{						
			//������壨GB2312�������ھ���Ѷ�ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺
			//Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0
			//���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ
			fontaddr = (text[i] - 0xb0) * 94; 
			fontaddr += (text[i + 1] - 0xa1) + 846;
			fontaddr = (uint32_t)(fontaddr * 32);

			get_and_write_16x16(fontaddr,reverse,framdecor,page,column);	 //��ָ����ַ��������д��Һ����ָ����page,column)������
			i += 2;
			column += 16;
		}
		else if(((text[i] >= 0xa1) && (text[i] <= 0xa3)) && (text[i+1] >= 0xa1))
		{						
			//������壨GB2312��15x16����ַ��ھ���Ѷ�ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺
			//Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0
			//���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ
			fontaddr = (text[i]- 0xa1) * 94; 
			fontaddr += (text[i + 1] - 0xa1);
			fontaddr = (uint32_t)(fontaddr * 32);
	
			get_and_write_16x16(fontaddr,reverse,framdecor,page,column);	 //��ָ����ַ��������д��Һ����ָ����page,column)������
			i += 2;
			column += 16;
		}
		else if((text[i] >= 0x20) &&(text[i] <= 0x7e))	
		{									
			fontaddr = (text[i]- 0x20);
			fontaddr = (uint32_t)(fontaddr * 16);
			fontaddr = (uint32_t)(fontaddr + 0x3cf80);			

			get_and_write_8x16(fontaddr,reverse,framdecor,page,column);	 //��ָ����ַ��������д��Һ����ָ����page,column)������
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
** Descriptions         :   ��ʾ5x8��������/��Ƿ���/��Ǳ��
                            ����reverse=1��ʱѡ���ԣ�����reverse=0��ʱѡ������
** parameters           :   ��
** Returned value       :   ��
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


			get_and_write_5x8(fontaddr,reverse,framdecor,page,column);	 //��ָ����ַ��������д��Һ����ָ����page,column)������
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

