/*************************************** Copyright (c)******************************************************
** File name            :   paramter.c
** Latest modified Date :   2020-06-10
** Latest Version       :   0.1
** Descriptions         :   ���ļ�������Ӧ�ô���
**
**--------------------------------------------------------------------------------------------------------
** Created by           :   YangDianKun
** Created date         :   2018-06-10
** Version              :   1.0
** Descriptions         :   The original version
**
**--------------------------------------------------------------------------------------------------------
** Copyright            :  
** Author Email         :   1163101403@qq.com
**********************************************************************************************************/
#include "main.h"
#include "board.h"
#include "paramter.h"
#include "lora433.h"
#include "hal_cpu_flash.h"
#include <string.h>





/* // ���ڱ��������ȫ�ֱ�����дflashǰ�ȶ�ȡ */
static ParamData_Type saveData;


/**********************************************************************************************************
** Function name        :   systemParamInit
** Descriptions         :   
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
int8_t systemParamInit(void)
{
	//ParamData_Type sysParam;
	WLConfigData_t wlParam;
	int8_t ret = 0;
	
	if(BORN_PERMIT == SYS_DATA->bornFlag) // �豸��һ�����У����ó�������
	{
		saveData.verifData.bornFlag = BORN_CONFIRM;              // ȷ��
		memcpy(saveData.verifData.mucId, MCU_ID_DATA->McuId, sizeof(McuId_type)); // ���� Mcu ID
		
		saveData.userData.devID = LEFT_DEV_ID;               // ����Ĭ����վ
		saveData.userData.systemMode = SYS_LOW_POWER_MODE;   // ����Ĭ�ϵ͹���ģʽ
		if(hal_WriteCpuFlash(SYS_DATA_ADDR, (uint8_t *)&saveData, sizeof(ParamData_Type)) != 0)  // ����д�뵽 flash ��
		{
			return -1;
		}
		
		/* // ��������ģ��Ĭ�ϲ��� */
		wlParam.addr_h = 0x12;          // �ߵ�ַλ
		wlParam.addr_l = 0x34;          // �͵�ַλ
		wlParam.channal = WL_CHANNEl;   // �ŵ�
		//wlParam.speed = 0x1A;           // ���ڲ����� ������ 9600 2.4k
		wlParam.speed = 0x2A;           // ���ڲ����� ������ 38400 2.4k
		wlParam.option = 0x7E;          // ͸�����䣬����FEC����������
		wlParam.cmd = WL_SET_CMD_SAVE;
		if (wl_set_param(&wlParam) < 0)
		{
			return -2;
		}

	}
	else
	{
		if(!memcmp(MCU_ID_DATA->McuId, SYS_DATA->mucId, sizeof(SYS_DATA->mucId))) // Mcu �����֤
		{
			// ��֤ͨ��
			ret = 0;
		}
		else 
		{
			// ��֤ʧ��
			ret = -3;
			lcd_clear();
			lcd_str_gb2312_line(1, 0, 0, "�����Ƿ�");
			while(1);
		}
	}
	return ret;
}

/**********************************************************************************************************
** Function name        :   systemParamSave
** Descriptions         :   
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
int8_t systemParamSave(UserData_type * pWriteData)
{
	//ParamData_Type saveData;
	
	/* ��ȡϵͳ��������ݣ�д������ʱ����� */
	saveData.verifData.bornFlag = SYS_DATA->bornFlag;
	memcpy(saveData.verifData.mucId, MCU_ID_DATA->McuId, sizeof(McuId_type));
	
	/* ��������д�뻺������� */
	//saveData.userData.devID = pWriteData->devID;
	memcpy(&(saveData.userData.devID), pWriteData, sizeof(UserData_type));
	
	
	/* ����д�뵽 flash �� */
	if(hal_WriteCpuFlash(PARAM_BASE_FLASH_ADDR, (uint8_t *)&saveData, sizeof(saveData)) != 0)  // ����д�뵽 flash ��
	{
		return -1;
	}
	
	return 0;
}
//int8_t systemParamSave(ParamData_Type * pWriteData)
//{
//	
//	/* ����д�뵽 flash �� */
//	if(hal_WriteCpuFlash(PARAM_BASE_FLASH_ADDR, (uint8_t *)pWriteData, sizeof(ParamData_Type)) != 0)  // ����д�뵽 flash ��
//	{
//		return -1;
//	}
//	
//	return 0;
//}




