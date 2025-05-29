#include "receiver.h"
#include "teleport.h"
#include "tf.h"
#include "control.h"
#include "taskinit.h"

uint8_t ReceiverReceiveBuff[25];//Receiver接收缓存数组
uint8_t ReceiverFifoBuff[25];//Receiver数据处理数组，缓存数组接收数据后保存至处理数组中

uint16_t ReceiverChannel[16];//Receiver接收通道数据
uint16_t ReceiverChannelPrevious[16]={1500,1500,1500,1500,1000,1500,1500,1500,1500,1500,1500,1500,1500,1500,1500,1500};//Receiver接收通道之前数据

SemaphoreHandle_t ReceiverSemaphore=NULL;//Receiver二值信号量
BaseType_t ReceiverHigherTaskSwitch;

ReceiverStatus ReceiverDataConvert(uint8_t *ReceiverBuff)
{
	uint8_t n = 0;
	if((ReceiverBuff[0] != 0x0F)||(ReceiverBuff[24] != 0x08)) return Receiver_ERR;
	if(ReceiverBuff[23] == 0x0C) return Receiver_NOSignal;
	ReceiverChannel[0] = ((uint16_t)ReceiverBuff[1])|((uint16_t)((ReceiverBuff[2]&0x07)<<8));
	ReceiverChannel[1] = ((uint16_t)((ReceiverBuff[2]&0xf8)>>3))|(((uint16_t)(ReceiverBuff[3]&0x3f))<<5);
	ReceiverChannel[2] = ((uint16_t)((ReceiverBuff[3]&0xc0)>>6))|((((uint16_t)ReceiverBuff[4])<<2))|(((uint16_t)(ReceiverBuff[5]&0x01))<<10);
	ReceiverChannel[3] = ((uint16_t)((ReceiverBuff[5]&0xfe)>>1))|(((uint16_t)(ReceiverBuff[6]&0x0f))<<7);
	ReceiverChannel[4] = ((uint16_t)((ReceiverBuff[6]&0xf0)>>4))|(((uint16_t)(ReceiverBuff[7]&0x7f))<<4);
	ReceiverChannel[5] = ((uint16_t)((ReceiverBuff[7]&0x80)>>7))|(((uint16_t)ReceiverBuff[8])<<1)|(((uint16_t)(ReceiverBuff[9]&0x03))<<9);
	ReceiverChannel[6] = ((uint16_t)((ReceiverBuff[9]&0xfc)>>2))|(((uint16_t)(ReceiverBuff[10]&0x1f))<<6);
	ReceiverChannel[7] = ((uint16_t)((ReceiverBuff[10]&0xe0)>>5))|(((uint16_t)(ReceiverBuff[11]))<<3);
	ReceiverChannel[8] = ((uint16_t)ReceiverBuff[12])|(((uint16_t)(ReceiverBuff[13]&0x07))<<8);
	ReceiverChannel[9] = ((uint16_t)((ReceiverBuff[13]&0xf8)>>3))|(((uint16_t)(ReceiverBuff[14]&0x3f))<<5);
	ReceiverChannel[10] = ((uint16_t)((ReceiverBuff[14]&0xc0)>>6))|(((uint16_t)ReceiverBuff[15])<<2)|(((uint16_t)(ReceiverBuff[16]&0x01))<<10);
	ReceiverChannel[11] = ((uint16_t)((ReceiverBuff[16]&0xfe)>>1))|(((uint16_t)(ReceiverBuff[17]&0x0f))<<7);
	ReceiverChannel[12] = ((uint16_t)((ReceiverBuff[17]&0xf0)>>4))|(((uint16_t)(ReceiverBuff[18]&0x7f))<<4);
	ReceiverChannel[13] = ((uint16_t)((ReceiverBuff[18]&0x80)>>7))|(((uint16_t)ReceiverBuff[19])<<1)|(((uint16_t)(ReceiverBuff[20]&0x03))<<9);
	ReceiverChannel[14] = ((uint16_t)((ReceiverBuff[20]&0xfc)>>2))|(((uint16_t)(ReceiverBuff[21]&0x1f))<<6);
	ReceiverChannel[15] = ((uint16_t)((ReceiverBuff[21]&0xe0)>>5))|(((uint16_t)ReceiverBuff[22])<<3);
	for(n=0;n<16;n++)
	{
		ReceiverChannel[n] = (ReceiverChannel[n]-352)*0.744+1000;
	}
	return Receiver_OK;
}

void ReceiverSolution(void)
{
	if(ReceiverChannel[4]<1400)//检测飞控停止工作信号
	{
		if(ReceiverChannelPrevious[4]>1450)
		{
			ControlStop();
		}
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,ReceiverChannel[0]);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,3000-ReceiverChannel[1]);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,ReceiverChannel[2]);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,ReceiverChannel[3]);
	}
	else if(ReceiverChannel[4]>1600)//检测飞控开始工作信号
	{
		if(ReceiverChannelPrevious[4]<1550)
		{
			ControlStart();
		}
	}
	//控制飞控飞行模式
	if(ReceiverChannel[5]<1400) FMUControlMode = FMU_Manual;
	else if(ReceiverChannel[5]<1600) 
	{
		FMUControlMode = FMU_Stable;
		if(FMUControlModePrevious != FMU_Stable) integtal_pitch = 0;
	}
	else 
	{
		FMUControlMode = FMU_Height;
		if(FMUControlModePrevious != FMU_Height) 
    {
      expected_height = GNSSData.alt;
      integtal_pitch = 0;
    }
	}
	//控制数传数据返回
	if(ReceiverChannel[6]<1400) 
	{
		vTaskSuspend(TeleportTransmit_TCB);
//		vTaskSuspend(TaskMonitor_TCB);
	}
	else if(ReceiverChannel[6]>1600) 
	{
		vTaskResume(TeleportTransmit_TCB);
//		vTaskResume(TaskMonitor_TCB);
	}
	//复制通道内容
	FMUControlModePrevious = FMUControlMode;
	memcpy(ReceiverChannelPrevious,ReceiverChannel,sizeof(ReceiverChannel));
}
