/*************************************** Copyright (c)******************************************************
** File name            :   hal_led.h
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
#ifndef __HAL_LED_H
#define __HAL_LED_H

#include "stm32f10x.h"




/* LED 端口、引脚定义  */
#define LED1_PORT             GPIOC
#define LED1_PIN              GPIO_Pin_3

#define LED_PORT_RCC          ( RCC_APB2Periph_GPIOC )



/* led 操作 */
#define LED1_ON()         GPIO_ResetBits(LED1_PORT, LED1_PIN)
#define LED1_OFF()        GPIO_SetBits(LED1_PORT, LED1_PIN)
#define LED1_TOGGLE()     do{LED1_PORT->ODR ^= LED1_PIN;}while(0)



/* 函数声明 */
int8_t led_init (void);



#endif

/*********************************** (C) COPYRIGHT 2018 YangDianKun ************************END OF FILE****/

