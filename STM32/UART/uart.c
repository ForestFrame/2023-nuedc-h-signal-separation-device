#include "usart.h"
#include "uart.h"
#include "stdio.h"
#include <string.h>  //用到函数strlen()

/*--------------------串口2相关变量--------------------*/
uint8_t	proBuffer2[RX_CMD_LEN2];  //用于处理数据
uint8_t	rxBuffer2[RX_CMD_LEN2];  //接收缓存数据

uint8_t	rxCompleted2 = RESET;  //HAL_UART_Receive_IT()接收是否完成

extern uint16_t threshold;

extern uint8_t HEAD;
extern uint8_t END;
extern uint8_t A;
extern uint8_t B;
extern uint8_t test;  //0为测试阶段比较频率并保存数据，1为当场测评
extern uint8_t f_type_A, f_type_B;  //A、B信号类型
extern uint8_t f_A, f_B;  //A、B信号频率
extern uint8_t fre_step_A, fre_step_B;  //信号A、B的频率字
extern uint8_t pha_x_A, pha_x_B;  //A、B的初相位参数x，因为题目要求的0°~180°，所以x始终为0
extern uint8_t pha_y_A, pha_y_B;  //A、B的初相位参数y

extern uint8_t flag[20];  ////频率字误差符号数组，查找表
extern uint8_t fre_step_err[20];  //频率字偏差数组，查找表

/*-----回调函数和空闲中断检测函数实现-----*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART2)
    {
        //接收到固定长度数据后使能UART_IT_IDLE中断，在UART_IT_IDLE中断里再次接收
        rxCompleted2 = SET;  //接收完成

            if(rxBuffer2[0] == 0X55)
            {
                if(rxBuffer2[1] == 20)
                {
										if(rxBuffer2[2] > 128)
										{
												flag[0] = 1;
												fre_step_err[0] = 256 - rxBuffer2[2];
										}
										else
										{
												flag[0] = 0;
												fre_step_err[0] = rxBuffer2[2];
										}											
                }
                if(rxBuffer2[1] == 25)
                {
										if(rxBuffer2[2] > 128)
										{
												flag[1] = 1;
												fre_step_err[1] = 256 - rxBuffer2[2];
										}
										else
										{
												flag[1] = 0;
												fre_step_err[1] = rxBuffer2[2];
										}			
                }
                if(rxBuffer2[1] == 30)
                {
										if(rxBuffer2[2] > 128)
										{
												flag[2] = 1;
												fre_step_err[2] = 256 - rxBuffer2[2];
										}
										else
										{
												flag[2] = 0;
												fre_step_err[2] = rxBuffer2[2];
										}
                }
                if(rxBuffer2[1] == 35)
                {
										if(rxBuffer2[2] > 128)
										{
												flag[3] = 1;
												fre_step_err[3] = 256 - rxBuffer2[2];
										}
										else
										{
												flag[3] = 0;
												fre_step_err[3] = rxBuffer2[2];
										}
                }
                if(rxBuffer2[1] == 40)
                {
										if(rxBuffer2[2] > 128)
										{
												flag[4] = 1;
												fre_step_err[4] = 256 - rxBuffer2[2];
										}
										else
										{
												flag[4] = 0;
												fre_step_err[4] = rxBuffer2[2];
										}
                }
                if(rxBuffer2[1] == 45)
                {
										if(rxBuffer2[2] > 128)
										{
												flag[5] = 1;
												fre_step_err[5] = 256 - rxBuffer2[2];
										}
										else
										{
												flag[5] = 0;
												fre_step_err[5] = rxBuffer2[2];
										}
                }
                if(rxBuffer2[1] == 50)
                {
										if(rxBuffer2[2] > 128)
										{
												flag[6] = 1;
												fre_step_err[6] = 256 - rxBuffer2[2];
										}
										else
										{
												flag[6] = 0;
												fre_step_err[6] = rxBuffer2[2];
										}
                }
                if(rxBuffer2[1] == 55)
                {
										if(rxBuffer2[2] > 128)
										{
												flag[7] = 1;
												fre_step_err[7] = 256 - rxBuffer2[2];
										}
										else
										{
												flag[7] = 0;
												fre_step_err[7] = rxBuffer2[2];
										}
                }
                if(rxBuffer2[1] == 60)
                {
										if(rxBuffer2[2] > 128)
										{
												flag[8] = 1;
												fre_step_err[8] = 256 - rxBuffer2[2];
										}
										else
										{
												flag[8] = 0;
												fre_step_err[8] = rxBuffer2[2];
										}
                }
                if(rxBuffer2[1] == 65)
                {
										if(rxBuffer2[2] > 128)
										{
												flag[9] = 1;
												fre_step_err[9] = 256 - rxBuffer2[2];
										}
										else
										{
												flag[9] = 0;
												fre_step_err[9] = rxBuffer2[2];
										}
                }
                if(rxBuffer2[1] == 70)
                {
										if(rxBuffer2[2] > 128)
										{
												flag[10] = 1;
												fre_step_err[10] = 256 - rxBuffer2[2];
										}
										else
										{
												flag[10] = 0;
												fre_step_err[10] = rxBuffer2[2];
										}
                }
                if(rxBuffer2[1] == 75)
                {
										if(rxBuffer2[2] > 128)
										{
												flag[11] = 1;
												fre_step_err[11] = 256 - rxBuffer2[2];
										}
										else
										{
												flag[11] = 0;
												fre_step_err[11] = rxBuffer2[2];
										}
                }
                if(rxBuffer2[1] == 80)
                {
										if(rxBuffer2[2] > 128)
										{
												flag[12] = 1;
												fre_step_err[12] = 256 - rxBuffer2[2];
										}
										else
										{
												flag[12] = 0;
												fre_step_err[12] = rxBuffer2[2];
										}
                }
                if(rxBuffer2[1] == 85)
                {
										if(rxBuffer2[2] > 128)
										{
												flag[13] = 1;
												fre_step_err[13] = 256 - rxBuffer2[2];
										}
										else
										{
												flag[13] = 0;
												fre_step_err[13] = rxBuffer2[2];
										}
                }
                if(rxBuffer2[1] == 90)
                {
										if(rxBuffer2[2] > 128)
										{
												flag[14] = 1;
												fre_step_err[14] = 256 - rxBuffer2[2];
										}
										else
										{
												flag[14] = 0;
												fre_step_err[14] = rxBuffer2[2];
										}
                }
                if(rxBuffer2[1] == 95)
                {
										if(rxBuffer2[2] > 128)
										{
												flag[15] = 1;
												fre_step_err[15] = 256 - rxBuffer2[2];
										}
										else
										{
												flag[15] = 0;
												fre_step_err[15] = rxBuffer2[2];
										}
                }
                if(rxBuffer2[1] == 100)
                {
										if(rxBuffer2[2] > 128)
										{
												flag[16] = 1;
												fre_step_err[16] = 256 - rxBuffer2[2];
										}
										else
										{
												flag[16] = 0;
												fre_step_err[16] = rxBuffer2[2];
										}
                }
            }

        if(rxBuffer2[0] == 0X66)
        {
            if(rxBuffer2[1] == 0XCC)
            {
								if(rxBuffer2[2] == 0)
								{
										pha_y_A = 1;
								}
								if(rxBuffer2[2] == 5)
								{
										pha_y_A = 255;
								}
								else
									pha_y_A = 180 / rxBuffer2[2] * 10;
            }
            if(rxBuffer2[1] == 0XDD)
            {
								if(rxBuffer2[2] == 0)
								{
										pha_y_B = 1;
								}
								if(rxBuffer2[2] == 5)
								{
										pha_y_B = 255;
								}
								else
									pha_y_B = 180 / rxBuffer2[2] * 10;
            }
        }
				
				if(rxBuffer2[0] == 0X77)
				{
						if(rxBuffer2[1] > 128)
						{
								threshold = threshold - (256 - rxBuffer2[1]);
						}
						else
								threshold = threshold + rxBuffer2[1];
				}

        __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);  //接收到数据后才开放IDLE中断
    }
}

void on_UART_IDLE(UART_HandleTypeDef *huart)  //在串口IDLE时处能
{
    if(__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) == RESET)  //IDLE中断挂起标志位是否置位
        return;

    __HAL_UART_CLEAR_IDLEFLAG(huart);  //清除IDLE挂起标志
    __HAL_UART_DISABLE_IT(huart, UART_IT_IDLE);  //禁止IDLE事件中断

    if(rxCompleted2)  //接收到1条指令
    {
        HAL_Delay(10);  //需要适当延时
				for(uint16_t i = 0; i < RX_CMD_LEN2; i++)
				{
					rxBuffer2[i] = 0;
				}
        rxCompleted2 = RESET;
        HAL_UART_Receive_IT(huart, rxBuffer2, RX_CMD_LEN2);  //再次启动串口接收
    }
}

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huartx, (uint8_t *)&ch, 1, 0xFFFF);  //阻塞方式打印
    return ch;
}

void drawWaveform_mV(uint8_t channel, float volt)
{
    printf("add s0.id,%d,%d\xff\xff\xff", channel, (uint8_t)((volt / 1000 + 4) *  256 / 8));
}

void drawWaveform_V(uint8_t channel, float volt)
{
    printf("add s0.id,%d,%d\xff\xff\xff", channel, (uint8_t)((volt + 4) *  256 / 8));
}





