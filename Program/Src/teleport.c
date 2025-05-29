#include "teleport.h"
#include "string.h"
#include "printf.h"
#include "stdio.h"

UART_HandleTypeDef *TeleHandle = &huart8;

PrintChannelSelect PrintChannel = TeleChannel;

uint8_t SendBuff[100];

void InfoPrint(PrintChannelSelect PrintChannel,char * info)//调试接口数据输出
{
	if(PrintChannel==DebugChannel)
	{
		printf("%s",info);
//		HAL_UART_Transmit_DMA(&huart1,info,strlen(info));
	}
	else if(PrintChannel==TeleChannel)
	{
		HAL_UART_Transmit_DMA(TeleHandle,info,strlen(info));
	}
}
