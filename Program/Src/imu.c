#include "main.h"
#include "imu.h"
#include "stdio.h"

uint8_t IMUReceiveBuff[60];//IMU接收缓存数组
uint8_t IMUFifoBuff[60];//IMU数据处理数组，缓存数组接收数据后保存至处理数组中

IMUDateStruct IMUData;//IMU数据存储结构体

SemaphoreHandle_t IMUSemaphore=NULL;//IMU二值信号量
BaseType_t IMUHigherTaskSwitch;

IMUStatus IMUDataConvert(uint8_t *DataBuff)
{
	uint8_t sum,i;
	uint8_t *point;
	int16_t tran_int16;
//	int32_t tran_int32;
	if(DataBuff[0]!=0x55 ) return IMU_Receive_ERR;
	if(DataBuff[1]!=0x51 ) return IMU_Receive_ERR;
	//计算加速度,单位为m/s^2
	point = DataBuff;
	sum=0;
	for(i=0;i<10;i++)
	{
		sum += *point;
		point++;
	}
	if(sum != *point) return IMU_Data_ERR;
	point = DataBuff + 2;
	tran_int16 = (((int16_t)point[1])<<8|(int16_t)point[0]);
	IMUData.acc_x = (double)tran_int16*0.00478515625;
	tran_int16 = (((int16_t)point[3])<<8|(int16_t)point[2]);
	IMUData.acc_y = (double)tran_int16*0.00478515625;
	tran_int16 = (((int16_t)point[5])<<8|(int16_t)point[4]);
	IMUData.acc_z = (double)tran_int16*0.00478515625;
	//计算角速度,单位为°/s
	point = DataBuff+11;
	sum=0;
	for(i=0;i<10;i++)
	{
		sum += *point;
		point++;
	}
	if(sum != *point) return IMU_Data_ERR;
	point = DataBuff + 13;
	tran_int16 = (((int16_t)point[1])<<8|(int16_t)point[0]);
	IMUData.gyr_x = (double)tran_int16*0.06103515625;
	tran_int16 = (((int16_t)point[3])<<8|(int16_t)point[2]);
	IMUData.gyr_y = (double)tran_int16*0.06103515625;
	tran_int16 = (((int16_t)point[5])<<8|(int16_t)point[4]);
	IMUData.gyr_z = (double)tran_int16*0.06103515625;
	//计算姿态角,单位为°
	point = DataBuff+22;
	sum=0;
	for(i=0;i<10;i++)
	{
		sum += *point;
		point++;
	}
	if(sum != *point) return IMU_Data_ERR;
	point = DataBuff + 24;
	tran_int16 = (((int16_t)point[1])<<8|(int16_t)point[0]);
	IMUData.pitch = (double)tran_int16*0.0054931640625;
	tran_int16 = (((int16_t)point[3])<<8|(int16_t)point[2]);
	IMUData.roll = (double)tran_int16*0.0054931640625;
	tran_int16 = (((int16_t)point[5])<<8|(int16_t)point[4]);
	IMUData.yaw = (double)tran_int16*0.0054931640625;
	//计算气压,单位为Pa和cm
	point = DataBuff+33;
	sum=0;
	for(i=0;i<10;i++)
	{
		sum += *point;
		point++;
	}
	if(sum != *point) return IMU_Data_ERR;
	point = DataBuff + 35;
	IMUData.pressure = ((uint32_t)point[3]<<24)|((uint32_t)point[2]<<16)|((uint32_t)point[1]<<8)|((uint32_t)point[0]);
	IMUData.height = (((uint32_t)point[7]<<24)|((uint32_t)point[6]<<16)|((uint32_t)point[5]<<8)|((uint32_t)point[4]))*0.01;
	//计算四元数
	point = DataBuff+44;
	sum=0;
	for(i=0;i<10;i++)
	{
		sum += *point;
		point++;
	}
	if(sum != *point) return IMU_Data_ERR;
	point = DataBuff + 46;
	tran_int16 = (((int16_t)point[1])<<8|(int16_t)point[0]);
	IMUData.quaternion[0] = (double)tran_int16*0.000030517578125;
	tran_int16 = (((int16_t)point[3])<<8|(int16_t)point[2]);
	IMUData.quaternion[1] = (double)tran_int16*0.000030517578125;
	tran_int16 = (((int16_t)point[5])<<8|(int16_t)point[4]);
	IMUData.quaternion[2] = (double)tran_int16*0.000030517578125;
	tran_int16 = (((int16_t)point[7])<<8|(int16_t)point[6]);
	IMUData.quaternion[3] = (double)tran_int16*0.000030517578125;
	printf("%0.4f  %0.4f  %0.4f  ",IMUData.acc_x,IMUData.acc_y,IMUData.acc_z);
	printf("%0.4f  %0.4f  %0.4f  ",IMUData.gyr_x,IMUData.gyr_y,IMUData.gyr_z);
	printf("%0.4f  %0.4f  %0.4f  ",IMUData.pitch,IMUData.roll,IMUData.yaw);
	printf("%0.4f  %0.4f  ",IMUData.pressure,IMUData.height);
	printf("%0.4f  %0.4f  %0.4f  %0.4f\r\n",IMUData.quaternion[0],IMUData.quaternion[1],IMUData.quaternion[2],IMUData.quaternion[3]);
	return IMU_OK;
}
