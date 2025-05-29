#ifndef __GNSS_H
#define __GNSS_H

#include "fatfs.h"
#include "cmsis_os.h"
#include "stm32h7xx_hal.h"

#define GNSS_Z230 0
#define GNSS_WTGPS 1

typedef enum 
{
	GNSS_OK = 0,
	GNSS_Receive_ERR,
	GNSS_Data_ERR,
	GNSS_NOFIX,
	GNSS_FIX
}GNSSStatus;

typedef struct
{
	GNSSStatus GNSSSta;
	double lat,lon,alt;
	double velocity,angle,velocity_e,velocity_n;  //速度单位为m/s,angle单位为弧度
}GNSSDateStruct;

//卫星信息
typedef struct  
{										    
 	uint8_t num;		//卫星编号
	uint8_t eledeg;	//卫星仰角
	uint16_t azideg;	//卫星方位角
	uint8_t sn;		//信噪比		   
}nmea_slmsg;  
//UTC时间信息
typedef struct  
{										    
 	uint16_t year;	//年份
	uint8_t month;	//月份
	uint8_t date;	//日期
	uint8_t hour; 	//小时
	uint8_t min; 	//分钟
	uint8_t sec; 	//秒钟
}nmea_utc_time; 

//NMEA 0183 协议解析后数据存放结构体
typedef struct  
{										    
 	uint8_t svnum;					//可见卫星数
	nmea_utc_time utc;			//UTC时间
	nmea_slmsg slmsg[12];		//最多12颗卫星
	double latitude;				//纬度 分扩大100000倍,实际要除以100000
	uint8_t nshemi;					//北纬/南纬,N:北纬;S:南纬				  
	double longitude;			    //经度 分扩大100000倍,实际要除以100000
	uint8_t ewhemi;					//东经/西经,E:东经;W:西经
	uint8_t gpssta;					//GPS状态:0,未定位;1,非差分定位;2,差分定位;6,正在估算.				  
 	uint8_t posslnum;				//用于定位的卫星数,0~12.
 	uint8_t possl[12];				//用于定位的卫星编号
	uint8_t fixmode;					//定位类型:1,没有定位;2,2D定位;3,3D定位
	uint16_t pdop;					//位置精度因子 0~500,对应实际值0~50.0
	uint16_t hdop;					//水平精度因子 0~500,对应实际值0~50.0
	uint16_t vdop;					//垂直精度因子 0~500,对应实际值0~50.0 
	int altitude_int;			 	//海拔高度m
	double altitude;					//海拔高度，浮点型变量
	double speed;					//地面速率,放大了1000倍,实际除以10.单位:0.001公里/小时
	double angle;					//对地航向角，单位为°，扩大了1000倍
}nmea_msg; 

extern uint8_t GNSSReceiveBuff[];
extern uint8_t GNSSFifoBuff[];

extern UART_HandleTypeDef *GNSSHandle;

extern GNSSDateStruct GNSSData;
extern nmea_msg GNSS_msg;

extern SemaphoreHandle_t GNSSSemaphore;
extern BaseType_t GNSSHigherTaskSwitch;

#if GNSS_Z230

void GNSSInit(void);
void GNSS_UART_ReInit(uint32_t band);

#endif

int NMEA_Str2num(uint8_t *buf,uint8_t*dx);
void GPS_Analysis(nmea_msg *gpsx,uint8_t *buf);
void NMEA_GPGSV_Analysis(nmea_msg *gpsx,uint8_t *buf);
void NMEA_GPGGA_Analysis(nmea_msg *gpsx,uint8_t *buf);
void NMEA_GPGSA_Analysis(nmea_msg *gpsx,uint8_t *buf);
void NMEA_GPGSA_Analysis(nmea_msg *gpsx,uint8_t *buf);
void NMEA_GPRMC_Analysis(nmea_msg *gpsx,uint8_t *buf);
void NMEA_GPVTG_Analysis(nmea_msg *gpsx,uint8_t *buf);
void NMEA_GPGLL_Analysis(nmea_msg *gpsx,uint8_t *buf);

GNSSStatus GNSSDataConvert(uint8_t *DataBuff);
void GNSS_UART_ReInit(uint32_t band);
void GNSSInit(void);

#endif

