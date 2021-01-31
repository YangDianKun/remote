/*************************************** Copyright (c)******************************************************
** File name            :   hal_usart.c
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
#include "hal_usart.h"	
#include <stdio.h>




//加入以下代码,支持printf函数,而不需要选择use MicroLIB	 
#if  0
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif



/**********************************************************************************************************
** Function name        :   __usart2_gpio_initial
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void usart2_gpio_initial (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//USART2_TX   GPIOA.2
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;            //PA.2
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;	  //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStruct);               //初始化GPIOA.9

	//USART2_RX	  GPIOA.3初始化
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;               //PA3
	//GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;           //上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStruct);                  //初始化GPIOA.10  
}

/**********************************************************************************************************
** Function name        :   hal_InitHardUsart2
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
int8_t usart2_init (uint32_t baud)
{
	USART_InitTypeDef USART_InitStructure;
	
	usart2_gpio_initial();
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	USART_DeInit(USART2);

	/* 配置串口硬件参数 */
	USART_InitStructure.USART_BaudRate = baud;	/* 波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	


	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
	   如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART2, USART_FLAG_TC);        // 清发送完成标志，Transmission Complete flag 
	USART_Cmd(USART2, ENABLE);                     //使能串口2

    return 0;
}

/**********************************************************************************************************
** Function name        :   usart2_baudrate_set
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void usart2_baudrate_set (uint32_t baud)
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate = baud;	/* 波特率 */
	USART_Init(USART2, &USART_InitStructure);
}
/**********************************************************************************************************
** Function name        :   bsp_HardUsart2SendByte
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void usart2_tx_byte (uint8_t byte)
{
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET);
	USART_SendData(USART2, byte);
}

/**********************************************************************************************************
** Function name        :   bsp_HardUsart2Transmit
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
int8_t usart2_tx_buf (const uint8_t *pbBuf, uint8_t len)
{
	uint8_t i;
	for (i = 0; i < len; i++)
	{
		usart2_tx_byte(pbBuf[i]);
	}
	
	return 0;
}

/**********************************************************************************************************
** Function name        :   usart2_rx_byte
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
int8_t usart2_rx_byte(uint8_t *oneByte, uint32_t timOut)
{
	uint32_t cnt = 0;
	
	if(oneByte == NULL)
	{
		return -1;
	}
	
	while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != SET && cnt++ < timOut);
	*oneByte = USART_ReceiveData(USART2);
	
	if(cnt < timOut)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}





