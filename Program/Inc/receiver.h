#ifndef __RECEIVER_H
#define __RECEIVER_H

#include "fatfs.h"
#include "cmsis_os.h"


typedef enum
{
	Receiver_ERR,
	Receiver_OK,
	Receiver_NOSignal
}ReceiverStatus;

extern uint8_t ReceiverReceiveBuff[];//Receiver接收缓存数组
extern uint8_t ReceiverFifoBuff[];//Receiver数据处理数组，缓存数组接收数据后保存至处理数组中

extern uint16_t ReceiverChannel[];//Receiver接收通道数据
extern uint16_t ReceiverChannelPrevious[];//Receiver接收通道之前数据

extern SemaphoreHandle_t ReceiverSemaphore;//Receiver二值信号量
extern BaseType_t ReceiverHigherTaskSwitch;

ReceiverStatus ReceiverDataConvert(uint8_t *ReceiverChannel);
void ReceiverSolution(void);

#endif
