#ifndef __CONTROL_H
#define __CONTROL_H

#include "fatfs.h"
#include "cmsis_os.h"

typedef enum
{
	ServoChannel_1 = 0,
	ServoChannel_2,
	ServoChannel_3,
	ServoChannel_4,
	ServoChannel_5,
	ServoChannel_6,
	ServoChannel_7,
	ServoChannel_8,
}ServoChannel;

typedef enum
{
	FMU_Manual,
	FMU_Stable,
	FMU_Height
}FMUControlModeSelect;

extern SemaphoreHandle_t ControlSemaphore;//控制率计算二值信号量
extern BaseType_t ControlHigherTaskSwitch;

extern double ControlTime;
extern const double ControlDt;

extern const double Kp_roll,Kd_roll,Kp_pitch,Kd_pitch,Kp_yaw,Kd_yaw;//控制率参数
extern double expected_roll,expected_pitch,expected_yaw,expected_height;//各通道期望值
extern double servo_roll,servo_pitch,servo_yaw;//对应通道角度
extern double integtal_pitch;

void ControlStart(void);//飞控开始工作初始化
void ControlUpdata(void);//飞控参数更新
void ControlStop(void);//飞控结束工作

extern FMUControlModeSelect FMUControlMode;
extern FMUControlModeSelect FMUControlModePrevious;

void ServoSet(ServoChannel channel,double angle);
void FixedWingControl(void);

#endif
