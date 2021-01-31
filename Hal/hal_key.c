/*************************************** Copyright (c)******************************************************
** File name            :   hal_key.c
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
#include "hal_key.h"
#include "hal_delay.h"
#include "hal_led.h"
#include "board.h"





/* ���尴���˿ڡ�������    */
#define KEY1_PROT        GPIOC
#define KEY1_PIN         GPIO_Pin_4

#define KEY2_PROT        GPIOC
#define KEY2_PIN         GPIO_Pin_5

#define KEY3_PROT        GPIOC
#define KEY3_PIN         GPIO_Pin_6

#define KEY4_PROT        GPIOC
#define KEY4_PIN         GPIO_Pin_7

#define KEY5_PROT        GPIOC
#define KEY5_PIN         GPIO_Pin_8

#define KEY6_PROT        GPIOC
#define KEY6_PIN         GPIO_Pin_9

#define KEY7_PROT        GPIOC
#define KEY7_PIN         GPIO_Pin_10

#define KEY8_PROT        GPIOC
#define KEY8_PIN         GPIO_Pin_11

#define KEY9_PROT        GPIOC
#define KEY9_PIN         GPIO_Pin_12

#define KEY10_PROT       GPIOC
#define KEY10_PIN        GPIO_Pin_13

#define KEY11_PROT       GPIOC
#define KEY11_PIN        GPIO_Pin_14

#define KEY12_PROT       GPIOC
#define KEY12_PIN        GPIO_Pin_15
					  
/* �˿�ʱ�� */				  
#define KEY_PORT_RCC     (RCC_APB2Periph_GPIOC)





//uint8_t (*IsKeyScanArray[KEY_COUNT])(void) = {IsKey1Down, IsKey2Down, IsKey3Down, IsKey4Down,
//											  IsKey5Down, IsKey6Down, IsKey7Down, IsKey8Down,	
//											  IsKey9Down, IsKey10Down, IsKey11Down, IsKey12Down};

/*********************************************************************************************************
*	�� �� ��: IsKeyDownX
*	����˵��: �жϰ����Ƿ���
*	��    ��: ��
*	�� �� ֵ: ����ֵ; 0 ��ʾ���£�1 ��ʾδ����
*********************************************************************************************************/
//uint8_t IsKey1Down(void)       { return GPIO_ReadInputDataBit(KEY1_PROT, KEY1_PIN); }
uint8_t IsKey2Down(void)       { return GPIO_ReadInputDataBit(KEY2_PROT, KEY2_PIN); }
uint8_t IsKey3Down(void)       { return GPIO_ReadInputDataBit(KEY3_PROT, KEY3_PIN); }
uint8_t IsKey4Down(void)       { return GPIO_ReadInputDataBit(KEY4_PROT, KEY4_PIN); }
uint8_t IsKey5Down(void)       { return GPIO_ReadInputDataBit(KEY5_PROT, KEY5_PIN); }
uint8_t IsKey6Down(void)       { return GPIO_ReadInputDataBit(KEY6_PROT, KEY6_PIN); }
uint8_t IsKey7Down(void)       { return GPIO_ReadInputDataBit(KEY7_PROT, KEY7_PIN); }
uint8_t IsKey8Down(void)       { return GPIO_ReadInputDataBit(KEY8_PROT, KEY8_PIN); }
uint8_t IsKey9Down(void)       { return GPIO_ReadInputDataBit(KEY9_PROT, KEY9_PIN); }
uint8_t IsKey10Down(void)      { return GPIO_ReadInputDataBit(KEY10_PROT, KEY10_PIN); }
uint8_t IsKey11Down(void)      { return GPIO_ReadInputDataBit(KEY11_PROT, KEY11_PIN); }
uint8_t IsKey12Down(void)      { return GPIO_ReadInputDataBit(KEY12_PROT, KEY12_PIN); }


									
/*********************************************************************************************************
*	�� �� ��: hal_InitKeyGpio
*	����˵��: ��ʼ����������
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************/
static void hal_InitKeyGpio (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	// �����˿�ʱ��
	RCC_APB2PeriphClockCmd(KEY_PORT_RCC, ENABLE);
	
	// ��������
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStruct.GPIO_Pin = KEY1_PIN;
//	GPIO_Init(KEY1_PROT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = KEY2_PIN;
	GPIO_Init(KEY2_PROT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = KEY3_PIN;
	GPIO_Init(KEY3_PROT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = KEY4_PIN;
	GPIO_Init(KEY4_PROT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = KEY5_PIN;
	GPIO_Init(KEY5_PROT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = KEY6_PIN;
	GPIO_Init(KEY6_PROT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = KEY7_PIN;
	GPIO_Init(KEY7_PROT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = KEY8_PIN;
	GPIO_Init(KEY8_PROT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = KEY9_PIN;
	GPIO_Init(KEY9_PROT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = KEY10_PIN;
	GPIO_Init(KEY10_PROT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = KEY11_PIN;
	GPIO_Init(KEY11_PROT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = KEY12_PIN;
	GPIO_Init(KEY12_PROT, &GPIO_InitStruct);
}

/*********************************************************************************************************
*	�� �� ��: hal_InitKey
*	����˵��: ��ʼ������. �ú����� hal_Init() ���á�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************/
int8_t hal_InitKey(void)
{
	hal_InitKeyGpio();		/* ��ʼ������Ӳ�� */
	
	delay_ms(10);
	// �������ϼ��
	if (hal_GetKeyValue() != KEY_NONE)
	{
		delay_ms(20);
		if (hal_GetKeyValue() != KEY_NONE)
		{
			lcd_clear();
			lcd_str_gb2312(2, 8, 0, 0, "  �������ϣ�");
		}
	}
	while(hal_GetKeyValue() != KEY_NONE)
	{
		delay_ms(20);
	}
	
	
	return 0;
}

/*********************************************************************************************************
*	�� �� ��: hal_GetKey
*	����˵��: 
*	��    ��:  ��
*	�� �� ֵ: ��������
*********************************************************************************************************/
#if 0
uint8_t hal_GetKeyValue(void)
{
	uint8_t retValue = KEY_NONE;
	uint8_t i;
	
	for(i = 0; i < KEY_COUNT; i++)
	{
		if(!IsKeyScanArray[i]())
		{
			retValue =  (i + KEY_1_DOWN);
			break;
		}
	}
	
	return retValue;
}
#else
uint8_t hal_GetKeyValue(void)
{
	//if(!IsKey1Down()) {return KEY_1_DOWN;}
	
	if(!IsKey2Down()) {return KEY_2_DOWN;}
	
	//if(!IsKey3Down()) {return KEY_3_DOWN;}
	
	if(!IsKey4Down()) {return KEY_4_DOWN;}
	
	if(!IsKey5Down()) {return KEY_5_DOWN;}
	
	if(!IsKey6Down()) {return KEY_6_DOWN;}
	
	if(!IsKey7Down()) {return KEY_7_DOWN;}
	
	if(!IsKey8Down()) {return KEY_8_DOWN;}
	
	if(!IsKey9Down()) {return KEY_9_DOWN;}
	
	if(!IsKey10Down()) {return KEY_10_DOWN;}
	
	if(!IsKey11Down()) {return KEY_11_DOWN;}
	
	if(!IsKey12Down()) {return KEY_12_DOWN;}
	
	return KEY_NONE;
	
}
#endif






