#ifndef HAL_DELAY_US_H
#define HAL_DELAY_US_H

#include "main.h"

#define CPU_FREQUENCY_MHZ  168  // STM32系统时钟频率

void delay_us(__IO uint32_t delay);

#endif

