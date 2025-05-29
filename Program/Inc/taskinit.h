#ifndef __TASKINIT_H
#define __TASKINIT_H

#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "taskinit.h"
#include "stdio.h"
#include "imu.h"
#include "gnss.h"
#include "receiver.h"
#include "control.h"
#include "string.h"
#include "printf.h"
#include "teleport.h"


extern EventGroupHandle_t FMUCheckEvent;

extern IMUStatus IMURet;
extern GNSSStatus GNSSRet;
extern ReceiverStatus ReceiverRet;


//TaskCreate
void TaskCreate(void);

//LEDTwink
extern BaseType_t LEDTwink_Ret;
extern UBaseType_t LEDTwink_Prio;
extern TaskHandle_t LEDTwink_TCB;

void LEDTwink(void *pvParameters);

//FMUCheck
extern BaseType_t FMUCheck_Ret;
extern UBaseType_t FMUCheck_Prio;
extern TaskHandle_t FMUCheck_TCB;

void FMUCheck(void *pvParameters);

//RocketFlight
extern BaseType_t RocketFlight_Ret;
extern UBaseType_t RocketFlight_Prio;
extern TaskHandle_t RocketFlight_TCB;

void RocketFlight(void *pvParameters);

//TaskMonitor
extern BaseType_t TaskMonitor_Ret;
extern UBaseType_t TaskMonitor_Prio;
extern TaskHandle_t TaskMonitor_TCB;

void TaskMonitor(void *pvParameters);

//SDWrite
extern BaseType_t SDWrite_Ret;
extern UBaseType_t SDWrite_Prio;
extern TaskHandle_t SDWrite_TCB;

void SDWrite(void *pvParameters);

//IMUReceive
extern BaseType_t IMUReceive_Ret;
extern UBaseType_t IMUReceive_Prio;
extern TaskHandle_t IMUReceive_TCB;

void IMUReceive(void *pvParameters);

//GNSSReceive
extern BaseType_t GNSSReceive_Ret;
extern UBaseType_t GNSSReceive_Prio;
extern TaskHandle_t GNSSReceive_TCB;

void GNSSReceive(void *pvParameters);

//ReceiverReceive
extern BaseType_t ReceiverReceive_Ret;
extern UBaseType_t ReceiverReceive_Prio;
extern TaskHandle_t ReceiverReceive_TCB;

void ReceiverReceive(void *pvParameters);

//TeleportTransmit函数声明
extern BaseType_t TeleportTransmit_Ret;
extern UBaseType_t TeleportTransmit_Prio;
extern TaskHandle_t TeleportTransmit_TCB;

void TeleportTransmit(void *pvParameters);

#endif
