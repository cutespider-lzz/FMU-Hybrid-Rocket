#include "main.h"

void HAL_Delay(uint32_t Delay)
{
	__HAL_TIM_SET_COUNTER(&htim5,0);
	__HAL_TIM_ENABLE(&htim5);
  while (__HAL_TIM_GET_COUNTER(&htim5) < Delay)
  {
  }
	__HAL_TIM_DISABLE(&htim5);
}
