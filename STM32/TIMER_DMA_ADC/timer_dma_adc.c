#include "timer_dma_adc.h"

uint32_t dmaDataBuffer[BATCH_DATA_LEN]; // DMA数据缓冲区
float Volt[BATCH_DATA_LEN];
extern float amp[FFT_SIZE / 2];

extern uint8_t HEAD;
extern uint8_t END;
extern uint8_t A;
extern uint8_t B;
extern uint8_t test;                   // 0为测试阶段比较频率并保存数据，1为当场测评
extern uint8_t f_type_A, f_type_B;     // A、B信号类型，4为三角波，1为正弦波
extern uint8_t f_A, f_B;               // A、B信号频率
extern uint8_t flag_A, flag_B;         // A、B误差是增还是减，0为正，1为负
extern uint8_t fre_rang_A, fre_rang_B; // A、B频率误差值
extern uint8_t pha_x_A, pha_x_B;       // A、B的初相位参数x，因为题目要求的0°~180°，所以x始终为0
extern uint8_t pha_y_A, pha_y_B;       // A、B的初相位参数y
extern uint8_t fre_range_buf[20];      // 误差值数组，从20kHz~100kHz，5kHz为倍数
extern uint8_t flag;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    for (uint16_t i = 0; i < BATCH_DATA_LEN; i++)
    {
        Volt[i] = adcDatatoVolt_mV(dmaDataBuffer[i]);
    }
    my_FFT(Volt, amp);
    judge_sin_tri(amp);
}

float adcDatatoVolt_mV(int32_t adcData) // 单位为mV
{
    return (ADC_MAX * 1000 * adcData / 64);
}

float adcDatatoVolt_V(int32_t adcData) // 单位为V
{
    return (ADC_MAX * adcData / 64);
}
