#include "fft.h"

float processArr[FFT_SIZE * 2];
float testOutput[FFT_SIZE];
float amp[FFT_SIZE / 2];
uint8_t transDir = 0;
uint8_t doBitReverse = 1;

void my_FFT(float *volt, float *amp)
{
    for (uint16_t i = 0, j = 0; i < FFT_SIZE * 2; i++)
    {
        if ((i % 2) == 0)
        {
            processArr[i] = volt[j];
            j++;
        }
        else
        {
            processArr[i] = 0;
        }
    }

    arm_cfft_f32(&arm_cfft_sR_f32_lenx, processArr, transDir, doBitReverse);
    arm_cmplx_mag_f32(processArr, testOutput, FFT_SIZE);

    // 若要得到每个频率分量的真实幅值，则上面取模后的每个结果除以(N/2),第一个点除N
    for (uint16_t i = 0; i < FFT_SIZE / 2; i++)
    {
        if (i == 0)
            amp[i] = testOutput[i] / FFT_SIZE;
        else
            amp[i] = testOutput[i] * 2 / FFT_SIZE;
    }
}
