#ifndef __AIRSPEED_H
#define __AIRSPEED_H

#define CONSTANTS_AIR_DENSITY_SEA_LEVEL_15C 1.225f			// kg/m^3
#define CONSTANTS_AIR_GAS_CONST  287.1f					// J/(kg * K)
#define CONSTANTS_ABSOLUTE_NULL_CELSIUS (-273.15f)				// °C
#define M_PI_F			3.14159265f

float calc_IAS_corrected(float tube_len, float tube_dia_mm, float differential_pressure, float pressure_ambient, float temperature_celsius);
float get_air_density(float static_pressure, float temperature_celsius);

#endif