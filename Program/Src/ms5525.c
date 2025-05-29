#include "ms5525.h"


//const uint8_t MS5525_Q1 = 15;
//const uint8_t MS5525_Q2 = 17;
//const uint8_t MS5525_Q3 = 7;
//const uint8_t MS5525_Q4 = 5;
//const uint8_t MS5525_Q5 = 7;
//const uint8_t MS5525_Q6 = 21;

const uint8_t MS5525_Q1 = 15;
const uint8_t MS5525_Q2 = 16;
const uint8_t MS5525_Q3 = 17;
const uint8_t MS5525_Q4 = 6;
const uint8_t MS5525_Q5 = 5;
const uint8_t MS5525_Q6 = 7;

uint16_t MS5525_C[8];
int64_t MS5525_Tref;
int32_t MS5525_Temp;
int32_t MS5525_Pre;

uint32_t MS5525_D1;
uint32_t MS5525_D2;


