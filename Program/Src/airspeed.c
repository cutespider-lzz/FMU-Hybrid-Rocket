#include "airspeed.h"
#include "math.h"

float calc_IAS_corrected(float tube_len, float tube_dia_mm, float differential_pressure, float pressure_ambient, float temperature_celsius)
{
	tube_len = 0;
	tube_dia_mm = 1.5;
	
	// air density in kg/m3
	float rho_air = get_air_density(pressure_ambient, temperature_celsius);
	float dp = fabsf(differential_pressure);
	float dp_tot = dp;
	float dv = 0.0f;
	
	//第一种补偿方法
	// assumes a metal pitot tube with round tip as here: https://drotek.com/shop/2986-large_default/sdp3x-airspeed-sensor-kit-sdp31.jpg
	// and tubing as provided by px4/drotek (1.5 mm diameter)
	// The tube_len represents the length of the tubes connecting the pitot to the sensor.
	const float dp_corr = dp * 96600.0f / pressure_ambient;
	// flow through sensor
	float flow_SDP33 = (300.805f - 300.878f / (0.00344205f * powf(dp_corr, 0.68698f) + 1.0f)) * 1.29f / rho_air;

	// for too small readings the compensation might result in a negative flow which causes numerical issues
	if (flow_SDP33 < 0.0f) 
	{
		flow_SDP33 = 0.0f;
	}
	float dp_pitot = 0.0f;		
	dp_pitot = (0.0032f * flow_SDP33 * flow_SDP33 + 0.0123f * flow_SDP33 + 1.0f) * 1.29f / rho_air;
	// pressure drop through tube
	const float dp_tube = (flow_SDP33 * 0.674f) / 450.0f * tube_len * rho_air / 1.29f;
	// speed at pitot-tube tip due to flow through sensor
	dv = 0.125f * flow_SDP33;
	// sum of all pressure drops
	dp_tot = dp_corr + dp_tube + dp_pitot;

	
	//第二种补偿方法
	
	// Pressure loss compensation as defined in https://goo.gl/UHV1Vv.
	// tube_dia_mm: Diameter in mm of the pitot and tubes, must have the same diameter.
	// tube_len: Length of the tubes connecting the pitot to the sensor and the static + dynamic port length of the pitot.

	// check if the tube diameter and dp is nonzero to avoid division by 0
//	if ((tube_dia_mm > 0.0f) && (dp > 0.0f)) 
//	{
//		float d_tubePow4 = powf(tube_dia_mm * 1e-3f, 4);
//		float denominator = M_PI_F * d_tubePow4 * rho_air * dp;
//		// avoid division by 0
//		float eps = 0.0f;
//		if (fabsf(denominator) > 1e-32f) 
//		{
//			float viscosity = (18.205f + 0.0484f * (temperature_celsius - 20.0f)) * 1e-6f;
//			// 4.79 * 1e-7 -> mass flow through sensor
//			// 59.5 -> dp sensor constant where linear and quadratic contribution to dp vs flow is equal
//			eps = -64.0f * tube_len * viscosity * 4.79f * 1e-7f * (sqrtf(1.0f + 8.0f * dp / 59.3319f) - 1.0f) / denominator;
//		}
//		// range check on eps
//		if (fabsf(eps) >= 1.0f) 
//		{
//			eps = 0.0f;
//		}
//		// pressure correction
//		dp_tot = dp / (1.0f + eps);
//	}
	
	// computed airspeed without correction for inflow-speed at tip of pitot-tube
	float airspeed_uncorrected = sqrtf(2.0f * dp_tot / CONSTANTS_AIR_DENSITY_SEA_LEVEL_15C);

	// corrected airspeed
	float airspeed_corrected = airspeed_uncorrected + dv;

	// return result with correct sign
	return (differential_pressure > 0.0f) ? airspeed_corrected : -airspeed_corrected;
}

float get_air_density(float static_pressure, float temperature_celsius)
{
	return static_pressure / (CONSTANTS_AIR_GAS_CONST * (temperature_celsius - CONSTANTS_ABSOLUTE_NULL_CELSIUS));
}

