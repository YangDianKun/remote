/*************************************** Copyright (c)******************************************************
** File name            :   main.h
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
#ifndef __MAIN_H
#define __MAIN_H
#include "stm32f10x.h"





/* // ϵͳ״̬���� */
#define SYS_ACTION_STATUS              0      //<! ��ҵ״̬
#define SYS_WAIT_WAKE_STATUS           1      //<! �ȴ�����״̬



/* // ϵͳģʽ���� */
#define SYS_LOW_POWER_MODE       0      //<! ����ģʽ
#define SYS_NORMAL_MODE          1      //<! ����ģʽ
#define SYS_HIG_POWER_MODE       2      //<! ������ģʽ


/* // ϵͳ�����붨�� */
#define SYS_NO_ERR          (uint8_t)(0x00)    //<! ϵͳ����
#define SYS_KEY_ERR         (uint8_t)(0x01)    //<! ��������
#define SYS_WL_ERR          (uint8_t)(0x02)    //<! ����ģ�����



/* // ϵͳ״̬���� */
typedef struct
{
	uint8_t mode;
	uint8_t state;
	uint8_t err;
}SysData_t;



#endif


