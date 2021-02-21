/*************************************** Copyright (c)******************************************************
** File name            :   paramter.c
** Latest modified Date :   2020-06-10
** Latest Version       :   0.1
** Descriptions         :   主文件，包含应用代码
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





/* // 用于保存参数的全局变量，写flash前先读取 */
static ParamData_Type saveData;


/**********************************************************************************************************
** Function name        :   systemParamInit
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
int8_t systemParamInit(void)
{
	//ParamData_Type sysParam;
	WLConfigData_t wlParam;
	int8_t ret = 0;
	
	if(BORN_PERMIT == SYS_DATA->bornFlag) // 设备第一次运行，配置出厂参数
	{
		saveData.verifData.bornFlag = BORN_CONFIRM;              // 确认
		memcpy(saveData.verifData.mucId, MCU_ID_DATA->McuId, sizeof(McuId_type)); // 拷贝 Mcu ID
		
		saveData.userData.devID = LEFT_DEV_ID;               // 出厂默认左站
		saveData.userData.systemMode = SYS_LOW_POWER_MODE;   // 出厂默认低功耗模式
		if(hal_WriteCpuFlash(SYS_DATA_ADDR, (uint8_t *)&saveData, sizeof(ParamData_Type)) != 0)  // 参数写入到 flash 内
		{
			return -1;
		}
		
		/* // 配置无线模块默认参数 */
		wlParam.addr_h = 0x12;          // 高地址位
		wlParam.addr_l = 0x34;          // 低地址位
		wlParam.channal = WL_CHANNEl;   // 信道
		//wlParam.speed = 0x1A;           // 串口波特率 ，空速 9600 2.4k
		wlParam.speed = 0x2A;           // 串口波特率 ，空速 38400 2.4k
		wlParam.option = 0x7E;          // 透明传输，开启FEC，功耗配置
		wlParam.cmd = WL_SET_CMD_SAVE;
		if (wl_set_param(&wlParam) < 0)
		{
			return -2;
		}

	}
	else
	{
		if(!memcmp(MCU_ID_DATA->McuId, SYS_DATA->mucId, sizeof(SYS_DATA->mucId))) // Mcu 身份验证
		{
			// 验证通过
			ret = 0;
		}
		else 
		{
			// 验证失败
			ret = -3;
			lcd_clear();
			lcd_str_gb2312_line(1, 0, 0, "参数非法");
			while(1);
		}
	}
	return ret;
}

/**********************************************************************************************************
** Function name        :   systemParamSave
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
int8_t systemParamSave(UserData_type * pWriteData)
{
	//ParamData_Type saveData;
	
	/* 读取系统保存的数据，写入数据时会擦除 */
	saveData.verifData.bornFlag = SYS_DATA->bornFlag;
	memcpy(saveData.verifData.mucId, MCU_ID_DATA->McuId, sizeof(McuId_type));
	
	/* 配置数据写入缓冲变量中 */
	//saveData.userData.devID = pWriteData->devID;
	memcpy(&(saveData.userData.devID), pWriteData, sizeof(UserData_type));
	
	
	/* 参数写入到 flash 内 */
	if(hal_WriteCpuFlash(PARAM_BASE_FLASH_ADDR, (uint8_t *)&saveData, sizeof(saveData)) != 0)  // 参数写入到 flash 内
	{
		return -1;
	}
	
	return 0;
}
//int8_t systemParamSave(ParamData_Type * pWriteData)
//{
//	
//	/* 参数写入到 flash 内 */
//	if(hal_WriteCpuFlash(PARAM_BASE_FLASH_ADDR, (uint8_t *)pWriteData, sizeof(ParamData_Type)) != 0)  // 参数写入到 flash 内
//	{
//		return -1;
//	}
//	
//	return 0;
//}




