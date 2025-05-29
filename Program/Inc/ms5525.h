#ifndef __MS5525_H
#define __MS5525_H

#include "main.h"

// Qx Coefficients Matrix by Pressure Range
//  5525DSO-pp001DS (Pmin = -1, Pmax = 1)
extern const uint8_t MS5525_Q1;
extern const uint8_t MS5525_Q2;
extern const uint8_t MS5525_Q3;
extern const uint8_t MS5525_Q4;
extern const uint8_t MS5525_Q5; 
extern const uint8_t MS5525_Q6;

// calibration coefficients from prom
extern uint16_t MS5525_C[];
extern int64_t MS5525_Tref;

// last readings for D1 (uncompensated pressure) and D2 (uncompensated temperature)
extern uint32_t MS5525_D1;
extern uint32_t MS5525_D2;

#endif

