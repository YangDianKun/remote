/*************************************** Copyright (c)******************************************************
** File name            :   hal_usart.h
** Latest modified Date :   2018-06-03
** Latest Version       :   0.1
** Descriptions         :   
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
#ifndef __HAL_USART_H
#define __HAL_USART_H
#include "stm32f10x.h"

//#include "hal_config.h"




/* º¯ÊýÉùÃ÷ */
int8_t usart2_init (uint32_t baud);
void usart2_gpio_initial (void);
void usart2_tx_byte (uint8_t byte);
int8_t usart2_tx_buf (const uint8_t *pbBuf, uint8_t len);
int8_t usart2_rx_byte(uint8_t *oneByte, uint32_t timOut);
void usart2_baudrate_set (uint32_t baud);


#endif

/******************* (C) COPYRIGHT 2018 YangDianKun ************END OF FILE****/

