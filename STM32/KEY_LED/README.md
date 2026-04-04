- 在main.h文件中添加#include "keyled.h"头文件；

- 时钟滴答计时器的中断优先级要高于外部中断优先级；

- HAL_GPIO_EXTI_IRQHandler()函数中的回调函数和清除中断挂起函数位置需要互换。