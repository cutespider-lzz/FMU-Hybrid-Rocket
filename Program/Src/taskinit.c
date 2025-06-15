#include "taskinit.h"
#include "main.h"
#include "tf.h"
#include "navigation.h"
#include "teleport.h"
#include "control.h"




EventGroupHandle_t FMUCheckEvent; 

IMUStatus IMURet;
GNSSStatus GNSSRet;
ReceiverStatus ReceiverRet;

//TaskCreate函数声明
void TaskCreate(void)
{
	FMUCheckEvent = xEventGroupCreate();//创建自检事件标志位
	ReceiverSemaphore = xSemaphoreCreateBinary();
	IMUSemaphore = xSemaphoreCreateBinary();
	GNSSSemaphore = xSemaphoreCreateBinary();
	ControlSemaphore = xSemaphoreCreateBinary();
	//Create LEDTwink
	LEDTwink_Ret = xTaskCreate((TaskFunction_t)LEDTwink,"LEDTwink",32,(void *)1,LEDTwink_Prio,(TaskHandle_t *)(&LEDTwink_TCB));
	if(LEDTwink_Ret == pdPASS) InfoPrint(PrintChannel,"LEDTwink creat successfully!\r\n");
	else InfoPrint(PrintChannel,"LEDTwink creat failed!\r\n");
	//Create FMUCheck
	FMUCheck_Ret = xTaskCreate((TaskFunction_t)FMUCheck,"FMUCheck",128,(void *)1,FMUCheck_Prio,(TaskHandle_t *)(&FMUCheck_TCB));
	if(FMUCheck_Ret == pdPASS) InfoPrint(PrintChannel,"FMUCheck creat successfully!\r\n");
	else InfoPrint(PrintChannel,"FMUCheck creat failed!\r\n");
	//Create RocketFlight
	RocketFlight_Ret = xTaskCreate((TaskFunction_t)RocketFlight,"RocketFlight",256,(void *)1,RocketFlight_Prio,(TaskHandle_t *)(&RocketFlight_TCB));
	if(RocketFlight_Ret == pdPASS) InfoPrint(PrintChannel,"RocketFlight creat successfully!\r\n");
	else InfoPrint(PrintChannel,"RocketFlight creat failed!\r\n");
	//Create TaskMonitor
//	TaskMonitor_Ret = xTaskCreate((TaskFunction_t)TaskMonitor,"TaskMonitor",256,(void *)1,TaskMonitor_Prio,(TaskHandle_t *)(&TaskMonitor_TCB));
//	if(TaskMonitor_Ret == pdPASS) InfoPrint(PrintChannel,"TaskMonitor creat successfully!\r\n");
//	else InfoPrint(PrintChannel,"TaskMonitor creat failed!\r\n");
	//Create SDWrite
	SDWrite_Ret = xTaskCreate((TaskFunction_t)SDWrite,"SDWrite",200,(void *)1,SDWrite_Prio,(TaskHandle_t *)(&SDWrite_TCB));
	if(SDWrite_Ret == pdPASS) InfoPrint(PrintChannel,"SDWrite creat successfully!\r\n");
	else InfoPrint(PrintChannel,"SDWrite creat failed!\r\n");
	//Create IMUReceive
	IMUReceive_Ret = xTaskCreate((TaskFunction_t)IMUReceive,"IMUReceive",256,(void *)1,IMUReceive_Prio,(TaskHandle_t *)(&IMUReceive_TCB));
	if(IMUReceive_Ret == pdPASS) InfoPrint(PrintChannel,"IMUReceive creat successfully!\r\n");
	else InfoPrint(PrintChannel,"IMUReceive creat failed!\r\n");
	//Create GNSSReceive
//	GNSSReceive_Ret = xTaskCreate((TaskFuncc creat failed!\r\n");
	//Create ReceiverReceive
//	ReceiverReceive_Ret = xTaskCreate((TaskFunction_t)ReceiverReceive,"ReceiverReceive",256,(void *)1,ReceiverReceive_Prio,(TaskHandle_t *)(&ReceiverReceive_TCB));
//	if(ReceiverReceive_Ret == pdPASS) InfoPrint(PrintChannel,"ReceiverReceive creat successfully!\r\n");
//	else InfoPrint(PrintChannel,"ReceiverReceive creat failed!\r\n");
	//Create TeleportTransmit
//	TeleportTransmit_Ret = xTaskCreate((TaskFunction_t)TeleportTransmit,"TeleportTransmit",256,(void *)1,TeleportTransmit_Prio,(TaskHandle_t *)(&TeleportTransmit_TCB));
//	if(TeleportTransmit_Ret == pdPASS) InfoPrint(PrintChannel,"TeleportTransmit creat successfully!\r\n");
//	else InfoPrint(PrintChannel,"TeleportTransmit creat failed!\r\n");
	//Start
	vTaskStartScheduler();
	while(1) ;
}

//LEDTwink函数声明
BaseType_t LEDTwink_Ret;
UBaseType_t LEDTwink_Prio=4;
TaskHandle_t LEDTwink_TCB;

void LEDTwink(void *pvParameters)
{
	while(1)
	{
		HAL_GPIO_TogglePin(SIGNAL_GPIO_Port,SIGNAL_Pin);
		vTaskDelay(250);
	}
}

//FMUCheck函数声明
BaseType_t FMUCheck_Ret;
UBaseType_t FMUCheck_Prio=3;
TaskHandle_t FMUCheck_TCB;

void FMUCheck(void *pvParameters)
{
	xEventGroupClearBits(FMUCheckEvent,0xFFFF);
	while(1)
	{
		if(HAL_GPIO_ReadPin(TRIGGER_GPIO_Port,TRIGGER_Pin)==GPIO_PIN_RESET)
		{
			__HAL_TIM_SET_COUNTER(&htim6,0);
			ControlTime = 0;
//					FileCreate();
			HAL_TIM_Base_Start_IT(&htim6);
			xEventGroupSetBits(FMUCheckEvent,0xFF);
			vTaskSuspend(NULL);
		}
		vTaskDelay(10);
	}
	//调试时使用禁用GPS
//	xEventGroupSetBits(FMUCheckEvent,0xFF);
//	vTaskSuspend(NULL);
	//
//	while(1)
//	{
//		xSemaphoreTake(GNSSSemaphore,portMAX_DELAY);
//		GNSSRet = GNSSDataConvert(GNSSFifoBuff);
//		if(GNSSRet == GNSS_FIX) 
//		{
//			InfoPrint(PrintChannel,"GNSS is ready!\r\n");
//			xEventGroupSetBits(FMUCheckEvent,0xFF);
//			while(1)
//			{
//				if(HAL_GPIO_ReadPin(TRIGGER_GPIO_Port,TRIGGER_Pin)==GPIO_PIN_RESET)
//				{
//					__HAL_TIM_SET_COUNTER(&htim6,0);
//					ControlTime = 0;
////					FileCreate();
//					HAL_TIM_Base_Start_IT(&htim6);
//					xEventGroupSetBits(FMUCheckEvent,0xFF);
//					vTaskSuspend(NULL);
//				}
//			}
//		}
//		else
//		{
//			InfoPrint(PrintChannel,"GNSS is preparing!\r\n");
//		}
//	}
}

//RocketFlight函数声明
BaseType_t RocketFlight_Ret;
UBaseType_t RocketFlight_Prio=20;
TaskHandle_t RocketFlight_TCB;

void RocketFlight(void *pvParameters)
{
	xEventGroupWaitBits(FMUCheckEvent,0x08,pdFALSE,pdTRUE,portMAX_DELAY);
	while(1)
	{
	  
		xSemaphoreTake(ControlSemaphore,portMAX_DELAY);
		
		if(ControlTime >= 5) HAL_GPIO_WritePin(BAT1_GPIO_Port,BAT1_Pin,GPIO_PIN_SET);//5s一口爆炸螺栓点燃
		if(ControlTime >= 10) HAL_GPIO_WritePin(BAT2_GPIO_Port,BAT2_Pin,GPIO_PIN_SET);//10s二口爆炸螺栓点燃
		if(ControlTime <= 20) //飞控运行时间
		{
			
			sprintf((char *)StorageBuff,"time: %0.2f ax: %0.2f ay: %0.2f az: %0.2f gx: %0.2f gy: %0.2f gz: %0.2f p: %0.2f r: %0.2f y: %0.2f pre: %0.2f h: %0.2f  lon:%0.8f  lat:%0.8f  alt:%0.2f\n",\
			ControlTime,IMUData.acc_x,IMUData.acc_y,IMUData.acc_z,IMUData.gyr_x,IMUData.gyr_y,IMUData.gyr_z,IMUData.pitch,IMUData.roll,IMUData.yaw,IMUData.pressure,IMUData.height,0,0,0);
			f_printf(&SDFile,(char *)StorageBuff);
		}
		else 
		{
			HAL_TIM_Base_Stop_IT(&htim6);
//			f_close(&SDFile);
			FileClose();
			vTaskSuspend(NULL);
		}
		
	}
}

//TaskMonitor函数声明
BaseType_t TaskMonitor_Ret;
UBaseType_t TaskMonitor_Prio=30;
TaskHandle_t TaskMonitor_TCB;

void TaskMonitor(void *pvParameters)
{
	char InfoBuffer[600];
	xEventGroupWaitBits(FMUCheckEvent,0x10,pdFALSE,pdTRUE,portMAX_DELAY);
	while(1)
	{
		taskENTER_CRITICAL();
		vTaskList(InfoBuffer);
		InfoPrint(DebugChannel,"---------------------------------------------\r\n");
		InfoPrint(DebugChannel,InfoBuffer);
		InfoPrint(DebugChannel,"---------------------------------------------\r\n");
		taskEXIT_CRITICAL();
		vTaskDelay(5000);
		taskENTER_CRITICAL();
		vTaskGetRunTimeStats(InfoBuffer);
		InfoPrint(DebugChannel,"---------------------------------------------\r\n");
		InfoPrint(DebugChannel,InfoBuffer);
		InfoPrint(DebugChannel,"---------------------------------------------\r\n");
		taskEXIT_CRITICAL();
		vTaskDelay(5000);
	}
}

//SDwrite函数声明
BaseType_t SDWrite_Ret;
UBaseType_t SDWrite_Prio=25;
TaskHandle_t SDWrite_TCB;

void SDWrite(void *pvParameters)
{
//	xEventGroupWaitBits(FMUCheckEvent,0x10,pdFALSE,pdTRUE,portMAX_DELAY);
	SDRet = f_mount(&SDFatFS,SDPath,1);
	if(SDRet != FR_OK)
	{
		switch(SDRet)
		{
			case FR_NO_FILESYSTEM:
				SDRet = f_mkfs(SDPath,FM_FAT32,0,work,_MAX_SS);
				if(SDRet==FR_OK) 
				{
					SDRet = f_mount(&SDFatFS,SDPath,1);
					if(SDRet == FR_OK) InfoPrint(PrintChannel,"TF mount successfully!\r\n");
					else InfoPrint(PrintChannel,"TF mount successfully!\r\n");
				}
				break;
			default:
				InfoPrint(PrintChannel,"TF mount failed!\r\n");
				while(1) ;
		}
	}
	else if(SDRet == FR_OK) 
	{
		InfoPrint(PrintChannel,"TF mount successfully!\r\n");
//		f_open(&SDFile,"TF test.txt",FA_WRITE|FA_OPEN_APPEND);
//		f_printf(&SDFile,"TF write test!\n");
//		f_close(&SDFile);
		FileCreate();
	}
	vTaskSuspend(NULL);
	while(1)
	{

	}
}

//IMUReceive函数声明
BaseType_t IMUReceive_Ret;
UBaseType_t IMUReceive_Prio=9;
TaskHandle_t IMUReceive_TCB;

void IMUReceive(void *pvParameters)
{
	HAL_UART_Receive_DMA(&huart2,IMUReceiveBuff,55);
	__HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);
//	xEventGroupWaitBits(FMUCheckEvent,0x10,pdFALSE,pdTRUE,portMAX_DELAY);
	while(1)
	{
		xSemaphoreTake(IMUSemaphore,portMAX_DELAY);
		IMURet = IMUDataConvert(IMUFifoBuff);
		if(IMURet == IMU_OK)
		{
//			InfoPrint(PrintChannel,"%0.4f  %0.4f  %0.4f  ",IMUData.acc_x,IMUData.acc_y,IMUData.acc_z);
//			InfoPrint(PrintChannel,"%0.4f  %0.4f  %0.4f  ",IMUData.gyr_x,IMUData.gyr_y,IMUData.gyr_z);
//			InfoPrint(PrintChannel,"%0.4f  %0.4f  %0.4f  ",IMUData.pitch,IMUData.roll,IMUData.yaw);
//			InfoPrint(PrintChannel,"%0.4f  %0.4f  ",IMUData.pressure,IMUData.height);
//			InfoPrint(PrintChannel,"%0.4f  %0.4f  %0.4f  %0.4f\r\n",IMUData.quaternion[0],IMUData.quaternion[1],IMUData.quaternion[2],IMUData.quaternion[3]);
			//体坐标系到惯性坐标系
//			printf("%0.4f  %0.4f  %0.4f  %0.4f  %0.4f  %0.4f\r\n",a_e,a_n,a_u,p_e,p_n,p_u);
			
		}
		else 
		{
			InfoPrint(PrintChannel,"IMU error!\r\n");
		}
		if(ControlTime <= 3)
		{
			ServoSet(ServoChannel_1,0);
			ServoSet(ServoChannel_2,0);
			ServoSet(ServoChannel_3,0);
			ServoSet(ServoChannel_4,0);
		}
		else
		{
			FixedWingControl();//舵机控制程序运行
		}
	}
}

//GNSSReceive函数声明
BaseType_t GNSSReceive_Ret;
UBaseType_t GNSSReceive_Prio=17;
TaskHandle_t GNSSReceive_TCB;

void GNSSReceive(void *pvParameters)
{
	GNSSInit();//初始化GNSS串口波特率
	HAL_UART_Receive_DMA(GNSSHandle,GNSSReceiveBuff,1024);
	__HAL_UART_ENABLE_IT(GNSSHandle,UART_IT_IDLE);
	xEventGroupWaitBits(FMUCheckEvent,0x02,pdFALSE,pdTRUE,portMAX_DELAY);
	while(1)
	{
		xSemaphoreTake(GNSSSemaphore,1500);
		GNSSRet = GNSSDataConvert(GNSSFifoBuff);
		if(GNSSRet == GNSS_FIX)
		{
	
		}
		else 
		{
			InfoPrint(PrintChannel,"GNSS error!\r\n");
		}
	}
}


//TeleportTransmit函数声明
BaseType_t TeleportTransmit_Ret;
UBaseType_t TeleportTransmit_Prio=10;
TaskHandle_t TeleportTransmit_TCB;

void TeleportTransmit(void *pvParameters)
{
	uint32_t voltage_uint32,current_uint32;
	double voltage,current;
	xEventGroupWaitBits(FMUCheckEvent,0x04,pdFALSE,pdTRUE,portMAX_DELAY);
	while(1)
	{
		HAL_ADC_Start(&hadc3);
		HAL_ADC_PollForConversion(&hadc3,0x10);
		current_uint32 = HAL_ADC_GetValue(&hadc3);
		HAL_ADC_Start(&hadc3);
		HAL_ADC_PollForConversion(&hadc3,0x10);
		voltage_uint32 = HAL_ADC_GetValue(&hadc3);
		voltage = voltage_uint32*0.00042802;
		current = current_uint32*0.00005355;
		HAL_ADC_Stop(&hadc3);
		sprintf(SendBuff,"%s：  lon:%0.8f  lat:%0.8f  alt:%0.8f  vol:%0.2f\r\n",Rocket,GNSSData.lon,GNSSData.lat,GNSSData.alt,voltage);
		InfoPrint(PrintChannel,SendBuff);
		vTaskDelay(1000);
	}
}

