#include "keyled.h"

extern uint32_t dmaDataBuffer[BATCH_DATA_LEN];  //DMA数据缓冲区
extern ADC_HandleTypeDef hadc1;

//轮询方式扫描两个按键，返回按键值
//timeout的单位为ms，若timeout = 0表示一直扫描，直到有键按下
KEYS ScanPressedKey(uint32_t timeout)
{
    uint32_t tickstart = HAL_GetTick();	//当前计数值
    const uint32_t	btnDelay = 20;	//按键按下阶段的抖动，延时再采样时间
    GPIO_PinState keyState;	//引脚输入状态

    while(1)
    {
        #ifdef	KEY0_Pin	//如果定义了宏KEY0_Pin,就可以检测KEY1键输入，高输入有效
        keyState = HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin);

        if(keyState == GPIO_PIN_SET)
        {
            HAL_Delay(btnDelay);	//延时跳过前抖动期
            keyState = HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin);	//再采样

            while(HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin));	//等待按键弹起

            HAL_Delay(btnDelay);	//延时跳过后抖动期

            if(keyState == GPIO_PIN_SET)
                return KEY0;
        }

        #endif

        #ifdef	KEY_UP_Pin	//如果定义了宏KEY2_Pin,就可以检测KEY2键输入，高输入有效
        keyState = HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin);

        if(keyState == GPIO_PIN_SET)
        {
            HAL_Delay(btnDelay);	//延时跳过前抖动期
            keyState = HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin);	//再采样

            while(HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin));	//等待按键弹起

            HAL_Delay(btnDelay);	//延时跳过后抖动期

            if(keyState == GPIO_PIN_SET)
                return KEY_UP;
        }

        if(timeout != KEY_WAIT_ALWAYS)	//没有按键按下时会计算超时，timeout时退出
        {
            if((HAL_GetTick() - tickstart) > timeout)
                break;
        }

        #endif
    }

    return KEY_NONE;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
		HAL_Delay(200);
    HAL_ADC_Start_DMA(&hadc1, dmaDataBuffer, BATCH_DATA_LEN);  //启动ADC1，DMA方式；
}

