#include "bsp_sd.h"
 
SD_HandleTypeDef SD_Handle;
 
/**
*  函数名:SD_Init
*  描述:SD 卡初始化
*  输入:
*  输出:返回值:0 初始化正确;
*/
uint8_t SDCard_Init(void)
{
	uint8_t sta;
	
	__HAL_RCC_SDIO_CLK_ENABLE();
	__SDIO_CLK_ENABLE();
	
	SD_Handle.Instance = SDIO; 
	SD_Handle.Init.BusWide = SDIO_BUS_WIDE_1B; // 1 位数据线
	SD_Handle.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE; // 不使用 bypass 模式
	SD_Handle.Init.ClockDiv = SDIO_TRANSFER_DIR_TO_SDIO; // SD 传输时钟频率
	SD_Handle.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING; // 上升沿
	SD_Handle.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE; // 空闲时不关闭时钟电源
	SD_Handle.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE; // 关闭硬件流控
 
	sta = HAL_SD_Init(&SD_Handle);
	
	HAL_SD_ConfigWideBusOperation(&SD_Handle,SDIO_BUS_WIDE_4B); // 使能4 位宽总线模式
	
	return sta;
}
 
/**
*  函数名:HAL_SD_MspInit
*  描述:SD 卡初始化回调函数 用于引脚初始化
*  输入:hsd句柄
*  输出:返回值:0 初始化正确;
*/
void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
	
	GPIO_InitStruct.Pin = Pin_SDIO_D0_D3 | Pin_SDIO_SCK; // D0~D3 SCK引脚
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = Pin_SDIO_CMD;	// CMD引脚
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}
 
/**
*  函数名:SD_GetCardInfo
*  描述:获取卡信息
*  输入:HAL_SD_CardInfoTypeDef
*  输出:状态值
*/
uint8_t SD_GetCardInfo(HAL_SD_CardInfoTypeDef *cardinfo)
{
	uint8_t sta;
	sta = HAL_SD_GetCardInfo(&SD_Handle, cardinfo); // 获取卡信息
	return sta;
}
 
// 打印SD卡信息
void SDCard_Show_Info(HAL_SD_CardInfoTypeDef *cardinfo)
{
	switch(cardinfo->CardType) //卡类型
	{
		case SDIO_STD_CAPACITY_SD_CARD_V1_1:
			printf("Card Type:SDSC V1.1\r\n");
		break;
		case SDIO_STD_CAPACITY_SD_CARD_V2_0:
			printf("Card Type:SDSC V2.0\r\n");
		break;
		case SDIO_HIGH_CAPACITY_SD_CARD:
			printf("Card Type:SDHC V2.0\r\n");
		break;
		case SDIO_MULTIMEDIA_CARD:
			printf("Card Type:MMC Card\r\n");
		break;
	}
	printf("Card CardVersion:%d\r\n", cardinfo->CardVersion); // 版本号
	printf("Card RelCardAdd:%d\r\n", cardinfo->RelCardAdd); // 卡相对地址
	printf("Card BlockNbr:%d\r\n", cardinfo->BlockNbr); // 显示块数量
	printf("Card BlockSize:%d\r\n", cardinfo->BlockSize); // 显示块大小
}
 
/**
*  函数名:SD_ReadDisk
*  描述:读 SD 卡
*  输入:buf:读数据缓存区 sector:扇区地址 cnt:扇区个数
*  输出:返回值:错误状态;0,正常;
*/
uint8_t SD_ReadDisk(uint8_t* buf, uint32_t sector, uint32_t cnt)
{
	uint8_t sta;
 
	sta = HAL_SD_ReadBlocks(&SD_Handle, buf, sector, cnt, 1000);
 
	return sta;
}
 
/**
*  函数名:SD_WriteDisk
*  描述:写 SD 卡
*  输入:buf:写数据缓存区 sector:扇区地址 cnt:扇区个数
*  输出:返回值:错误状态;0,正常;
*/
uint8_t SD_WriteDisk(uint8_t *buf, uint32_t sector, uint32_t cnt)
{
	uint8_t sta;
 
	sta = HAL_SD_WriteBlocks(&SD_Handle, buf, sector, cnt, 1000);
 
	return sta;
}