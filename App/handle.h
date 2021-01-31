/*************************************** Copyright (c)******************************************************
** File name            :   remote.h
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
#ifndef __HANDLE_H
#define __HANDLE_H
#include "stm32f10x.h"


/*  */
#define STATUS_LED          LED_2

/* 通讯帧 */
//#define FRAME_HEAD             0xD1   //<! 帧头
//#define FRAME_TAIL             0xA1   //<! 帧尾



/* 命令操作 */
#define LEFT_MV_FUNC           0x11   //<! 左牵
#define RIGHT_MV_FUNC          0x12   //<! 右牵
//#define UP_MV_FUNC             0x13   //<! 上升
//#define DOWN_MV_FUNC           0x14   //<! 下降
#define ACC_MV_FUNC            0x15   //<! 加速
#define DEC_MV_FUNC            0x16   //<! 减速
#define STOP_MV_FUNC           0x17   //<! 牵停
#define MAIN_STOP_FUNC         0x18   //<! 主停
#define CRUSH_UP_FUNC          0x19   //<! 碎升
#define CRUSH_DOWN_FUNC        0x1A   //<! 碎降


#define L_CH_UP_MV_FUNC             0x21   //<! 左站上升
#define L_CH_DOWN_MV_FUNC           0x22   //<! 左站下降
#define R_CH_UP_MV_FUNC             0x23   //<! 右站上升
#define R_CH_DOWN_MV_FUNC           0x24   //<! 右站下降



/* 配对 */
#define PAIR_REQUEST           0x31   //<! request
#define PAIR_COMPLETE          0x32   //<! complete
#define PAIR_ERROR             0x33   //<! ERROR


/********************************超时设置*********************************/		

//#define RMT_TICKS_PER_MISECOND      450000 					 //@MCU-M1  72MHz  100ms
//#define RMT_INC_TIMER               rmt_Timer++
//#define RMT_RESET_TIMER             rmt_Timer = 0


//#define RMT_200MS_TIMEOUT           2 * RMT_TICKS_PER_MISECOND
//#define RMT_300MS_TIMEOUT           3 * RMT_TICKS_PER_MISECOND
//#define RMT_1000MS_TIMEOUT          10 * RMT_TICKS_PER_MISECOND


#pragma pack(1)
typedef struct
{
	uint8_t opt;        // 帧头
	uint8_t addr_H;
	uint8_t addr_L;
	uint8_t key;
	uint8_t sum;
}Pair_t;

typedef union
{
	Pair_t pair;
}DataFrame_t;


/* 遥控帧 */
typedef struct 
{
	uint8_t opt;        // 帧头
	uint8_t check;
}RemoteFrame_t;


#pragma pack()




/* 函数声明 */
int8_t opt_handle_init(void);
int8_t keys_option_handle(void);
int8_t rmf_opt_transmit (uint8_t cmd);

#endif

