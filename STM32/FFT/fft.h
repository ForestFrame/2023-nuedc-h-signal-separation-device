#ifndef FFT_H
#define FFT_H

#include "main.h"

#define FFT_SIZE 512
#define arm_cfft_sR_f32_lenx arm_cfft_sR_f32_len512

void my_FFT(float *volt, float *amp);

#endif

