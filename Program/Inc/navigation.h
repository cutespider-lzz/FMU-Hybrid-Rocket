#ifndef __navigation_h
#define __navigation_h

typedef struct
{
	double p_x,p_y,p_z;
	double v_x,v_y,v_z;
	double a_x,a_y,a_z;
	double lon,lat,alt;
	double pre,helght;
}NavigationDataStruct;

#endif


void NevigationSolution(void);
	
double Lon2Distance(double lon_1,double lon_2); //lon1-lon2的距离
double Lat2Distance(double lat_1,double lat_2); //lat1-lat2的距离
double Distance2Lon(double px_1,double px_2); //东向距离转换为经度
double Distance2Lat(double py_1,double py_2); //北向距离转换为纬度
