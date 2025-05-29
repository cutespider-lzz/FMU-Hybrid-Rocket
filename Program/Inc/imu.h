#ifndef __IMU_H
#define __IMU_H

#include "fatfs.h"
#include "cmsis_os.h"

typedef struct
{
	double acc_x,acc_y,acc_z;
	double gyr_x,gyr_y,gyr_z;
	double pitch,roll,yaw;
	double pressure,height;
	double temperature;
	double quaternion[4];
}IMUDateStruct;

typedef enum 
{
	IMU_OK = 0,
	IMU_Receive_ERR,
	IMU_Data_ERR
}IMUStatus;

extern uint8_t IMUReceiveBuff[];
extern uint8_t IMUFifoBuff[];

extern IMUDateStruct IMUData;

extern SemaphoreHandle_t IMUSemaphore;
extern BaseType_t IMUHigherTaskSwitch;

IMUStatus IMUDataConvert(uint8_t *DataBuff);

#endif
