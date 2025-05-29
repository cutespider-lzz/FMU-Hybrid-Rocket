#ifndef __TELEPORT_H
#define __TELEPORT_H

#include "main.h"


extern UART_HandleTypeDef *	TeleHandle;

typedef enum
{
	DebugChannel = 1,
	TeleChannel
}PrintChannelSelect;

extern PrintChannelSelect PrintChannel;

extern uint8_t SendBuff[];

void InfoPrint(PrintChannelSelect PrintChannel,char * info);

#endif
