/*************************************** Copyright (c)******************************************************
** File name            :   hal_usart.c
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
#include "stm32f10x.h"


#pragma pack(1)
typedef struct 
{
    /* data */
    uint8_t *read;
    uint8_t *write;
    uint8_t *buf;
    uint8_t bufSize;
	uint8_t ReadCnt;
}RingBufType;
#pragma pack()

typedef RingBufType * RingBuf_t;


int8_t ringBufInit(RingBuf_t RingBuf, uint8_t *buf, uint8_t bufSize)
{
    if (NULL == RingBuf || NULL == buf)
    {
        return -1;
    } 

    RingBuf->buf = buf;
    RingBuf->bufSize = bufSize;
	RingBuf->read = buf;
	RingBuf->write = buf;
	RingBuf->ReadCnt = 0;
	
    return 0;
}


uint8_t ringBufRead(RingBuf_t RingBuf, uint8_t *data)
{
	if (NULL == RingBuf || NULL == buf)
    {
        return -1;
    } 
	if(RingBuf->read > RingBuf->write)
	{
		RingBuf->ReadCnt++;
		return *RingBuf->read;
	}
    //return RingBuf->buf[RingBuf->read];
}

void ringBufFree(RingBuf_t RingBuf)
{

}

uint8_t ringBufReadFree(void)
{

}


void ringBufWrite(uint8_t data)
{
    ringBuf_t.buf[ringBuf_t.write++] = data;
}

uint8_t ringBufIsEmpty(void)
{
    return 0;
}

