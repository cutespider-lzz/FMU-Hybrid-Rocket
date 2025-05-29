#include "gnss.h"
#include "stdio.h"
#include "string.h"
#include "math.h"

uint8_t GNSSReceiveBuff[1024];
uint8_t GNSSFifoBuff[1024];

UART_HandleTypeDef *GNSSHandle;

GNSSDateStruct GNSSData;
nmea_msg GNSS_msg;

SemaphoreHandle_t GNSSSemaphore;
BaseType_t GNSSHigherTaskSwitch;

void GNSSInit(void)
{
	//Z230对应初始化代码
	#if GNSS_Z230
		GNSSHandle = &huart3;
		HAL_UART_Transmit(GNSSHandle,"$CFGLOAD\r\n",strlen("$CFGLOAD\r\n"),0xFFFF);
		GNSS_UART_ReInit(230400);	
	#elif GNSS_WTGPS
		GNSSHandle = &huart6;
		HAL_UART_Transmit(GNSSHandle,"log g05hz",strlen("log g05hz"),0xFFFF);
	#endif
}

void GNSS_UART_ReInit(uint32_t band)//GNSS模块重初始化
{
	HAL_UART_DeInit(&huart3);
	huart3.Instance = USART3;
  huart3.Init.BaudRate = band;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
}


GNSSStatus GNSSDataConvert(uint8_t *DataBuff)
{
	double angle,velocity;
	GPS_Analysis(&GNSS_msg,DataBuff);
	if(GNSS_msg.gpssta == 'A') 
	{
		GNSSData.GNSSSta = GNSS_FIX;
		GNSSData.lat = GNSS_msg.latitude;
		GNSSData.lon = GNSS_msg.longitude;
		GNSSData.alt = GNSS_msg.altitude;
		GNSSData.angle = GNSS_msg.angle*0.0174532922222222;
		velocity = GNSS_msg.speed*0.2777777777777778;
		GNSSData.velocity = velocity;
		GNSSData.velocity_n = velocity*cos(GNSSData.angle);
		GNSSData.velocity_e = velocity*sin(GNSSData.angle);
		return GNSS_FIX;
	}
	else
	{
		GNSSData.GNSSSta = GNSS_NOFIX;
		GNSSData.lat = 0;
		GNSSData.lon = 0;
		GNSSData.alt = 0;
		angle = 0;
		velocity = 0;
		GNSSData.velocity = 0;
		GNSSData.velocity_n = 0;
		GNSSData.velocity_e = 0;
		return GNSS_NOFIX;
	}
} 


uint8_t NMEA_Comma_Pos(uint8_t *buf,uint8_t cx)
{	 		    
	uint8_t *p=buf;
	while(cx)
	{		 
		if(*buf=='*'||*buf<' '||*buf>'z')return 0XFF;//遇到'*'或者非法字符,则不存在第cx个逗号
		if(*buf==',')cx--;
		buf++;
	}
	return buf-p;	 
}
//m^n函数
//返回值:m^n次方.
uint32_t NMEA_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}

//str转换为数字,以','或者'*'结束
//buf:数字存储区
//dx:小数点位数,返回给调用函数
//返回值:转换后的数值
int NMEA_Str2num(uint8_t *buf,uint8_t*dx)
{
	uint8_t *p=buf;
	uint32_t ires=0,fres=0;
	uint8_t ilen=0,flen=0,i;
	uint8_t mask=0;
	int res;
	while(1) //得到整数和小数的长度
	{
		if(*p=='-'){mask|=0X02;p++;}//是负数
		if(*p==','||(*p=='*'))break;//遇到结束了
		if(*p=='.'){mask|=0X01;p++;}//遇到小数点了
		else if(*p>'9'||(*p<'0'))	//有非法字符
		{	
			ilen=0;
			flen=0;
			break;
		}	
		if(mask&0X01)flen++;
		else ilen++;
		p++;
	}
	if(mask&0X02)buf++;	//去掉负号
	for(i=0;i<ilen;i++)	//得到整数部分数据
	{  
		ires+=NMEA_Pow(10,ilen-1-i)*(buf[i]-'0');
	}
	if(flen>5)flen=5;	//最多取5位小数
	*dx=flen;	 		//小数点位数
	for(i=0;i<flen;i++)	//得到小数部分数据
	{  
		fres+=NMEA_Pow(10,flen-1-i)*(buf[ilen+1+i]-'0');
	} 
	res=ires*NMEA_Pow(10,flen)+fres;
	if(mask&0X02)res=-res;		   
	return res;
}	  							 
//分析GPGSV信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GPGSV_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
	uint8_t *p,*p1,dx;
	uint8_t len,i,j,slx=0;
	uint8_t posx;   	 
	p=buf;
	p1=(uint8_t*)strstr((const char *)p,"$GPGSV");
	len=p1[7]-'0';								//得到GPGSV的条数
	posx=NMEA_Comma_Pos(p1,3); 					//得到可见卫星总数
	if(posx!=0XFF)gpsx->svnum=NMEA_Str2num(p1+posx,&dx);
	for(i=0;i<len;i++)
	{	 
		p1=(uint8_t*)strstr((const char *)p,"$GPGSV");  
		for(j=0;j<4;j++)
		{	  
			posx=NMEA_Comma_Pos(p1,4+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].num=NMEA_Str2num(p1+posx,&dx);	//得到卫星编号
			else break; 
			posx=NMEA_Comma_Pos(p1,5+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].eledeg=NMEA_Str2num(p1+posx,&dx);//得到卫星仰角 
			else break;
			posx=NMEA_Comma_Pos(p1,6+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].azideg=NMEA_Str2num(p1+posx,&dx);//得到卫星方位角
			else break; 
			posx=NMEA_Comma_Pos(p1,7+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].sn=NMEA_Str2num(p1+posx,&dx);	//得到卫星信噪比
			else break;
			slx++;	   
		}   
 		p=p1+1;//切换到下一个GPGSV信息
	}   
}
//分析GPGGA信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GPGGA_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
	uint8_t *p1,dx;			 
	uint8_t posx;    
//	p1=(uint8_t*)strstr((const char *)buf,"$GPGGA");
	p1=(uint8_t*)strstr((const char *)buf,"GGA");
	posx=NMEA_Comma_Pos(p1,9);							
						
	if(posx!=0XFF)
	{
		gpsx->altitude_int=NMEA_Str2num(p1+posx,&dx); //得到海拔高度
		gpsx->altitude = gpsx->altitude_int/pow(10,dx);
	}
	posx=NMEA_Comma_Pos(p1,7);								
	if(posx!=0XFF)gpsx->posslnum=NMEA_Str2num(p1+posx,&dx);  //得到卫星数目
}
//分析GPGSA信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GPGSA_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
	uint8_t *p1,dx;			 
	uint8_t posx; 
	uint8_t i;   
	p1=(uint8_t*)strstr((const char *)buf,"GSA");
	posx=NMEA_Comma_Pos(p1,2);								//得到定位类型
	if(posx!=0XFF)gpsx->fixmode=NMEA_Str2num(p1+posx,&dx);	
	for(i=0;i<12;i++)										//得到定位卫星编号
	{
		posx=NMEA_Comma_Pos(p1,3+i);					 
		if(posx!=0XFF)gpsx->possl[i]=NMEA_Str2num(p1+posx,&dx);
		else break; 
	}				  
	posx=NMEA_Comma_Pos(p1,15);								//得到PDOP位置精度因子
	if(posx!=0XFF)gpsx->pdop=NMEA_Str2num(p1+posx,&dx);  
	posx=NMEA_Comma_Pos(p1,16);								//得到HDOP位置精度因子
	if(posx!=0XFF)gpsx->hdop=NMEA_Str2num(p1+posx,&dx);  
	posx=NMEA_Comma_Pos(p1,17);								//得到VDOP位置精度因子
	if(posx!=0XFF)gpsx->vdop=NMEA_Str2num(p1+posx,&dx);  
}
//分析GPRMC信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GPRMC_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
	uint8_t *p1,dx,*tran;			 
	uint8_t posx;     
//	float rs;  
	uint32_t temp;
	int itemp;
	double lat,lon,point;
	p1=(uint8_t*)strstr((const char *)buf,"RMC");//"$GPRMC",经常有&和GPRMC分开的情况,故只判断GPRMC.
	posx=NMEA_Comma_Pos(p1,1);								//得到UTC时间
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx)/NMEA_Pow(10,dx);	 	//得到UTC时间,去掉ms
		
		gpsx->utc.hour=temp/10000;
		gpsx->utc.min=(temp/100)%100;
		gpsx->utc.sec=temp%100;	
	}
	posx=NMEA_Comma_Pos(p1,2);								//得到接收机状态
	if(posx!=0XFF)
	{
		gpsx->gpssta=*(p1+posx);
	}
	else gpsx->gpssta='V';
	posx=NMEA_Comma_Pos(p1,3);								//得到纬度
	if(posx!=0XFF)
	{	
//		temp=NMEA_Str2num(p1+posx,&dx);		 	 
//		gpsx->latitude=temp/NMEA_Pow(10,dx+2);	//得到°
//		rs=temp%NMEA_Pow(10,dx+2);				//得到'		 
//		gpsx->latitude=gpsx->latitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;//转换为° 
		tran = p1+posx;
		lat = (*tran++ - 0x30)*10;
		lat += (*tran++ - 0x30);
		lat += (*tran++ - 0x30)/6.0;
		lat += (*tran++ - 0x30)/60.0;
		tran++;
		point = 0.1;
		while(1)
		{
			lat += (*tran++ - 0x30)*point/60.0;
			if(*tran == ',') break;
			point *= 0.1;
		}
		gpsx->latitude = lat;
	}
	posx=NMEA_Comma_Pos(p1,4);								//南纬还是北纬 
	if(posx!=0XFF)
	{
		gpsx->nshemi=*(p1+posx);
	}			 
 	posx=NMEA_Comma_Pos(p1,5);								//得到经度
	if(posx!=0XFF)
	{												  
//		temp=NMEA_Str2num(p1+posx,&dx);		 	 
//		gpsx->longitude=temp/NMEA_Pow(10,dx+2);	//得到°
//		rs=temp%NMEA_Pow(10,dx+2);				//得到'		 
//		gpsx->longitude=gpsx->longitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;//转换为° 
		tran = p1+posx;
		lon = (*tran++ - 0x30)*100;
		lon += (*tran++ - 0x30)*10;
		lon += (*tran++ - 0x30);
		lon += (*tran++ - 0x30)/6.0;
		lon += (*tran++ - 0x30)/60.0;
		tran++;
		point = 0.1;
		while(1)
		{
			lon += (*tran++ - 0x30)*point/60.0;
			if(*tran == ',') break;
			point *= 0.1;
		}
		gpsx->longitude = lon;
	}
	posx=NMEA_Comma_Pos(p1,6);								//东经还是西经
	if(posx!=0XFF)
	{

		gpsx->ewhemi=*(p1+posx);		 
	}	
	posx=NMEA_Comma_Pos(p1,7);								//得到速率大小
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);
//		temp = temp*1000*1.852;
		itemp = temp;
		gpsx->speed = itemp/pow(10,dx)*1.852;
	} 
	posx=NMEA_Comma_Pos(p1,8);								//得到速率方向
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);
//		temp = temp*1000*1.852;
//		itemp = temp;
		gpsx->angle = temp/pow(10,dx);
	}
	posx=NMEA_Comma_Pos(p1,9);								//得到UTC日期
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 				//得到UTC日期
		gpsx->utc.date=temp/10000;
		gpsx->utc.month=(temp/100)%100;
		gpsx->utc.year=2000+temp%100;	
	}
}
//分析GPVTG信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GPVTG_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
	uint8_t *p1,dx;			 
	uint8_t posx;    
	p1=(uint8_t*)strstr((const char *)buf,"$GPVTG");							 
	posx=NMEA_Comma_Pos(p1,7);								//得到地面速率
	if(posx!=0XFF)
	{
		gpsx->speed=NMEA_Str2num(p1+posx,&dx);
		if(dx<3)gpsx->speed*=NMEA_Pow(10,3-dx);	 	 		//确保扩大1000倍
	}
}  
//分析GPGLL信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GPGLL_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
	uint8_t *p1;			 
	uint8_t posx;    
	p1=(uint8_t*)strstr((const char *)buf,"GLL");							 
	posx=NMEA_Comma_Pos(p1,6);								//得到地面速率
	if(posx!=0XFF)
	{
		gpsx->gpssta=*(p1+posx);
	}
	else gpsx->gpssta='V';
}  
//提取NMEA-0183信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void GPS_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
//	NMEA_GPGSV_Analysis(gpsx,buf);	//GPGSV解析
	NMEA_GPGGA_Analysis(gpsx,buf);	//GPGGA解析 
	NMEA_GPRMC_Analysis(gpsx,buf);	//GPRMC解析	
//	NMEA_GPGLL_Analysis(gpsx,buf);	//GPGLL解析	
//	NMEA_GPGSA_Analysis(gpsx,buf);	//GPGSA解析
//	NMEA_GPVTG_Analysis(gpsx,buf);	//GPVTG解析
}

//GPS校验和计算
//buf:数据缓存区首地址
//len:数据长度
//cka,ckb:两个校验结果.
void Ublox_CheckSum(uint8_t *buf,uint16_t len,uint8_t* cka,uint8_t*ckb)
{
	uint16_t i;
	*cka=0;*ckb=0;
	for(i=0;i<len;i++)
	{
		*cka=*cka+buf[i];
		*ckb=*ckb+*cka;
	}
}

