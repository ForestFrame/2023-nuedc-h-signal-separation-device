#ifndef TIMER_DMA_ADC_H
#define TIMER_DMA_ADC_H

#include "main.h"

#define BATCH_DATA_LEN 512 //DMA数据缓冲区长度
#define ADC_MAX 3.3

float adcDatatoVolt_mV(int32_t adcData);
float adcDatatoVolt_V(int32_t adcData);

#endif

