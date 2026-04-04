#ifndef USART_H
#define USART_H

#include "main.h"

extern uint8_t rxBuffer2[];	//接收缓存数据

#define RX_CMD_LEN2 4

#define huartx huart2

void on_UART_IDLE(UART_HandleTypeDef *huart);  //IDLE串口空闲中断
void drawWaveform_mV(uint8_t channel, float volt);
void drawWaveform_V(uint8_t channel, float volt);

#endif /* USART_H */

