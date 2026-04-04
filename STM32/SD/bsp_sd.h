#ifndef __BSP_SD_H
#define __BSP_SD_H
 
#include "stm32f4xx.h"
 
// 支持的SD卡定义
#define SDIO_STD_CAPACITY_SD_CARD_V1_1             ((uint32_t)0x00000000)
#define SDIO_STD_CAPACITY_SD_CARD_V2_0             ((uint32_t)0x00000001)
#define SDIO_HIGH_CAPACITY_SD_CARD                 ((uint32_t)0x00000002)
#define SDIO_MULTIMEDIA_CARD                       ((uint32_t)0x00000003)
#define SDIO_SECURE_DIGITAL_IO_CARD                ((uint32_t)0x00000004)
#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD            ((uint32_t)0x00000005)
#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD          ((uint32_t)0x00000006)
#define SDIO_HIGH_CAPACITY_MMC_CARD                ((uint32_t)0x00000007)
 
// GPIOC D0-D3  SCK
#define Pin_SDIO_D0_D3	 GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11
#define Pin_SDIO_SCK 		 GPIO_PIN_12
// GPIOD CMD
#define Pin_SDIO_CMD		 GPIO_PIN_2
 
uint8_t SDCard_Init(void); // 初始化函数
uint8_t SD_GetCardInfo(HAL_SD_CardInfoTypeDef *cardinfo); // 获取sd卡信息 
void    SDCard_Show_Info(HAL_SD_CardInfoTypeDef *cardinfo);	// 打印sd卡信息
uint8_t SD_ReadDisk(uint8_t* buf, uint32_t sector, uint32_t cnt);	// 读取数据
uint8_t SD_WriteDisk(uint8_t *buf, uint32_t sector, uint32_t cnt); // 写入数据
#endif