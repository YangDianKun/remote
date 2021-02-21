/*************************************** Copyright (c)******************************************************
** File name            :   remote.c
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
#include "handle.h"
#include "board.h"
#include "paramter.h"
//#include "menu.h"
#include "interact.h"




/*  */
typedef struct
{
	uint8_t (*keyFunc) (void);
	uint8_t cmdOpt;
	uint8_t * cmdStr;
}OptRelatTable_t;




/* // 按键及遥控命令对应的功能字符串 */
static const OptRelatTable_t OptRelatTable[] = {{IsKey2Down, CRUSH_UP_FUNC, "碎升"},
										 {IsKey4Down, CRUSH_DOWN_FUNC, "碎降"},
										 {IsKey5Down, LEFT_MV_FUNC, "向左"},
										 {IsKey6Down, RIGHT_MV_FUNC, "向右"},
										 {IsKey7Down, DEC_MV_FUNC, "减速"},
										 {IsKey8Down, ACC_MV_FUNC, "加速"},
										 {IsKey9Down, STOP_MV_FUNC, "牵停"},
										 {IsKey10Down, MAIN_STOP_FUNC, "主停"},
										 {IsKey11Down, L_CH_UP_MV_FUNC, "上升"},
										 {IsKey12Down, L_CH_DOWN_MV_FUNC, "下降"}};

												 
												 
												 
/**********************************************************************************************************
** Function name        :   opt_handle_init
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/									 
int8_t opt_handle_init(void)
{
//	if (RIGHT_DEV_ID == CONFIG_DATA->devID)
//	{
//		OptRelatTable[8].cmdOpt = R_CH_UP_MV_FUNC;
//		OptRelatTable[9].cmdOpt = R_CH_DOWN_MV_FUNC;
//	}
//	else
//	{
//		OptRelatTable[8].cmdOpt = L_CH_UP_MV_FUNC;
//		OptRelatTable[9].cmdOpt = L_CH_DOWN_MV_FUNC;
//	}
	
	return 0;
}
/**********************************************************************************************************
** Function name        :   check_sum
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
uint8_t check_sum(const uint8_t * data, uint8_t size)
{
	uint8_t sum = 0;
	
	if(data != NULL && size > 0)
	{
		while(size--)
		{
			sum += *data++;
		}
	}
	return sum;
}


/**********************************************************************************************************
** Function name        :   keys_option_handle
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
int8_t keys_option_handle(void)
{
	int8_t ret = 0;
	uint8_t i, array_max;
	//static uint8_t cmdBlink = 0, cmdBlinkCnt = 0;
	
	/* 按键功能处理 */
	array_max = sizeof(OptRelatTable) / sizeof(OptRelatTable_t);
	for (i = 0; i < array_max; i++)
	{
		if (0 == OptRelatTable[i].keyFunc())  // 查询按键，0 有按键按下事件
		{
			//LCD_LED_ON(); // 开启 lcd 背光
			lcd_str_gb2312(6, 8, 0, LCD_UP_FRAM, OptRelatTable[i].cmdStr); // 命令显示
			while(0 == OptRelatTable[i].keyFunc())
			{
				if (i < array_max - 2)
				{
					rmf_opt_transmit(OptRelatTable[i].cmdOpt);      // 发送操作数据
				}
				else
				{
					rmf_opt_transmit(OptRelatTable[i].cmdOpt + 2);      // 发送操作数据
				}
				//rmf_opt_transmit(OptRelatTable[i].cmdOpt);      // 发送操作数据
				LED1_TOGGLE();
//				if (++cmdBlinkCnt >= 3)
//				{
//					cmdBlinkCnt = 0;
//					cmdBlink = ~cmdBlink;
//					lcd_str_gb2312(6, 8, cmdBlink, LCD_UP_FRAM, OptRelatTable[i].cmdStr); // 命令显示
//				}
				delay_ms(50);
			}
			LED1_OFF();
			lcd_str_gb2312(6, 8, 0, LCD_UP_FRAM, "    ");	 // 清空功能码显示	
			ret = 1;
			break; // 跳出 for
		}
	}
	
	return ret;
}

/**********************************************************************************************************
** Function name        :   rfm_frame_transmit
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
int8_t rfm_data_transmit (void * data, uint8_t dataLen)
{
	uint8_t buf[16];
	uint8_t * tData = NULL;
	uint8_t cnt = 0;
	uint8_t i;
	
	if(data == NULL || dataLen == 0)
	{
		return -1;
	}
	
	// 数据长度判断
	if(dataLen > 16 - cnt)
	{
		dataLen = 16 - cnt;
	}
	
	// 接收端，帧同步段
	buf[cnt++] = 0xD1;
	
	// 数据装载
	tData = (uint8_t *)data;
	for(i = cnt; i < dataLen; i++)
	{
		buf[i] = *tData++;
	}
	
//	// 判断模块是否忙
//	if(!wl_state_get())
//		return -1;
		
	// 发送数据
	usart2_tx_buf(buf, cnt + i);
	
	return 0;
}

/**********************************************************************************************************
** Function name        :   rmfOptionSend
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
int8_t rmf_opt_transmit (uint8_t cmd)
{
	uint8_t tx[3];
	
	// 数据装载
	tx[0] = 0xD1;
	tx[1] = cmd;
	tx[2] = ~cmd;
	
	// 判断模块是否忙
//	if(!wl_state_get())
//		return -1;
	
	// 发送数据
	usart2_tx_buf(tx, sizeof(tx));
	
	return 0;
}

/**********************************************************************************************************
** Function name        :   enter_pair
** Descriptions         :   
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
#if 0
void enter_pair(void)
{
	ConfigData_Type targetData;
	uint8_t repeat = 0;
	uint8_t pairState = 0;
	uint32_t timCnt = 0;

	lcd_clear();
	usart2_it_rxBuf_clear();
	lcd_str_gb2312_line(2, 0, 0, "搜索设备...");
	
	while(1)
	{
		if(repeat++ >= 10)
		{
			lcd_str_gb2312_line(2, 0, 0, "连接超时...");
			lcd_str_gb2312_line(4, 0, 0, "请关机重试");
			while(1);
		}
		
		if(0 == pairState) // 开始配对
		{
			DataFrame_t pairData;
			
			pairData.head = DATA_FRAME_TYPE | PAIR_REQUEST;   // 帧类型
			pairData.addr_H = getWLParam()->addr_h;                 // 本机地址
			pairData.addr_L = getWLParam()->addr_l;
			pairData.key = getWLParam()->channal;
			pairData.sum = check_sum(&(pairData.head), sizeof(pairData) - 1);
			if(wl_state_get())
			{
				rfm_frame_transmit(0xFFFF, pairData.key, &pairData, sizeof(pairData));  // 广播发送
			}
			
			// 等待应答
			timCnt = 0;
			while(!usart2_it_rxBuf_isOk() && timCnt++ < BD_300MS_TIMEOUT);
			
			if(usart2_it_rxBuf_isOk())
			{
				DataFrame_t * pDataFrame = (DataFrame_t *)usart2_it_rxBuf();
				if((pDataFrame->head & FRAME_TYPE_MASK) == DATA_FRAME_TYPE)
				{
					if(pDataFrame->sum == check_sum(&(pDataFrame->head), sizeof(DataFrame_t) - 1))
					{
						if((pDataFrame->head & CMD_FRAME_MASK) == PAIR_REQUEST)
						{
							targetData.targetAddr = pDataFrame->addr_H;      // 保存接收的地址数据
							targetData.targetAddr |= (uint16_t)pDataFrame->addr_L;
							lcd_str_gb2312_line(2, 0, 0, "连接设备...");
							pairState = 1;
						}
					}
				}
				usart2_it_rxBuf_clear();
			}
		}
		else if(1 == pairState) // 配对
		{
			RemoteFrame_t confirmFrame;
			
			confirmFrame.head = PAIR_COMPLETE | STATUS_FRAME_TYPE;  // 帧类型
			confirmFrame.check = ~confirmFrame.head;                // 数据校验
			if(wl_state_get())
			{
				rfm_frame_transmit(targetData.targetAddr, getWLParam()->channal, &confirmFrame, sizeof(confirmFrame));  // 定点发送
			}
			
			// 等待应答
			timCnt = 0;
			while(!usart2_it_rxBuf_isOk() && timCnt++ < BD_300MS_TIMEOUT);
			
			if(usart2_it_rxBuf_isOk())
			{
				RemoteFrame_t * pconfirmFrame = (RemoteFrame_t *)usart2_it_rxBuf();
				pconfirmFrame->check = 0;
				if((pconfirmFrame->head & FRAME_TYPE_MASK) == STATUS_FRAME_TYPE)
				{
					if(pconfirmFrame->head + pconfirmFrame->check == 0xFF)
					{
						// 配对成功
						lcd_str_gb2312_line(2, 0, 0, "数据验证...");
						//usart2_it_rxBuf_clear();
						//break;
						pairState = 2;
					}
					else
					{
						pairState = 0;
					}
				}
				usart2_it_rxBuf_clear();
			}
			
		}
		else if(2 == pairState) // 配对数据保存
		{
			
			repeat = 0;
			while(device_config_data_write(&targetData) && repeat++ < 3)
			{
				delay_ms(20);
			}
			
			if(repeat < 3)
			{
				lcd_str_gb2312_line(2, 0, 0, "配对成功...");
				break;
			}
			else
			{
				lcd_str_gb2312_line(2, 0, 0, "数据保存失败");
				lcd_str_gb2312_line(4, 0, 0, "请关机重试");
				while(1);
			}
		}
		led_toggle(LED_3);
	}
}
#endif
