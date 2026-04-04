/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "fatfs.h"
#include "sdio.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SINE 1
#define TRIANGLE 4

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern uint32_t dmaDataBuffer[BATCH_DATA_LEN];  //DMA数据缓冲区

uint16_t threshold = 450;

uint8_t HEAD = 0X55;
uint8_t END = 0XAA;
uint8_t A = 0X0A;
uint8_t B = 0X0B;
uint8_t test;  //0为测试阶段比较频率并保存数据，1为当场测评
uint8_t f_type_A, f_type_B;  //A、B信号类型，4为三角波，1为正弦波
uint8_t f_A, f_B;  //A、B信号频率
uint8_t flag_A, flag_B;  //A、B频率字误差的正负，0为正，1为负
uint8_t fre_step_err_A, fre_step_err_B;  //信号A、B的频率字误差
uint8_t pha_x_A = 1, pha_x_B = 1;  //A、B的初相位参数x，因为题目要求的0°~180°，所以x始终为0
uint8_t pha_y_A = 1, pha_y_B = 1;  //A、B的初相位参数y

uint8_t flag[20] = {0};  ////频率字误差符号数组，查找表
uint8_t fre_step_err[20] = {0};  //频率字误差数组，查找表

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void judge_sin_tri(float32_t *amp)  //2、3参数为信号的频率，4、5参数为信号的类型，0为正弦波、1为三角波
{
    float32_t max = 0, second_max = 0, third_max = 0, forth_max = 0;
    uint8_t max_index = 0, second_max_index = 0, third_max_index = 0, forth_max_index = 0;
    uint8_t temp;
    for(uint16_t i = 1; i < BATCH_DATA_LEN; i++)
    {
        if(max < amp[i])
        {
            max = amp[i];
            max_index = i;
        }
    }
    for(uint16_t i = 1; i < BATCH_DATA_LEN; i++)
    {
        if(i == max_index)
            continue;
        if(second_max < amp[i])
        {
            second_max = amp[i];
            second_max_index = i;
        }
    }
    if(max < threshold)
    {
        f_type_A = TRIANGLE;
        f_A = (max_index > second_max_index ? second_max_index : max_index) * 2.5;
        f_type_B = TRIANGLE;
        f_B = (max_index < second_max_index ? second_max_index : max_index) * 2.5;
    }
    else if(second_max > threshold)
    {
        f_type_A = SINE;
        f_A = (max_index > second_max_index ? second_max_index : max_index) * 2.5;
        f_type_B = SINE;
        f_B = (max_index < second_max_index ? second_max_index : max_index) * 2.5;
    }
    else
    {
        f_type_A = amp[max_index > second_max_index ? second_max_index : max_index] > 450 ? SINE : TRIANGLE;
        f_A = (max_index > second_max_index ? second_max_index : max_index) * 2.5;
        f_type_B = amp[max_index < second_max_index ? second_max_index : max_index] > 450 ? SINE : TRIANGLE;;
        f_B = (max_index < second_max_index ? second_max_index : max_index) * 2.5;
    }
}

void set_fre_err(void)
{
    if(f_A == 20)
    {
        flag_A = flag[0];
        fre_step_err_A = fre_step_err[0];
    }
    if(f_A == 25)
    {
        flag_A = flag[1];
        fre_step_err_A = fre_step_err[1];
    }
    if(f_A == 30)
    {
        flag_A = flag[2];
        fre_step_err_A = fre_step_err[2];
    }
		if(f_A == 35)
    {
        flag_A = flag[3];
        fre_step_err_A = fre_step_err[3];
    }
		if(f_A == 40)
    {
        flag_A = flag[4];
        fre_step_err_A = fre_step_err[4];
    }
		if(f_A == 45)
    {
        flag_A = flag[5];
        fre_step_err_A = fre_step_err[5];
    }
		if(f_A == 50)
    {
        flag_A = flag[6];
        fre_step_err_A = fre_step_err[6];
    }
		if(f_A == 55)
    {
        flag_A = flag[7];
        fre_step_err_A = fre_step_err[7];
    }
		if(f_A == 60)
    {
        flag_A = flag[8];
        fre_step_err_A = fre_step_err[8];
    } 
		if(f_A == 65)
    {
        flag_A = flag[9];
        fre_step_err_A = fre_step_err[9];
    }
		if(f_A == 70)
    {
        flag_A = flag[10];
        fre_step_err_A = fre_step_err[10];
    }
		if(f_A == 75)
    {
        flag_A = flag[11];
        fre_step_err_A = fre_step_err[11];
    }
		if(f_A == 80)
    {
        flag_A = flag[12];
        fre_step_err_A = fre_step_err[12];
    }
		if(f_A == 85)
    {
        flag_A = flag[13];
        fre_step_err_A = fre_step_err[13];
    }
		if(f_A == 90)
    {
        flag_A = flag[14];
        fre_step_err_A = fre_step_err[14];
    }
		if(f_A == 95)
    {
        flag_A = flag[15];
        fre_step_err_A = fre_step_err[15];
    }
		if(f_A == 100)
    {
        flag_A = flag[16];
        fre_step_err_A = fre_step_err[16];
    }


    if(f_B == 20)
    {
        flag_B = flag[0];
        fre_step_err_B = fre_step_err[0];
    }
    if(f_B == 25)
    {
        flag_B = flag[1];
        fre_step_err_B = fre_step_err[1];
    }
    if(f_B == 30)
    {
        flag_B = flag[2];
        fre_step_err_B = fre_step_err[2];
    }
		if(f_B == 35)
    {
        flag_B = flag[3];
        fre_step_err_B = fre_step_err[3];
    }
		if(f_B == 40)
    {
        flag_B = flag[4];
        fre_step_err_B = fre_step_err[4];
    }
		if(f_B == 45)
    {
        flag_B = flag[5];
        fre_step_err_B = fre_step_err[5];
    }
		if(f_B == 50)
    {
        flag_B = flag[6];
        fre_step_err_B = fre_step_err[6];
    } 
		if(f_B == 55)
    {
        flag_B = flag[7];
        fre_step_err_B = fre_step_err[7];
    }
		if(f_B == 60)
    {
        flag_B = flag[8];
        fre_step_err_B = fre_step_err[8];
    }
		if(f_B == 65)
    {
        flag_B = flag[9];
        fre_step_err_B = fre_step_err[9];
    }
		if(f_B == 70)
    {
        flag_B = flag[10];
        fre_step_err_B = fre_step_err[10];
    }
		if(f_B == 75)
    {
        flag_B = flag[11];
        fre_step_err_B = fre_step_err[11];
    }
		if(f_B == 80)
    {
        flag_B = flag[12];
        fre_step_err_B = fre_step_err[12];
    }
		if(f_B == 85)
    {
        flag_B = flag[13];
        fre_step_err_B = fre_step_err[13];
    }
		if(f_B == 90)
    {
        flag_B = flag[14];
        fre_step_err_B = fre_step_err[14];
    }
		if(f_B == 95)
    {
        flag_B = flag[15];
        fre_step_err_B = fre_step_err[15];
    }
		if(f_B == 100)
    {
        flag_B = flag[16];
        fre_step_err_B = fre_step_err[16];
    }
}

void comunicate_FPGA(void)
{
    set_fre_err();
    HAL_UART_Transmit(&huart3, &HEAD, 1, 200);
    HAL_UART_Transmit(&huart3, &A, 1, 200);
    HAL_UART_Transmit(&huart3, &f_type_A, 1, 200);
    HAL_UART_Transmit(&huart3, &f_A, 1, 200);
    HAL_UART_Transmit(&huart3, &flag_A, 1, 200);
    HAL_UART_Transmit(&huart3, &fre_step_err_A, 1, 200);
    HAL_UART_Transmit(&huart3, &pha_y_A, 1, 200);
    HAL_UART_Transmit(&huart3, &END, 1, 200);

    HAL_UART_Transmit(&huart3, &HEAD, 1, 200);
    HAL_UART_Transmit(&huart3, &B, 1, 200);
    HAL_UART_Transmit(&huart3, &f_type_B, 1, 200);
    HAL_UART_Transmit(&huart3, &f_B, 1, 200);
    HAL_UART_Transmit(&huart3, &flag_B, 1, 200);
    HAL_UART_Transmit(&huart3, &fre_step_err_B, 1, 200);
    HAL_UART_Transmit(&huart3, &pha_y_B, 1, 200);
    HAL_UART_Transmit(&huart3, &END, 1, 200);

//    printf("f_type_A:%d\n", f_type_A);
//    printf("f_A:%d\n", f_A);
//    printf("fre_step_err_A:%d\n", fre_step_err_A);
//    printf("pha_y_A:%d\n\n", pha_y_A);

//    printf("f_type_B:%d\n", f_type_B);
//    printf("f_B:%d\n", f_B);
//    printf("fre_step_err_B:%d\n", fre_step_err_B);
//    printf("pha_y_B:%d\n\n", pha_y_B);
}

void comunicate_HMI(void)
{
		if(f_type_A == SINE)
		{
				printf("page0.t5.txt=\"正弦波\"\xff\xff\xff");
		}else
		{
				printf("page0.t5.txt=\"三角波\"\xff\xff\xff");
		}
		
		if(f_type_B == SINE)
		{
				printf("page0.t6.txt=\"正弦波\"\xff\xff\xff");
		}else
		{
				printf("page0.t6.txt=\"三角波\"\xff\xff\xff");
		}
		
		if(f_type_A == SINE)
		{
				printf("page0.t5.txt=\"正弦波\"\xff\xff\xff");
		}else
		{
				printf("page0.t5.txt=\"三角波\"\xff\xff\xff");
		}
		printf("page0.n0.val=%d\xff\xff\xff", f_A);
		printf("page0.n1.val=%d\xff\xff\xff", f_B);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */
    FRESULT f_res;
    FIL file;
    FATFS SD;
    int len, len2;

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_TIM3_Init();
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    MX_USART3_UART_Init();
    MX_SDIO_SD_Init();
    MX_FATFS_Init();
    /* USER CODE BEGIN 2 */
    HAL_UART_Receive_IT(&huart2, rxBuffer2, RX_CMD_LEN2); //启动中断接收；
    HAL_ADC_Start_DMA(&hadc1, dmaDataBuffer, BATCH_DATA_LEN);  //启动ADC1，DMA方式；
    HAL_TIM_Base_Start(&htim3);  //启动定时器TIM3；

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
				comunicate_FPGA();
				comunicate_HMI();
        HAL_Delay(2000);
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 64;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 3;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
