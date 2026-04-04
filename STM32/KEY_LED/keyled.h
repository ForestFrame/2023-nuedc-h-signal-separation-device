/*
 * keyled.h
 *
 *  Created on: 2022年8月12日
 *      Author: forest
 */

#ifndef KEY_LED_KEYLED_H_
#define KEY_LED_KEYLED_H_

#include "main.h"

typedef enum
{
    KEY_NONE = 0,	//没有按键被按下
    KEY0,			//左边KEY1按键
    KEY_UP			//右边KEY2按键
} KEYS;

#define	KEY_WAIT_ALWAYS	0	//作为函数ScanKeys()的一种参数，表示一直等待按键输入
//轮询方式扫描按键，timeout = KEY_WAIT_ALWAYS时一直扫描，否则等待时间timeout，单位ms

KEYS ScanPressedKey(uint32_t timeout);

#ifdef	LED1_Pin	//绿灯的控制
    #define	LED_G_Pin_Toggle()	HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin)	//输出反转
    #define	LED_G_Pin_ON()	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET)
    #define LED_G_Pin_OFF()	 HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET)
#endif

#ifdef	LED0_Pin	//红灯的控制
    #define	LED_R_Pin_Toggle()	HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin)	//输出反转
    #define	LED_R_Pin_ON()	HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET)
    #define LED_R_Pin_OFF()	 HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET)
#endif

#endif /* KEY_LED_KEYLED_H_ */
